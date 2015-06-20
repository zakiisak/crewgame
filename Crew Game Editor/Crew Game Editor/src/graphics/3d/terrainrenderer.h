#pragma once
#include "../shader.h"
#include "model/terrain.h"
#include "light.h"

namespace zakix {
	class TerrainRenderer {
	private:
		Shader* shader;
		GLint projectionLocation;
		GLint modelViewLocation;
		GLint transformationLocation;
		GLint shineDamperLocation;
		GLint reflectivityLocation;
		GLint skyColorLocation;
		GLint lightPositionLocation[4];
		GLint lightColorLocation[4];
		GLint lightAttenuationLocation[4];

		GLuint backgroundTextureLocation;
		GLuint rTextureLocation;
		GLuint gTextureLocation;
		GLuint bTextureLocation;
		GLuint blendMapLocation;
	public:
		TerrainRenderer();
		TerrainRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation);
		~TerrainRenderer();
		void applyProjection(const mat4& projectionMatrix) const;
		void applyModelView(const mat4& modelViewMatrix) const;
		void loadSkyColor(const vec3& skyColor);
		void loadLights(const std::vector<Light>& lights);
		Shader* getShader() const;
		void render(const std::vector<Terrain>& terrains, const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights);
	private:
		void bindTextures(const Terrain& terrain);
		void prepareTerrain(const Terrain& terrain);
		void applyTransformation(const Terrain& terrain);
		void unbindTerrain();
	};
}