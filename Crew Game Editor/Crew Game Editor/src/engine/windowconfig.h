#pragma once
#include <GL/glew.h>

namespace zakix {
	struct WindowConfiguration {
		int x = -2;
		int y = -2;
		int width = 640;
		int height = 480;
		const char* title = "Zakix Engine";
		
		int resizable = GL_TRUE;
		int visible = GL_TRUE;
		int decorated = GL_TRUE;

		int redBits = 8;
		int greenBits = 8;
		int blueBits = 8;
		int alphaBits = 8;
		int depthBits = 24;
		int stencilBits = 8;
		int samples = 0;

		int refreshRate = 0;
		int stereo = GL_FALSE;

		int forwardCompatibility = GL_FALSE;
		int debug = GL_FALSE;
		bool vsync = true;

		float updateRatio = 60.0f;
	};
}