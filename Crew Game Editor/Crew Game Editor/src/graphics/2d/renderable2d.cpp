#include "renderable2d.h"

namespace zakix {
	void Renderable2D::setUV(unsigned int index, const vec2& uv) {
		m_UV[index] = uv;
	}

	void Renderable2D::setUV(const vec4& uv) {
		m_UV[0] = vec2(uv.x, uv.y);
		m_UV[1] = vec2(uv.z, uv.y);
		m_UV[2] = vec2(uv.z, uv.w);
		m_UV[3] = vec2(uv.x, uv.w);
	}
}