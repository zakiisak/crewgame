#pragma once

#include "../shader.h"
#include "entity/particle.h"

namespace zakix {

#define MAX_PARTICLES 100000
#define PARTICLE_MAX_TEXTURES 32

	static const GLfloat particleGeometry[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	class ParticleRenderer {
	private:
		Shader* shader;
		GLint projectionLocation;
		GLint modelViewLocation;
		GLint cameraRightLocation;
		GLint cameraUpLocation;


		GLuint billboard_vertex_buffer;
		GLuint particles_position_buffer;
		GLuint particles_color_buffer;
		GLuint particles_uv_buffer;
		GLuint particles_texture_buffer;

		GLfloat *positionData;
		GLfloat *colorData;
		GLfloat *uvData;
		GLfloat *textureData;

		std::vector<GLuint> textureSlots;
	public:
		ParticleRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation);
		

		~ParticleRenderer() {
			glDeleteBuffers(1, &billboard_vertex_buffer);
			glDeleteBuffers(1, &particles_position_buffer);
			glDeleteBuffers(1, &particles_color_buffer);
			glDeleteBuffers(1, &particles_uv_buffer);
			glDeleteBuffers(1, &particles_texture_buffer);
			delete[] positionData;
			delete[] colorData;
			delete[] uvData;
			delete[] textureData;
		}

		void applyProjection(const mat4& projectionMatrix) const;
		void applyModelView(const mat4& modelViewMatrix) const;
		
		void render(const mat4& modelView, std::vector<Particle*>& particles);
	private:
		void flush(GLuint offset, GLuint particlesAmount);
		void init();
	};
}