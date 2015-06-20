#pragma once
#include "renderable2d.h"

namespace zakix {
	class Sprite : public Renderable2D
	{
	public:
		Sprite(float width, float height, unsigned int color);
		Sprite(float width, float height, Texture* texture);
	};
}
