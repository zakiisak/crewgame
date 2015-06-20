#include "camera.h"

namespace zakix {
	void Camera::update(World3D* world, Window* window) {}

	void Camera::setSensitivity(float sensitivity) {
		this->sensitivity = sensitivity;
	}
	mat4 Camera::getModelViewMatrix() {
		return createViewMatrix(position, rotX, rotY);
	}

}