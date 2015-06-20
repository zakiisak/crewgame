#pragma once

#include "../../../maths/maths.h"
#include <GL/glew.h>

namespace zakix {
	class World3D; 
	class Renderer;

	class Particle {
	protected:
		vec3 position;
		float size = 1.0f;
		vec4 color = vec4(1, 1, 1, 1);
		vec4 uv = vec4(0, 0, 1, 1);
		GLuint texture = 0;
		vec3 velocity;
		float cameraDistance;

		bool removed = false;
	public:
		virtual void update(World3D* world);

		bool operator<(const Particle& that);

		const vec3& getPosition() const;
		const vec4& getColor() const;
		const vec4& getUV() const;
		inline const GLuint getTexture() const { return texture; }
		inline const float getSize() const { return size; }
		inline const float getDistanceToCamera() const { return cameraDistance; }
		inline const bool isRemoved() const { return removed; }
		void setPosition(const vec3& position);
		void setSize(float size);
		void setColor(const vec4& color);
		void setUV(const vec4& uv);
		void setTexture(GLuint texture);
		void setVelocity(const vec3& velocity);
		void setDistanceToCamera(float distance);
		virtual void remove();


	};
}