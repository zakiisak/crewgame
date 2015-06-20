#pragma once
#include "rawmodel.h"
#include "modeltexture.h"

namespace zakix {
	class TexturedModel {
	private:
		RawModel rawModel;
		ModelTexture texture;
	public:
		TexturedModel();
		TexturedModel(const RawModel& model, const ModelTexture& texture);
		bool TexturedModel::operator<(const TexturedModel& other) const;

		inline const RawModel& getRawModel() const { return rawModel; }
		inline const ModelTexture& getTexture() const { return texture; }
	};
}