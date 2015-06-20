#pragma once
#include <GL/glew.h>
#include <string>

namespace zakix {
	class Texture {
	private:
		const char* path;
		GLuint glID;
		unsigned int width, height;
	public:
		Texture();
		Texture(unsigned int id, unsigned int width, unsigned int height);
		void dispose();
		Texture(const char* path, GLint paramMin = GL_NEAREST, GLint paramMag = GL_NEAREST);
		void bind(GLuint texture = 0) const;
		void unbind() const;

		void save(const std::string& filename);

		inline const unsigned int getWidth() const { return width; }
		inline const unsigned int getHeight() const { return height; }
		inline const unsigned int getID() const { return glID; }
		void setMinMag(GLint paramMin, GLint paramMag);
		void setWrap(GLint paramWrapS, GLint paramWrapT);
		void setMipmapping(float mipLOD = -0.4f, GLint mipParamMin = GL_LINEAR_MIPMAP_LINEAR);
	private:

		GLuint load(GLint paramMin, GLint paramMag);
	};
}