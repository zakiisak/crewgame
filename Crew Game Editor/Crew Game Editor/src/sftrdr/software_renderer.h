#pragma once
#include <GL/glew.h>

namespace zakix {
	class SoftwareRenderer {
	private:
		unsigned int *framebuffer;
		unsigned int viewWidth, viewHeight;
		GLuint texture;
	public:
		SoftwareRenderer(const unsigned int& viewWidth, const unsigned int& viewHeight);
		~SoftwareRenderer();

		const unsigned int getViewWidth() const { return viewWidth; }
		const unsigned int getViewHeight() const { return viewHeight; }
		
		unsigned int *getFrameBuffer() const;
		void setFrameBuffer(unsigned int* framebuffer);
		
		void drawPixel(const int& x, const int& y, unsigned int col);
		void drawRect(int x, int y, int width, int height);
		void enable();
		void flush();
		void disable();

	};
}

