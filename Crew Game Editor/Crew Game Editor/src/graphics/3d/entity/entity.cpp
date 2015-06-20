#include "entity.h"
#include "../../renderer.h"

namespace zakix {
	Entity::Entity() {}
	Entity::Entity(const TexturedModel& model, const vec3& position, float rotX, float rotY, float rotZ, float scale) {
		this->model = model;
		this->position = position;
		this->rotX = rotX;
		this->rotY = rotY;
		this->rotZ = rotZ;
		this->scale = scale;
	}

	void Entity::move(const vec3& xyz) {
		position += xyz;
	}

	void Entity::move(float dx, float dy, float dz) {
		position.x += dx;
		position.y += dy;
		position.z += dz;
	}

	void Entity::rotate(float dx, float dy, float dz) {
		rotX += dx;
		rotY += dy;
		rotZ += dz;
	}

	void Entity::update(World3D *world, Window* window) {

	}
	void Entity::render(World3D *world, Renderer* renderer) {
		renderer->processEntity(*this);
	}

	void Entity::setPosition(const vec3& position) {
		this->position = position;
	}

	void Entity::setRotX(float rotX) {
		this->rotX = rotX;
	}

	void Entity::setRotY(float rotY) {
		this->rotY = rotY;
	}

	void Entity::setRotZ(float rotZ) {
		this->rotZ = rotZ;
	}

	void Entity::setScale(float scale) {
		this->scale = scale;
	}

	void Entity::remove() {
		removed = true;
	}

	void Entity::setRenderable(bool renderable) {
		this->renderable = renderable;
	}
}