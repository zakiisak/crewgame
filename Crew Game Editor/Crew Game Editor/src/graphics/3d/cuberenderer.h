#pragma once
#include "../shader.h"

namespace zakix {

#define MAX_CUBES 4000000

	static float cubeGeometry[] = {
		-0.5f, -0.5f, -0.5f, // triangle 1 : begin
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f, // triangle 1 : end
		0.5f, 0.5f, -0.5f, // triangle 2 : begin
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f, // triangle 2 : end
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f
	};

	class CubeRenderer {
	private:
		Shader* shader;
		GLint projectionLocation;
		GLint modelViewLocation;

		GLuint vertexBuffer;
		GLuint positionBuffer;
		GLuint colorBuffer;
		GLuint uvBuffer;

		std::vector<GLfloat> positionData;
		std::vector<GLfloat> colorData;
		std::vector<GLfloat> uvData;
		
		unsigned int idx = 0;
	public:
		CubeRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation);
		
		~CubeRenderer() {
			glDeleteBuffers(1, &vertexBuffer);
			glDeleteBuffers(1, &positionBuffer);
			glDeleteBuffers(1, &colorBuffer);
			glDeleteBuffers(1, &uvBuffer);
		}

		void applyProjection(const mat4& projectionMatrix) const;
		void applyModelView(const mat4& modelViewMatrix) const;

		void processCube(const vec3& pos, const vec4& color, const vec4& uv);
		void processCube(const vec3& pos, const vec4& color);
		void render(const mat4& modelViewMatrix);
	private:
		void init();
	};
}