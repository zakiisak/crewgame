#pragma once
#include <GL/glew.h>

namespace zakix {
	class TerrainTexturePack {
	private:
		GLuint backgroundTexture;
		GLuint rTexture;
		GLuint gTexture;
		GLuint bTexture;
	public:
		TerrainTexturePack();
		TerrainTexturePack(GLuint backgroundTexture, GLuint rTexture, GLuint gTexture, GLuint bTexture);

		const GLuint getBackgroundTexture() const;
		const GLuint getRTexture() const;
		const GLuint getGTexture() const;
		const GLuint getBTexture() const;

	};
}