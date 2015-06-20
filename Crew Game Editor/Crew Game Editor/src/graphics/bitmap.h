#pragma once
#include <FreeImage.h>

namespace zakix {
	class Bitmap {
	private:
		const char* path;
		unsigned width, height;
	public:
		FIBITMAP *bitmap;
		void* pixels;
		Bitmap();
		Bitmap(const char* path);
		Bitmap(const char* path, FREE_IMAGE_TYPE type);
		Bitmap(void* pixels, unsigned int width, unsigned int height);

		const unsigned int getWidth() const;
		const unsigned int getHeight() const;
		const char* getPath() const;
		FIBITMAP* getFreeImageBitmap() const;
	};

}