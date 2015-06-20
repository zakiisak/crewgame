#include "particle.h"

namespace zakix {

	bool Particle::operator<(const Particle& that) {
		return this->cameraDistance > that.cameraDistance;
	}

	const vec3& Particle::getPosition() const {
		return position;
	}

	const vec4& Particle::getColor() const {
		return color;
	}

	const vec4& Particle::getUV() const {
		return uv;
	}

	void Particle::update(World3D *world) {}

	void Particle::setPosition(const vec3& position) {
		this->position = position;
	}

	void Particle::setSize(float size) {
		this->size = size;
	}

	void Particle::setColor(const vec4& color) {
		this->color = color;
	}

	void Particle::setUV(const vec4& uv) {
		this->uv = uv;
	}

	void Particle::setTexture(GLuint texture) {
		this->texture = texture;
	}

	void Particle::setVelocity(const vec3& velocity) {
		this->velocity = velocity;
	}

	void Particle::setDistanceToCamera(float distance) {
		cameraDistance = distance;
	}

	void Particle::remove() {
		removed = true;
	}
}