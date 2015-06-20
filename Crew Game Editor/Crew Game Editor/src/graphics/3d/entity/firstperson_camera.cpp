#include "firstperson_camera.h"

namespace zakix {
	FirstPersonCamera::FirstPersonCamera() {}
	FirstPersonCamera::FirstPersonCamera(void (*controller) (Window* window, FirstPersonCamera* camera)) {
		this->controller = controller;
	}

	float springiness = 50; // tweak to taste.

	void smooth_mouse(double& mouseX, double& mouseY, float time_d, double realx, double realy, float centerX, float centerY) {
		double d = 1 - exp(log(0.5)*springiness*time_d);

		mouseX += (realx - mouseX) * d;
		mouseY += (realy - mouseY) * d;
	}

	void FirstPersonCamera::update(World3D* world, Window* window) {
		bool checkGrabbed = grabbed;
		if (grabbed) {
			double x, y;
			glfwGetCursorPos(window->getID(), &x, &y);
			float centerX = window->getWidth() / 2;
			float centerY = window->getHeight() / 2;
			//smooth_mouse(mouseX, mouseY, dt, x, y, centerX, centerY);
			double dx = x - centerX;
			double dy = y - centerY;

			rotX += (float)dy * sensitivity;
			rotY += (float)dx * sensitivity;

			glfwSetCursorPos(window->getID(), window->getWidth() / 2, window->getHeight() / 2);
		}
		if(controller != nullptr) controller(window, this);
		if (!checkGrabbed && grabbed) {
			double x, y;
			glfwGetCursorPos(window->getID(), &x, &y);
			mouseX = x;
			mouseY = y;
		}
	}

	void FirstPersonCamera::setGrabbed(bool grabbed) {
		this->grabbed = grabbed;
	}
}