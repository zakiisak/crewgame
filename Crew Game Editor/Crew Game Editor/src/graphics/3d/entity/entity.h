#pragma once
#include "../model/texturedmodel.h"
#include "../../../maths/maths.h"

namespace zakix {
	class World3D;
	class Window;
	class Renderer;

	class Entity {
	protected:
		TexturedModel model;
		vec3 position;
		float rotX, rotY, rotZ;
		float scale;
		bool removed = false;
		bool renderable = true;
	public:
		Entity();
		Entity(const TexturedModel& model, const vec3& position, float rotX, float rotY, float rotZ, float scale);

		void move(const vec3& xyz);
		void move(float dx, float dy, float dz);
		void rotate(float dx, float dy, float dz);
		virtual void update(World3D* world, Window* window);
		virtual void render(World3D* world, Renderer* renderer);

		const TexturedModel& getModel() const { return model; }
		const vec3& getPosition() const { return position; }
		inline const float getRotX() const { return rotX; }
		inline const float getRotY() const { return rotY; }
		inline const float getRotZ() const { return rotZ; }
		inline const float getScale() const { return scale; }
		inline const bool isRemoved() const { return removed; }
		inline const bool isRenderable() const { return renderable; }
		void setPosition(const vec3& position);
		void setRotX(float rotX);
		void setRotY(float rotY);
		void setRotZ(float rotZ);
		void setScale(float scale);
		void remove();
		void setRenderable(bool renderable);
	};
}