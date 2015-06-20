#include "entityrenderer.h"
#include "../glFuncs.h"
#include "../../utils/util_funcs.h"

namespace zakix {

	EntityRenderer::EntityRenderer() {}

	EntityRenderer::EntityRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation, bool outlineEnabled, const std::string& outlineVertexShader, const std::string& outlineFragmentShader) {
		shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
		if(outlineEnabled) outlineShader = new Shader(outlineVertexShader.c_str(), outlineFragmentShader.c_str());
		this->outline = outlineEnabled;
		
		this->projectionLocation[0] = shader->getUniformLocation(projectionLocation.c_str());
		this->modelViewLocation[0] = shader->getUniformLocation(modelViewLocation.c_str());
		this->transformationLocation[0] = shader->getUniformLocation(transformationLocation.c_str());

		fakeLightingLocation = shader->getUniformLocation("useFakeLighting");
		shineDamperLocation = shader->getUniformLocation("shineDamper");
		reflectivityLocation = shader->getUniformLocation("reflectivity");
		
		skyColorLocation[0] = shader->getUniformLocation("skyColor");
		
		for (int i = 0; i < 4; i++) {
			lightPositionLocation[i] = shader->getUniformLocation(toString("lightPosition[%i]", i));
			lightColorLocation[i] = shader->getUniformLocation(toString("lightColor[%i]", i));
			lightAttenuationLocation[i] = shader->getUniformLocation(toString("lightAttenuation[%i]", i));
		}

		if (outlineEnabled) {
			this->projectionLocation[1] = outlineShader->getUniformLocation(projectionLocation.c_str());
			this->modelViewLocation[1] = outlineShader->getUniformLocation(modelViewLocation.c_str());
			this->transformationLocation[1] = outlineShader->getUniformLocation(transformationLocation.c_str());
			skyColorLocation[1] = outlineShader->getUniformLocation("skyColor");
		}
	}

	EntityRenderer::~EntityRenderer() {
		delete shader;
		delete outlineShader;
	}

	void EntityRenderer::applyProjection(const mat4& projectionMatrix) const {
		shader->enable();
		shader->setUniformMat4(projectionLocation[0], projectionMatrix);
		shader->disable();
		if (outline) {
			outlineShader->enable();
			outlineShader->setUniformMat4(projectionLocation[1], projectionMatrix);
			outlineShader->disable();
		}
	}

	void EntityRenderer::applyModelView(const mat4& modelViewMatrix, bool outline) const {
		if (outline) outlineShader->setUniformMat4(modelViewLocation[1], modelViewMatrix);
		else shader->setUniformMat4(modelViewLocation[0], modelViewMatrix);
	}

	void EntityRenderer::processEntity(const Entity& entity) {
		const TexturedModel& model = entity.getModel();
		EntityList::iterator iter = entities.find(model);
		if (iter != entities.end()) {
			entities[model].push_back(entity);
		}
		else {
			std::vector<Entity> newBatch;
			newBatch.push_back(entity);
			entities[model] = newBatch;
		}
	}

	void EntityRenderer::prepareTexturedModel(const TexturedModel& model, bool outline) const {
		RawModel rawModel = model.getRawModel();
		glBindVertexArray(rawModel.getVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		if (!outline) {
			ModelTexture texture = model.getTexture();
			if (texture.hasTransparency()) {
				disableCulling();
			}
			shader->setUniform1f(fakeLightingLocation, texture.useFakeLighting());
			shader->setUniform1f(shineDamperLocation, texture.getShineDamper());
			shader->setUniform1f(reflectivityLocation, texture.getReflectivity());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.getID());
		}
	}

	void EntityRenderer::prepareInstance(const Entity &entity, bool outline) const { 
		mat4 transformationMatrix = createTransformationMatrix(entity.getPosition(), entity.getRotX(), entity.getRotY(), entity.getRotZ(), entity.getScale());
		if (outline) outlineShader->setUniformMat4(transformationLocation[1], transformationMatrix);
		else shader->setUniformMat4(transformationLocation[0], transformationMatrix);
	}

	void EntityRenderer::unbindTexturedModel() const {
		enableCulling();
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void EntityRenderer::loadSkyColor(const vec3& skyColor, bool outline) {
		if (outline) outlineShader->setUniform3f(skyColorLocation[1], skyColor);
		else shader->setUniform3f(skyColorLocation[0], skyColor);
	}

	void EntityRenderer::loadLights(const std::vector<Light>& lights) {
		for (int i = 0; i < 4; i++) {
			if (i < lights.size()) {
				shader->setUniform3f(lightPositionLocation[i], lights[i].getPosition());
				shader->setUniform3f(lightColorLocation[i], lights[i].getColor());
				shader->setUniform3f(lightAttenuationLocation[i], lights[i].getAttenuation());
			}
			else {
				shader->setUniform3f(lightPositionLocation[i], vec3(0, 0, 0));
				shader->setUniform3f(lightColorLocation[i], vec3(0, 0, 0));
				shader->setUniform3f(lightAttenuationLocation[i], vec3(1, 0, 0));
			}
		}
	}

	void EntityRenderer::render(const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights) {
		if (outline) {
			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_STENCIL_TEST);

			glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}

		shader->enable();
		applyModelView(modelViewMatrix, false);
		loadSkyColor(skyColor, false);
		loadLights(lights);
		for (EntityList::iterator it = entities.begin(); it != entities.end(); ++it) {

			TexturedModel model = it->first;
			prepareTexturedModel(model, false);
			std::vector<Entity> batch = it->second;
			for (unsigned int i = 0; i < batch.size(); i++) {
				prepareInstance(batch[i], false);
				glDrawElements(GL_TRIANGLES, model.getRawModel().getVertexCount(), GL_UNSIGNED_INT, 0);
			}
			unbindTexturedModel();
		}
		shader->disable();

		if (outline) {
			outlineShader->enable();
			glStencilFunc(GL_NOTEQUAL, 1, 0xFFFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glLineWidth(3.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			applyModelView(modelViewMatrix, true);
			loadSkyColor(skyColor, true);
			for (EntityList::iterator it = entities.begin(); it != entities.end(); ++it) {

				TexturedModel model = it->first;
				prepareTexturedModel(model, true);
				std::vector<Entity> batch = it->second;
				for (unsigned int i = 0; i < batch.size(); i++) {
					prepareInstance(batch[i], true);
					glDrawElements(GL_TRIANGLES, model.getRawModel().getVertexCount(), GL_UNSIGNED_INT, 0);
				}
				unbindTexturedModel();
			}
			glDisable(GL_STENCIL_TEST);
			outlineShader->disable();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		entities.clear();
	}

	Shader* EntityRenderer::getShader() const { return shader; }

	const bool EntityRenderer::hasOutlineEnabled() const {
		return outline;
	}
}