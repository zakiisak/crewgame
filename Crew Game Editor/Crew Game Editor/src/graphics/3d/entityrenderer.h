#pragma once
#include <map>
#include "entity/entity.h"
#include <vector>
#include "../shader.h"
#include "light.h"

namespace zakix {
	typedef std::map<TexturedModel, std::vector<Entity>> EntityList; //unordered map later

	class EntityRenderer {
	private:
		EntityList entities;
		Shader* shader;
		Shader* outlineShader;
		GLint projectionLocation[2];
		GLint modelViewLocation[2];
		GLint transformationLocation[2];
		GLint fakeLightingLocation;
		GLint shineDamperLocation;
		GLint reflectivityLocation;
		GLint skyColorLocation[2];
		GLint lightPositionLocation[4];
		GLint lightColorLocation[4];
		GLint lightAttenuationLocation[4];
		bool outline;
	public:
		EntityRenderer();
		EntityRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation, bool outlineEnabled = false, const std::string& outlineVertexShader = "", const std::string& outlineFragmentShader = "");
		~EntityRenderer();
		void applyProjection(const mat4& projectionMatrix) const;
		void applyModelView(const mat4& modelViewMatrix, bool outline) const;
		void processEntity(const Entity& entity);
		void render(const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights);
		Shader* getShader() const;
		const bool hasOutlineEnabled() const;
	private:
		void prepareTexturedModel(const TexturedModel& model, bool outline) const;
		void prepareInstance(const Entity &entity, bool outline) const;
		void unbindTexturedModel() const;

		void loadSkyColor(const vec3& skyColor, bool outline);
		void loadLights(const std::vector<Light>& lights);
	};
}