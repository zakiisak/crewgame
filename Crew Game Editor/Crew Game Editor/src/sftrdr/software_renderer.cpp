#include "software_renderer.h"

namespace zakix {
	SoftwareRenderer::SoftwareRenderer(const unsigned int& viewWidth, const unsigned int& viewHeight) {
		this->viewWidth = viewWidth;
		this->viewHeight = viewHeight;
		unsigned int frameBufferSize = viewWidth * viewHeight;
		framebuffer = new unsigned int[frameBufferSize];
		for (unsigned int i = 0; i < frameBufferSize; i++) framebuffer[i] = 0;
		glGenTextures(1, &texture);
		enable();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		disable();
	}

	SoftwareRenderer::~SoftwareRenderer() {
		delete[] framebuffer;
	}

	unsigned int *SoftwareRenderer::getFrameBuffer() const {
		return framebuffer;
	}

	void SoftwareRenderer::setFrameBuffer(unsigned int* framebuffer) {
		this->framebuffer = framebuffer;
	}

	void SoftwareRenderer::drawPixel(const int& x, const int& y, unsigned int col) {
		framebuffer[x + y * viewWidth] = col;
	}

	void SoftwareRenderer::drawRect(int x, int y, int width, int height) {
		const int xw = x + width;
		const int yh = y + height;

		for (int xp = x; xp < xw; xp++) {
			for (int yp = y; yp < yh; yp++) {
				if (xp < 0 || xp >= viewWidth || yp < 0 || yp >= viewHeight) continue;
				framebuffer[xp + yp * viewWidth] = 0xffffffff;
			}
		}
	}

	void SoftwareRenderer::flush() {
		//glDrawPixels(viewWidth, viewHeight, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, framebuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewWidth, viewHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, framebuffer);
	}

	void SoftwareRenderer::enable() {
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void SoftwareRenderer::disable() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}