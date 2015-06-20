#include <iostream>
#include "bitmap.h"

namespace zakix {
	Bitmap::Bitmap() {}

	Bitmap::Bitmap(const char* path) {
		this->path = path;
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path);
		if (format == -1)
		{
			std::cerr << "Error loading image: " << path << " ! (image path or format is incompatible)" << std::endl;
			return;
		}
		bitmap = FreeImage_Load(format, path);
#ifdef ZAKIX_TEXTURE_FLIP_VERTICAL 
		FreeImage_FlipVertical(bitmap);
#endif
		width = FreeImage_GetWidth(bitmap);
		height = FreeImage_GetHeight(bitmap);
		pixels = FreeImage_GetBits(FreeImage_ConvertTo32Bits(bitmap));
	}

	Bitmap::Bitmap(const char* path, FREE_IMAGE_TYPE type) {
		this->path = path;
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path);
		if (format == -1)
		{
			std::cerr << "Error loading image: " << path << " ! (image path or format is incompatible)" << std::endl;
			return;
		}
		bitmap = FreeImage_Load(format, path);
#ifdef ZAKIX_TEXTURE_FLIP_VERTICAL 
		FreeImage_FlipVertical(bitmap);
#endif
		width = FreeImage_GetWidth(bitmap);
		height = FreeImage_GetHeight(bitmap);
		pixels = FreeImage_GetBits(FreeImage_ConvertToType(bitmap, type));
	}

	Bitmap::Bitmap(void* pixels, unsigned int width, unsigned int height) {
		this->pixels = pixels;
		this->width = width;
		this->height = height;
	}

	const unsigned int Bitmap::getWidth() const {
		return width;
	}

	const unsigned int Bitmap::getHeight() const {
		return height;
	}

	const char* Bitmap::getPath() const {
		return path;
	}

	FIBITMAP* Bitmap::getFreeImageBitmap() const {
		return bitmap;
	}
}