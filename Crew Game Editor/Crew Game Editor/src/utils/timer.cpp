#include "timer.h"
#include <GLFW/glfw3.h>

namespace zakix {
	void Timer::reset() {
		elapsed = glfwGetTime();
	}
	
	double Timer::getElapsedSeconds() const {
		return glfwGetTime() - elapsed;
	}

	double Timer::getElapsedMilli() const {
		return (glfwGetTime() - elapsed) * 1000.0;
	}

	double Timer::getElapsedMicro() const {
		return (glfwGetTime() - elapsed) * 1000000.0;
	}

	double Timer::getElapsedNano() const {
		return (glfwGetTime() - elapsed) * 1000000000.0;
	}

}