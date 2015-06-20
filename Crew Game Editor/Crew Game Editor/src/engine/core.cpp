#include "core.h"

namespace zakix {
	Core::Core() {}
	Core::~Core() {
		quit();
	}

	bool Core::start() {
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW!" << std::endl;
			return false;
		}
		glfwSetErrorCallback(Core::glfwError);
		create(startConfig);
		window = new Window(this, startConfig);
		SoundManager::init();
		load();
		loop();
		return 0;
	}

	void Core::loop() {
		float frames = 0.0f;
		float dt = 0.0f;
		while (!window->closed()) {
			double bt = glfwGetTime();
			update(dt);
			render();
			window->update();
			double et = glfwGetTime();
			frames = (float) (1.0 / (et - bt));
			dt = 1.0f / frames;
		}
	}

	void Core::glfwError(int error, const char* description) {
		std::cout << description << ", errorcode: " << error << std::endl;
	}

	Window* Core::getWindow() const {
		return window;
	}

	void Core::quit() {
		delete window;
		glfwTerminate();
	}
}