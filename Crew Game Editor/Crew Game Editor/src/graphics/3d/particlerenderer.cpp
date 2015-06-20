#include "particlerenderer.h"
#include <algorithm>
#include <glm/gtx/norm.hpp>

namespace zakix {

	ParticleRenderer::ParticleRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation) {
		shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
		this->projectionLocation = shader->getUniformLocation(projectionLocation.c_str());
		this->modelViewLocation = shader->getUniformLocation(modelViewLocation.c_str());
		cameraRightLocation = shader->getUniformLocation("CameraRight_worldspace");
		cameraUpLocation = shader->getUniformLocation("CameraUp_worldspace");

		shader->enable();

		GLint texIDs[] =
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31
		};

		shader->setUniform1iv("textures", texIDs, 32);
		shader->disable();

		init();
	}

	void ParticleRenderer::applyProjection(const mat4& projectionMatrix) const {
		shader->enable();
		shader->setUniformMat4(projectionLocation, projectionMatrix);
		shader->disable();
	}

	void ParticleRenderer::applyModelView(const mat4& modelViewMatrix) const {
		shader->setUniformMat4(modelViewLocation, modelViewMatrix);
		shader->setUniform3f(cameraRightLocation, vec3(modelViewMatrix[0][0], modelViewMatrix[1][0], modelViewMatrix[2][0]));
		shader->setUniform3f(cameraUpLocation, vec3(modelViewMatrix[0][1], modelViewMatrix[1][1], modelViewMatrix[2][1]));
	}
	
	void ParticleRenderer::render(const mat4& modelView, std::vector<Particle*>& particles) {
		shader->enable();
		applyModelView(modelView);
		glm::vec3 cameraPosition(glm::inverse(modelView)[3]);

		const unsigned int particlesAmount = particles.size();
		unsigned int offset = 0;

		for (unsigned int i = 0; i < particlesAmount; i++) {
			Particle* p = particles[i];
			const vec3& position = p->getPosition();
			p->setDistanceToCamera(glm::length2(position - cameraPosition));
		}

		std::sort(particles.begin(), particles.end());

		for (unsigned int i = 0; i < particlesAmount; i++) {
			Particle *p = particles[i];
			const vec3& position = p->getPosition();
			const vec4& color = p->getColor();
			const vec4& uv = p->getUV();
			const GLuint tid = p->getTexture();

			float ts = 0.0f;
			if (tid > 0)
			{
				bool found = false;
				for (unsigned int i = 0; i < textureSlots.size(); i++)
				{
					if (textureSlots[i] == tid)
					{
						ts = (float)(i + 1);
						found = true;
						break;
					}
				}

				if (!found)
				{
					if (textureSlots.size() >= PARTICLE_MAX_TEXTURES)
					{
						flush(offset, i);
						offset = i;
					}
					textureSlots.push_back(tid);
					ts = (float)(textureSlots.size());
				}
			}

			positionData[4 * i + 0] = position.x;
			positionData[4 * i + 1] = position.y;
			positionData[4 * i + 2] = position.z;

			positionData[4 * i + 3] = p->getSize();

			colorData[4 * i + 0] = color.x;
			colorData[4 * i + 1] = color.y;
			colorData[4 * i + 2] = color.z;
			colorData[4 * i + 3] = color.w;

			uvData[4 * i + 0] = uv.x;
			uvData[4 * i + 1] = uv.y;
			uvData[4 * i + 2] = uv.z;
			uvData[4 * i + 3] = uv.w;

			textureData[i] = ts;
		}

		flush(offset, particlesAmount);
		shader->disable();
	}

	void ParticleRenderer::flush(GLuint offset, GLuint particlesAmount) {
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, offset, particlesAmount * sizeof(GLfloat)* 4, positionData);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, offset, particlesAmount * sizeof(GLfloat)* 4, colorData);

		glBindBuffer(GL_ARRAY_BUFFER, particles_uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, offset, particlesAmount * sizeof(GLfloat)* 4, uvData);

		glBindBuffer(GL_ARRAY_BUFFER, particles_texture_buffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, offset, particlesAmount * sizeof(GLfloat), textureData);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0, // attribute. No particular reason for 0, but must match the layout in the shader.
			3, // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1, // attribute. No particular reason for 1, but must match the layout in the shader.
			4, // size : x + y + z + size => 4
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2, // attribute. No particular reason for 1, but must match the layout in the shader.
			4, // size : r + g + b + a => 4
			GL_FLOAT, // type
			GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0, // stride
			(void*)0// array buffer offset
			);

		// 4th attribute buffer : particles' texture coordinates
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, particles_uv_buffer);
		glVertexAttribPointer(
			3, // attribute. No particular reason for 1, but must match the layout in the shader.
			4, // size : x + y + z + size => 4
			GL_FLOAT, // type
			GL_TRUE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, particles_texture_buffer);
		glVertexAttribPointer(
			4, // attribute. No particular reason for 1, but must match the layout in the shader.
			1, // size : x + y + z + size => 4
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		for (unsigned int i = 0; i < textureSlots.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureSlots[i]);
		}

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad -> 1
		glVertexAttribDivisor(3, 1); // uv : one per quad -> 1
		glVertexAttribDivisor(4, 1); // texture : one per quad -> 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, offset, 4, particlesAmount);

		textureSlots.clear();
	}

	void ParticleRenderer::init() {
		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particleGeometry), particleGeometry, GL_STATIC_DRAW);

		// The VBO containing the positions and sizes of the particles
		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles
		glGenBuffers(1, &particles_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the texture coordinates of the particles
		glGenBuffers(1, &particles_uv_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_uv_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		glGenBuffers(1, &particles_texture_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_texture_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		positionData = new GLfloat[MAX_PARTICLES * 4];
		colorData = new GLfloat[MAX_PARTICLES * 4];
		uvData = new GLfloat[MAX_PARTICLES * 4];
		textureData = new GLfloat[MAX_PARTICLES];
	}

}