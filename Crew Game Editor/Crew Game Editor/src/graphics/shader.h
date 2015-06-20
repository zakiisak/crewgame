#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "../maths/maths.h"
#include "../utils/fileutils.h"

namespace zakix {
	class Shader
	{
	private:
		const char* m_VertPath;
		const char* m_FragPath;
	public:
		GLuint m_ShaderID;
		Shader(const char* vertPath, const char* fragPath);
		~Shader();

		GLint getUniformLocation(const GLchar* name);
		void setUniform1f(GLint location, float value);
		void setUniform1fv(GLint location, float* value, int count);
		void setUniform1i(GLint location, int value);
		void setUniform1iv(GLint location, int* value, int count);
		void setUniform2f(GLint location, const vec2& vector);
		void setUniform3f(GLint location, const vec3& vector);
		void setUniform4f(GLint location, const vec4& vector);
		void setUniformMat4(GLint location, const mat4& matrix);

		void setUniform1f(const GLchar* name, float value);
		void setUniform1fv(const GLchar* name, float* value, int count);
		void setUniform1i(const GLchar* name, int value);
		void setUniform1iv(const GLchar* name, int* value, int count);
		void setUniform2f(const GLchar* name, const vec2& vector);
		void setUniform3f(const GLchar* name, const vec3& vector);
		void setUniform4f(const GLchar* name, const vec4& vector);
		void setUniformMat4(const GLchar* name, const mat4& matrix);

		void enable() const;
		void disable() const;
	private:
		GLuint load();
	};
}