#include "renderer.h"
#include "glFuncs.h"
#include "../utils/timer.h"

namespace zakix {

	Renderer::Renderer() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::createTerrainRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation) {
		terrainRenderer = new TerrainRenderer(vertexShader, fragmentShader, projectionLocation, modelViewLocation, transformationLocation);
	}
	void Renderer::renderTerrains(const std::vector<Terrain>& terrains, const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights) {
		terrainRenderer->render(terrains, modelViewMatrix, skyColor, lights);
	}

	void Renderer::createEntityRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation, bool outlineEnabled, const std::string& outlineVertexShader, const std::string& outlineFragmentShader) {
		entityRenderer = new EntityRenderer(vertexShader, fragmentShader, projectionLocation, modelViewLocation, transformationLocation, outlineEnabled, outlineVertexShader, outlineFragmentShader);
	}

	void Renderer::processEntity(const Entity& entity) {
		entityRenderer->processEntity(entity);
	}

	void Renderer::renderEntities(const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights) {
		entityRenderer->render(modelViewMatrix, skyColor, lights);
	}

	void Renderer::createParticleRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation) {
		particleRenderer = new ParticleRenderer(vertexShader, fragmentShader, projectionLocation, modelViewLocation);
	}

	void Renderer::renderParticles(const mat4& modelView, std::vector<Particle*>& particles) {
		particleRenderer->render(modelView, particles);
	}

	void Renderer::createCubeRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation) {
		cubeRenderer = new CubeRenderer(vertexShader, fragmentShader, projectionLocation, modelViewLocation);
	}

	void Renderer::processCube(const vec3& pos, const vec4& color, const vec4& uv) {
		cubeRenderer->processCube(pos, color, uv);
	}

	void Renderer::processCube(const vec3& pos, const vec4& color) {
		cubeRenderer->processCube(pos, color);
	}
	
	void Renderer::renderCubes(const mat4& modelView) {
		cubeRenderer->render(modelView);
	}

	void Renderer::createBatchRenderer2D(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation) {
		batchRenderer2D = new BatchRenderer2D(vertexShader, fragmentShader, projectionLocation, modelViewLocation, transformationLocation);
	}
	
	void Renderer::submit2D(const vec3& position, const Renderable2D* renderable) {
		batchRenderer2D->submit(position, renderable);
	}
	
	void Renderer::submitString(const std::string& str, const vec3& position, const Font& font, unsigned int color) {
		batchRenderer2D->submitString(str, position, font, color);
	}

	void Renderer::submitString3D(const std::string& str, const vec3& position, const Font& font, unsigned int color) {
		batchRenderer2D->submitString3D(str, position, font, color);
	}

	void Renderer::beginBatch2D(unsigned int renderMode) {
		batchRenderer2D->begin(renderMode);
	}

	void Renderer::endBatch2D() {
		batchRenderer2D->end();
		batchRenderer2D->flush();
	}

	void Renderer::addLight(const Light& light) {
		lights.push_back(light);
	}
	void Renderer::removeLight(unsigned int index) {
		assert(index < lights.size());

		if (index + 1 != lights.size())
			std::swap(lights[index], lights.back());

		lights.pop_back();
	}

	void Renderer::clearLights(const Light& light) {
		lights.clear();
	}

	Light& Renderer::getLight(unsigned int index) {
		return lights[index];
	}


	void Renderer::applyProjection3D(const mat4& projectionMatrix) {
		if (terrainRenderer != nullptr) {
			terrainRenderer->applyProjection(projectionMatrix);
		}
		if (entityRenderer != nullptr) {
			entityRenderer->applyProjection(projectionMatrix);
		}
		if (particleRenderer != nullptr) {
			particleRenderer->applyProjection(projectionMatrix);
		}
		if (cubeRenderer != nullptr) {
			cubeRenderer->applyProjection(projectionMatrix);
		}
	}

	void Renderer::applyProjection2D(const mat4& projectionMatrix) {
		if (batchRenderer2D != nullptr) {
			batchRenderer2D->applyProjection(projectionMatrix);
		}
	}

	void Renderer::render3D(const std::vector<Terrain>& terrains, const std::vector<Light>& lights, std::vector<Particle*>& particles, Camera* camera) {
		modelViewMatrix = camera->getModelViewMatrix();
		if (terrainRenderer != nullptr) {
			enableCulling();
			renderTerrains(terrains, modelViewMatrix, skyColor, lights);
		}
		if (entityRenderer != nullptr) {
			renderEntities(modelViewMatrix, skyColor, lights);
		}
		if (particleRenderer != nullptr) {
			disableCulling();
			renderParticles(modelViewMatrix, particles);
			enableCulling();
		}
		if (cubeRenderer != nullptr) {
			renderCubes(modelViewMatrix);
		}
	}

	TerrainRenderer* Renderer::getTerrainRenderer() const {
		return terrainRenderer;
	}

	EntityRenderer* Renderer::getEntityRenderer() const { 
		return entityRenderer; 
	}

	ParticleRenderer* Renderer::getParticleRenderer() const {
		return particleRenderer;
	}

	CubeRenderer* Renderer::getCubeRenderer() const {
		return cubeRenderer;
	}

	BatchRenderer2D* Renderer::getBatchRenderer2D() const {
		return batchRenderer2D;
	}


	const vec3& Renderer::getSkyColor() const {
		return skyColor;
	}

	void Renderer::setSkyColor(const vec3& skyColor) {
		this->skyColor = skyColor;
	}

	const mat4 Renderer::getModelViewMatrix() const {
		return modelViewMatrix;
	}

	Renderer::~Renderer() {
		dispose();
	}

	void Renderer::dispose() {
		if (!disposed) {
			if (terrainRenderer != nullptr) delete terrainRenderer;
			if(entityRenderer != nullptr) delete entityRenderer;
			if (particleRenderer != nullptr) delete particleRenderer;
			if (cubeRenderer != nullptr) delete cubeRenderer;
			if (batchRenderer2D != nullptr) delete batchRenderer2D;
			disposed = true;
		}
	}
}