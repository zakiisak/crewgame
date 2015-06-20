#include "terraintexturepack.h"

namespace zakix {
	TerrainTexturePack::TerrainTexturePack() {}

	TerrainTexturePack::TerrainTexturePack(GLuint backgroundTexture, GLuint rTexture, GLuint gTexture, GLuint bTexture) {
		this->backgroundTexture = backgroundTexture;
		this->rTexture = rTexture;
		this->gTexture = gTexture;
		this->bTexture = bTexture;
	}

	const GLuint TerrainTexturePack::getBackgroundTexture() const {
		return backgroundTexture;
	}

	const GLuint TerrainTexturePack::getRTexture() const {
		return rTexture;
	}

	const GLuint TerrainTexturePack::getGTexture() const {
		return gTexture;
	}

	const GLuint TerrainTexturePack::getBTexture() const {
		return bTexture;
	}
}