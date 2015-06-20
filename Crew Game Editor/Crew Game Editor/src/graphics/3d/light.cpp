#include "light.h"

namespace zakix {
	Light::Light() {}
	Light::Light(const vec3& position, const vec3& color, const vec3& attenuation) {
		this->position = position;
		this->color = color;
		this->attenuation = attenuation;
	}

	const vec3& Light::getPosition() const {
		return position;
	}

	const vec3& Light::getColor() const {
		return color;
	}

	const vec3& Light::getAttenuation() const {
		return attenuation;
	}

	void Light::setPosition(const vec3& position) {
		this->position = position;
	}
	
	void Light::setColor(const vec3& color) {
		this->color = color;
	}

	void Light::setAttenuation(const vec3& attenuation) {
		this->attenuation = attenuation;
	}
}