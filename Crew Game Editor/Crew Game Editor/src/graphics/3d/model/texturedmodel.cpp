#include "texturedmodel.h"

namespace zakix {
	TexturedModel::TexturedModel() {}
	TexturedModel::TexturedModel(const RawModel& model, const ModelTexture& texture) {
		this->rawModel = model;
		this->texture = texture;
	}

	bool TexturedModel::operator<(const TexturedModel& other) const
	{
		if (rawModel.getVaoID() == other.getRawModel().getVaoID())
		{
			return texture.getID() < other.getTexture().getID();
		}

		return rawModel.getVaoID() < other.getRawModel().getVaoID();
	}
}