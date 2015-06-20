#include "terrainrenderer.h"
#include "../../utils/util_funcs.h"

namespace zakix {

	TerrainRenderer::TerrainRenderer() {}

	TerrainRenderer::TerrainRenderer(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation) {
		shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
		this->projectionLocation = shader->getUniformLocation(projectionLocation.c_str());
		this->modelViewLocation = shader->getUniformLocation(modelViewLocation.c_str());
		this->transformationLocation = shader->getUniformLocation(transformationLocation.c_str());
		shineDamperLocation = shader->getUniformLocation("shineDamper");
		reflectivityLocation = shader->getUniformLocation("reflectivity");
		skyColorLocation = shader->getUniformLocation("skyColor");
		backgroundTextureLocation = shader->getUniformLocation("backgroundTexture");
		rTextureLocation = shader->getUniformLocation("rTexture");
		gTextureLocation = shader->getUniformLocation("gTexture");
		bTextureLocation = shader->getUniformLocation("bTexture");
		blendMapLocation = shader->getUniformLocation("blendMap");
		for (int i = 0; i < 4; i++) {
			lightPositionLocation[i] = shader->getUniformLocation(toString("lightPosition[%i]", i));
			lightColorLocation[i] = shader->getUniformLocation(toString("lightColor[%i]", i));
			lightAttenuationLocation[i] = shader->getUniformLocation(toString("lightAttenuation[%i]", i));
		}
		shader->enable();
		shader->setUniform1i(backgroundTextureLocation, 0);
		shader->setUniform1i(rTextureLocation, 1);
		shader->setUniform1i(gTextureLocation, 2);
		shader->setUniform1i(bTextureLocation, 3);
		shader->setUniform1i(blendMapLocation, 4);
		shader->disable();
	}

	TerrainRenderer::~TerrainRenderer() {
		delete shader;
	}

	void TerrainRenderer::applyProjection(const mat4& projectionMatrix) const {
		shader->enable();
		shader->setUniformMat4(projectionLocation, projectionMatrix);
		shader->disable();
	}

	void TerrainRenderer::applyModelView(const mat4& modelViewMatrix) const {
		shader->setUniformMat4(modelViewLocation, modelViewMatrix);
	}

	void TerrainRenderer::loadSkyColor(const vec3& skyColor) {
		shader->setUniform3f(skyColorLocation, skyColor);
	}

	void TerrainRenderer::loadLights(const std::vector<Light>& lights) {
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

	void TerrainRenderer::bindTextures(const Terrain& terrain) {
		const TerrainTexturePack& texturePack = terrain.getTexturePack();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePack.getBackgroundTexture());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturePack.getRTexture());

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texturePack.getGTexture());

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texturePack.getBTexture());

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, terrain.getBlendMap());
	}

	void TerrainRenderer::prepareTerrain(const Terrain& terrain) {
		const RawModel& rawModel = terrain.getModel();
		glBindVertexArray(rawModel.getVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		shader->setUniform1f(shineDamperLocation, 1);
		shader->setUniform1f(reflectivityLocation, 0);
		bindTextures(terrain);
	}

	void TerrainRenderer::applyTransformation(const Terrain& terrain) {
		mat4 transformationMatrix = createTransformationMatrix(glm::vec3(terrain.getX(), 0, terrain.getZ()), 0, 0, 0, 1);
		shader->setUniformMat4(transformationLocation, transformationMatrix);
	}
	void TerrainRenderer::unbindTerrain() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void TerrainRenderer::render(const std::vector<Terrain>& terrains, const mat4& modelViewMatrix, const vec3& skyColor, const std::vector<Light>& lights) {
		shader->enable();
		applyModelView(modelViewMatrix);
		loadSkyColor(skyColor);
		loadLights(lights);
		for (unsigned int i = 0; i < terrains.size(); i++) {
			const Terrain& t = terrains[i];
			prepareTerrain(t);
			applyTransformation(t);
			glDrawElements(GL_TRIANGLES, t.getModel().getVertexCount(), GL_UNSIGNED_INT, 0);
			unbindTerrain();
		}
		shader->disable();
	}
}