#include "texture.h"
#include <FreeImage.h>
#include <iostream>

namespace zakix {
	Texture::Texture() {}

	Texture::Texture(unsigned int id, unsigned int width, unsigned int height) {
		this->glID = id;
		this->width = width;
		this->height = height;
	}

	void Texture::dispose() {
		glDeleteTextures(1, &glID);
	}

	GLuint Texture::load(GLint paramMin, GLint paramMag) {
		GLuint glTexture = 0;
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path);
		if (format == -1)
		{
			std::cerr << "Error loading image: " << path << " ! (image path or format is incompatible)" << std::endl;
			return glTexture;
		}
		FIBITMAP *bitmap = FreeImage_Load(format, path);
#ifdef ZAKIX_TEXTURE_FLIP_VERTICAL 
		FreeImage_FlipVertical(bitmap); 
#endif
		width = FreeImage_GetWidth(bitmap);
		height = FreeImage_GetHeight(bitmap);

		glGenTextures(1, &glTexture);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramMag);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(FreeImage_ConvertTo32Bits(bitmap)));

		glBindTexture(GL_TEXTURE_2D, 0);
		FreeImage_Unload(bitmap);

		return glTexture;
	}

	void Texture::setMinMag(GLint paramMin, GLint paramMag) {
		glBindTexture(GL_TEXTURE_2D, glID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramMag);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture::setWrap(GLint paramWrapS, GLint paramWrapT) {
		glBindTexture(GL_TEXTURE_2D, glID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, paramWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, paramWrapT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture::setMipmapping(float mipLOD, GLint mipParamMin) {
		glBindTexture(GL_TEXTURE_2D, glID);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipParamMin);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, mipLOD);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(const char* path, GLint paramMin, GLint paramMag) {
		this->path = path;
		glID = load(paramMin, paramMag);
	}

	void Texture::bind(GLuint texture) const {
		glActiveTexture(GL_TEXTURE0 + texture);
		glBindTexture(GL_TEXTURE_2D, glID);
	}

	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::save(const std::string& filename) {
		const int w = this->width;
		const int h = this->height;
		BYTE* pixels = new BYTE[w * h];
		glBindTexture(GL_TEXTURE_2D, glID);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB8, GL_UNSIGNED_BYTE, &pixels);
		glBindTexture(GL_TEXTURE_2D, 0);

		FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(pixels, w, h, 0, 24, 0xFF0000, 0x00FF00, 0x0000FF, FALSE);

		FreeImage_Save(FIF_PNG, bitmap, filename.c_str());		delete pixels;	}}