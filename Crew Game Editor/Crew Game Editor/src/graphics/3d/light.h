#pragma once
#include "../../maths/maths.h"

namespace zakix {
	class Light{
	private:
		vec3 position;
		vec3 color;
		vec3 attenuation = vec3(1, 0, 0);
	public:
		Light();
		Light(const vec3& position, const vec3& color, const vec3& attenuation = vec3(1, 0, 0));

		const vec3& getPosition() const;
		const vec3& getColor() const;
		const vec3& getAttenuation() const;
		void setPosition(const vec3& position);
		void setColor(const vec3& color);
		void setAttenuation(const vec3& attenuation);


	};
}