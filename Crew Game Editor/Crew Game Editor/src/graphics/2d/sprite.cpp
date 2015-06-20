#include "sprite.h"

namespace zakix {
	Sprite::Sprite(float width, float height, unsigned int color)
		: Renderable2D(vec2(width, height), color)
	{

	}

	Sprite::Sprite(float width, float height, Texture* texture)
		: Renderable2D(vec2(width, height), 0xffffffff)
	{
		m_Texture = texture;
	}
}