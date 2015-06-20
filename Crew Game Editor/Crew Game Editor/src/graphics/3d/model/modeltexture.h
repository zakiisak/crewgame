#pragma once
#include <GL/glew.h>

namespace zakix {
	class ModelTexture {
	private:
		GLuint textureID;

		float shineDamper = 1;
		float reflectivity = 0;
		bool hasTrans = false;
		bool fakeLighting = false;

	public:
		ModelTexture();
		ModelTexture(const GLuint& textureID);

		inline const GLuint getID() const { return textureID; }
		inline const float getShineDamper() const { return shineDamper; }
		inline const float getReflectivity() const { return reflectivity; }
		inline const bool hasTransparency() const { return hasTrans; }
		inline const bool useFakeLighting() const { return fakeLighting; }
		void setShineDamper(float damper);
		void setReflectivity(float reflect);
		void setTransparency(bool trans);
		void setFakeLighting(bool fakeLight);
	};
}