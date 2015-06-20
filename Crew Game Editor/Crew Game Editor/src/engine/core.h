#pragma once
#include "../graphics/window.h"
#include "../audio/soundmanager.h"

namespace zakix {
	class Core {
	protected:
		Window* window;
		WindowConfiguration startConfig;
	public:
		Core();
		~Core();
		virtual void resized(unsigned int width, unsigned int height) = 0;
		virtual void create(WindowConfiguration& config) = 0;
		virtual void load() = 0;
		virtual void update(float delta) = 0;
		virtual void render() = 0;
		virtual void dispose() = 0;
		static void glfwError(int error, const char* description);

		Window* getWindow() const;

		bool start();
		void quit();

	protected:
		virtual void loop();
	};
}