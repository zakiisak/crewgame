#include "cuberenderer.h"
#include "../glFuncs.h"

namespace zakix {
	CubeRenderer::CubeRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation) {
		shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
		this->projectionLocation = shader->getUniformLocation(projectionLocation.c_str());
		this->modelViewLocation = shader->getUniformLocation(modelViewLocation.c_str());

		init();
	}

	void CubeRenderer::applyProjection(const mat4& projectionMatrix) const {
		shader->enable();
		shader->setUniformMat4(projectionLocation, projectionMatrix);
		shader->disable();
	}

	void CubeRenderer::applyModelView(const mat4& modelViewMatrix) const {
		shader->setUniformMat4(modelViewLocation, modelViewMatrix);
	}

	void CubeRenderer::processCube(const vec3& pos, const vec4& color, const vec4& uv) {
		positionData[3 * idx + 0] = pos.x;
		positionData[3 * idx + 1] = pos.y;
		positionData[3 * idx + 2] = pos.z;

		colorData[4 * idx + 0] = color.x;
		colorData[4 * idx + 1] = color.y;
		colorData[4 * idx + 2] = color.z;
		colorData[4 * idx + 3] = color.w;

		uvData[4 * idx + 0] = uv.x;
		uvData[4 * idx + 1] = uv.y;
		uvData[4 * idx + 2] = uv.z;
		uvData[4 * idx + 3] = uv.w;
		idx++;
	}

	void CubeRenderer::processCube(const vec3& pos, const vec4& color) {
		positionData[3 * idx + 0] = pos.x;
		positionData[3 * idx + 1] = pos.y;
		positionData[3 * idx + 2] = pos.z;

		colorData[4 * idx + 0] = color.x;
		colorData[4 * idx + 1] = color.y;
		colorData[4 * idx + 2] = color.z;
		colorData[4 * idx + 3] = color.w;
		idx++;
	}

	void CubeRenderer::render(const mat4& modelViewMatrix) {
		shader->enable();
		enableCulling();
		//glDisable(GL_CULL_FACE);
		applyModelView(modelViewMatrix);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, idx * sizeof(GLfloat)* 3, &positionData[0]);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, idx * sizeof(GLfloat)* 4, &colorData[0]);

		//glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		//glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		//glBufferSubData(GL_ARRAY_BUFFER, 0, idx * sizeof(GLfloat)* 4, &uvData[0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glVertexAttribPointer(
			1, // attribute. No particular reason for 1, but must match the layout in the shader.
			3, // size : x + y + z + size => 4
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(
			2, // attribute. No particular reason for 1, but must match the layout in the shader.
			4, // size : r + g + b + a => 4
			GL_FLOAT, // type
			GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0, // stride
			(void*)0// array buffer offset
			);

		/*// 4th attribute buffer : particles' texture coordinates
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(
			3, // attribute. No particular reason for 1, but must match the layout in the shader.
			4, // size : x + y + z + size => 4
			GL_FLOAT, // type
			GL_TRUE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);
			*/
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad -> 1
		//glVertexAttribDivisor(3, 1); // uv : one per quad -> 1

		//glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, cubeIndices, idx);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, idx);

		shader->disable();
		//idx = 0;
		//glEnable(GL_CULL_FACE);
	}

	void CubeRenderer::init() {
		
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeGeometry), cubeGeometry, GL_STATIC_DRAW);


		// The VBO containing the positions and sizes of the particles
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the texture coordinates of the particles
		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		positionData.resize(MAX_CUBES * 3);
		colorData.resize(MAX_CUBES * 4);
		uvData.resize(MAX_CUBES * 4);
	}
}