#pragma once
#include "entity.h"
#include "../../window.h"

namespace zakix {
	class Camera : public Entity {
	protected:
		Entity* attached;
		float sensitivity = 0.062f;
	public:
		virtual void update(World3D* world, Window* window);
		inline const float getSensitivity() const { return sensitivity; }
		void setSensitivity(float sensitivity);
		mat4 getModelViewMatrix();
	};
}