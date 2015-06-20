#pragma once
#include <vector>
#include "rawmodel.h"
#include "terraintexturepack.h"
#include "../loader3d.h"

namespace zakix {
	class Terrain {
	private:
		float x;
		float z;
		float size;
		float maxHeight;
		int vertexCount;
		RawModel model;
		TerrainTexturePack texturePack;
		GLuint blendMap;
		std::vector<float> heights;
	public:
		Terrain();
		// *** Generates a terrain from a height and blend map. ***
		Terrain(float x, float z, float size, float maxHeight, const TerrainTexturePack& texturePack, GLuint blendMapID, const char* heightMap, Loader3D* loader);


		const float getX() const;
		const float getZ() const;
		const RawModel& getModel() const;
		const TerrainTexturePack& getTexturePack() const;
		const GLuint getBlendMap() const;
		const float getTerrainHeight(float x, float z) const;
		const inline int getSize() const;
	private:
		RawModel generateTerrain(const char* heightMap, Loader3D* loader);
	};
}