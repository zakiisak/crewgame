#pragma once
#include <vector>

#include "../../maths/maths.h"
#include "../texture.h"

namespace zakix {
	class Renderable2D
	{
	protected:
		vec2 m_Size;
		unsigned int m_Color;
		std::vector<vec2> m_UV;
		Texture* m_Texture;
	protected:
		Renderable2D() : m_Texture(nullptr) { setUVDefaults(); }
	public:
		Renderable2D(vec2 size, unsigned int color)
			: m_Size(size), m_Color(color), m_Texture(nullptr)
		{
			setUVDefaults();
		}

		virtual ~Renderable2D() { }

		void setColor(unsigned int color) { m_Color = color; }
		void setColor(const vec4& color)
		{
			int r = color.x * 255.0f;
			int g = color.y * 255.0f;
			int b = color.z * 255.0f;
			int a = color.w * 255.0f;

			m_Color = a << 24 | b << 16 | g << 8 | r;
		}

		inline const vec2& getSize() const { return m_Size; }
		inline const unsigned int getColor() const { return m_Color; }
		inline const std::vector<vec2>& getUV() const { return m_UV; }
		void setUV(unsigned int index, const vec2& uv);
		void setUV(const vec4& uv);

		inline const GLuint getTID() const { return m_Texture ? m_Texture->getID() : 0; }
	private:
		void setUVDefaults()
		{
			m_UV.push_back(vec2(0, 0));
			m_UV.push_back(vec2(1, 0));
			m_UV.push_back(vec2(1, 1));
			m_UV.push_back(vec2(0, 1));
		}
	};
}