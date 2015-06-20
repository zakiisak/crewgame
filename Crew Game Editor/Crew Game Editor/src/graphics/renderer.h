#pragma once
#include "3d/terrainrenderer.h"
#include "3d/entityrenderer.h"
#include "3d/particlerenderer.h"
#include "3d/cuberenderer.h"
#include "3d/entity/camera.h"

#include "2d/batchrenderer2d.h"

namespace zakix {
	class Renderer {
	private:
		TerrainRenderer* terrainRenderer;
		EntityRenderer* entityRenderer;
		ParticleRenderer* particleRenderer;
		CubeRenderer* cubeRenderer;
		BatchRenderer2D* batchRenderer2D;

		
		bool disposed = false;
		vec3 skyColor = vec3(0, 0.5f, 1.0f);
	
		std::vector<Light> lights;

		mat4 modelViewMatrix;
	public:
		Renderer();
		void createTerrainRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation);
		void renderTerrains(const std::vector<Terrain>& terrains, const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights);

		void createEntityRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation, bool outlineEnabled = false, const std::string& outlineVertexShader = "", const std::string& outlineFragmentShader = "");
		void processEntity(const Entity& entity);
		void renderEntities(const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights);

		void createParticleRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation);
		void renderParticles(const mat4& modelView, std::vector<Particle*>& particles);

		void createCubeRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation);
		void processCube(const vec3& pos, const vec4& color, const vec4& uv);
		void processCube(const vec3& pos, const vec4& color);
		void renderCubes(const mat4& modelView);
		
		void createBatchRenderer2D(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation);
		void submit2D(const vec3& position, const Renderable2D* renderable);
		void submitString(const std::string& str, const vec3& position, const Font& font, unsigned int color);
		void submitString3D(const std::string& str, const vec3& position, const Font& font, unsigned int color);
		void beginBatch2D(unsigned int renderMode);
		void endBatch2D();

		void addLight(const Light& light);
		void removeLight(unsigned int index);
		void clearLights(const Light& light);
		Light& getLight(unsigned int index);

		void applyProjection3D(const mat4& projectionMatrix);
		void applyProjection2D(const mat4& projectionMatrix);
		void render3D(const std::vector<Terrain>& terrains, const std::vector<Light>& lights, std::vector<Particle*>& particles, Camera* camera);
		void render2D();

		TerrainRenderer* getTerrainRenderer() const;
		EntityRenderer* getEntityRenderer() const;
		ParticleRenderer* getParticleRenderer() const;
		CubeRenderer* getCubeRenderer() const;
		BatchRenderer2D* getBatchRenderer2D() const;

		const vec3& getSkyColor() const;
		void setSkyColor(const vec3& skyColor);

		const mat4 getModelViewMatrix() const;

		~Renderer();
		void dispose();
	};
}

