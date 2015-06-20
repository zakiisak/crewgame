#include <iostream>
#include "terrain.h"
#include "../../bitmap.h"
#include "../../../maths/maths.h"

namespace zakix {
	Terrain::Terrain() {}
	Terrain::Terrain(float x, float z, float size, float maxHeight, const TerrainTexturePack& texturePack, GLuint blendMapID, const char* heightMap, Loader3D* loader) {
		this->x = x;
		this->z = z;
		this->size = size;
		this->maxHeight = maxHeight;
		this->texturePack = texturePack;
		this->blendMap = blendMapID;
		model = generateTerrain(heightMap, loader);
	}

	const float Terrain::getX() const {
		return x;
	}

	const float Terrain::getZ() const {
		return z;
	}

	const RawModel& Terrain::getModel() const {
		return model;
	}

	const TerrainTexturePack& Terrain::getTexturePack() const {
		return texturePack;
	}

	const GLuint Terrain::getBlendMap() const {
		return blendMap;
	}


	float getHeight(int x, int y, const Bitmap& data, float maxHeight) {
		if (x >= data.getWidth() || y >= data.getHeight() || x < 0 || y < 0) {
			return 0;
		}
		float height = ((float*)data.pixels)[(x + y * data.getWidth())];
		height *= maxHeight;
		height *= 2.0f;
		height -= 1.0f;
		return height;
	}

	vec3 calculateNormal(int x, int y, Bitmap& data, float maxHeight) {
		float heightL = getHeight(x - 1, y, data, maxHeight);
		float heightR = getHeight(x + 1, y, data, maxHeight);
		float heightD = getHeight(x, y - 1, data, maxHeight);
		float heightU = getHeight(x, y + 1, data, maxHeight);
		glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
		normal = glm::normalize(normal);
		return normal;
	}

	const float Terrain::getTerrainHeight(float worldX, float worldZ) const {
		float terrainX = worldX - this->x;
		float terrainZ = worldZ - this->z;
		float gridSquareSize = size / (float)(vertexCount - 1);
		int gridX = (int)glm::floor(terrainX / gridSquareSize);
		int gridZ = (int)glm::floor(terrainZ / gridSquareSize);
		if (gridX >= vertexCount - 1 || gridZ >= vertexCount - 1 || gridX < 0 || gridZ < 0) {
			return 0;
		}
		float xCoord = ((int)terrainX % (int)gridSquareSize) / gridSquareSize;
		float zCoord = ((int)terrainZ % (int)gridSquareSize) / gridSquareSize;
		float answer;
		if (xCoord <= (1 - zCoord)) {
			answer = barryCentric(glm::vec3(0, heights[gridX + gridZ * vertexCount], 0),
				glm::vec3(1, heights[gridX + 1 + gridZ * vertexCount], 0),
				glm::vec3(0, heights[gridX + (gridZ + 1)/*<-- Maybe*/ * vertexCount], 1),
				glm::vec2(xCoord, zCoord));
		}
		else {
			answer = barryCentric(glm::vec3(1, heights[gridX + 1 + gridZ * vertexCount], 0),
				glm::vec3(1, heights[gridX + 1 + (gridZ + 1) * vertexCount], 1),
				glm::vec3(0, heights[gridX + (gridZ + 1) * vertexCount], 1),
				glm::vec2(xCoord, zCoord));
		}
		return answer;
	}

	RawModel Terrain::generateTerrain(const char* heightMap, Loader3D* loader) {
		Bitmap heightData(heightMap);
		//heightData.bitmap = FreeImage_ConvertToGreyscale(heightData.bitmap);
		//heightData.pixels = (void*)FreeImage_GetBits(heightData.bitmap);
		//heightData.bitmap = FreeImage_ConvertToFloat(heightData.bitmap);
		//heightData.pixels = (void*)FreeImage_GetBits(heightData.bitmap);
		int VERTEX_COUNT = heightData.getWidth();
		this->vertexCount = VERTEX_COUNT;
		std::cout << "heightmap data: width: " << heightData.getWidth() << ", height: " << heightData.getHeight() << std::endl;
		heights.resize(VERTEX_COUNT * VERTEX_COUNT);
		int count = VERTEX_COUNT * VERTEX_COUNT;
		std::vector<GLfloat> vertices; vertices.resize(count * 3);
		std::vector<GLfloat> normals; normals.resize(count * 3);
		std::vector<GLfloat> textureCoords; textureCoords.resize(count * 2);
		std::vector<GLuint> indices; indices.resize(6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT * 1));
		int vertexPointer = 0;
		for (int i = 0; i<VERTEX_COUNT; i++){
			for (int j = 0; j<VERTEX_COUNT; j++){
				vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * size;
				float height = getHeight(j, i, heightData, maxHeight);
				heights[j + i * VERTEX_COUNT] = height;
				vertices[vertexPointer * 3 + 1] = height;
				vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * size;
				glm::vec3 normal = calculateNormal(j, i, heightData, maxHeight);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;
				textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
				textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
				vertexPointer++;
			}
		}
		int pointer = 0;
		for (int gz = 0; gz<VERTEX_COUNT - 1; gz++){
			for (int gx = 0; gx<VERTEX_COUNT - 1; gx++){
				int topLeft = (gz*VERTEX_COUNT) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}
		return loader->loadToVAO(vertices, textureCoords, normals, indices);
	}
}