#include "modeltexture.h"

namespace zakix {
	ModelTexture::ModelTexture() {}
	ModelTexture::ModelTexture(const GLuint& textureID) {
		this->textureID = textureID;
	}

	void ModelTexture::setShineDamper(float damper) {
		shineDamper = damper;
	}

	void ModelTexture::setReflectivity(float reflect) {
		reflectivity = reflect;
	}

	void ModelTexture::setTransparency(bool trans) {
		hasTrans = trans;
	}

	void ModelTexture::setFakeLighting(bool useFake) {
		fakeLighting = useFake;
	}
}