#include "batchrenderer2d.h"

namespace zakix {
	BatchRenderer2D::BatchRenderer2D(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation)
	{
		shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
		fontFlagLocation = shader->getUniformLocation("awesomeFont");
		this->projectionLocation = shader->getUniformLocation(projectionLocation.c_str());
		this->modelViewLocation = shader->getUniformLocation(modelViewLocation.c_str());
		this->transformationLocation = shader->getUniformLocation(transformationLocation.c_str());

		shader->enable();

		GLint texIDs[] =
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31
		};

		shader->setUniform1iv("textures", texIDs, 32);
		shader->disable();
		init();

	}

	BatchRenderer2D::~BatchRenderer2D()
	{
		delete m_IBO;
		glDeleteBuffers(1, &m_VBO);
	}

	void BatchRenderer2D::applyProjection(const mat4& projectionMatrix) const {
		shader->enable();
		shader->setUniformMat4(projectionLocation, projectionMatrix);
		shader->disable();
	}

	void BatchRenderer2D::applyModelView(const mat4& modelViewMatrix) const {
		shader->setUniformMat4(modelViewLocation, modelViewMatrix);
	}

	void BatchRenderer2D::applyTransformation(const mat4& transformationMatrix) const {
		shader->setUniformMat4(transformationLocation, transformationMatrix);
	}

	void BatchRenderer2D::init()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glEnableVertexAttribArray(SHADER_TID_INDEX);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);

		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, uv)));
		glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, tid)));
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, color)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

		glBindVertexArray(0);

	}

	void BatchRenderer2D::begin(unsigned int renderMode)
	{
		this->renderMode = renderMode;
		shader->enable();
		if (renderMode == RENDER_FONT_MODE) shader->setUniform1f(fontFlagLocation, 1.0f);
		else shader->setUniform1f(fontFlagLocation, 0.0f);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void BatchRenderer2D::submit(const vec3& position, const Renderable2D* renderable)
	{
		const vec2& size = renderable->getSize();
		const unsigned int color = renderable->getColor();
		const std::vector<vec2>& uv = renderable->getUV();
		const GLuint tid = renderable->getTID();

		float ts = 0.0f;
		if (tid > 0)
		{
			bool found = false;
			for (unsigned int i = 0; i < m_TextureSlots.size(); i++)
			{
				if (m_TextureSlots[i] == tid)
				{
					ts = (float)(i + 1);
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (m_TextureSlots.size() >= RENDERER_MAX_TEXTURES)
				{
					end();
					flush();
					begin(renderMode);
				}
				m_TextureSlots.push_back(tid);
				ts = (float)(m_TextureSlots.size());
			}
		}

		m_Buffer->vertex = position;
		m_Buffer->uv = uv[0];
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_Buffer->vertex = vec3(position.x, position.y + size.y, position.z);
		m_Buffer->uv = uv[1];
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_Buffer->vertex = vec3(position.x + size.x, position.y + size.y, position.z);
		m_Buffer->uv = uv[2];
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_Buffer->vertex = vec3(position.x + size.x, position.y, position.z);
		m_Buffer->uv = uv[3];
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::submitTile(const vec3& position, Tile* tile)
	{
		const unsigned int color = tile->getTint();
		const GLuint tid = Tile::tileset->getID();
		const float size = Tile::TILE_SIZE;

		const float x1 = tile->tilesetOffset;
		const float x2 = x1 + tile->tilesetWidthIndex;
		const float y2 = tile->tilesetHeightIndex;


		float ts = 0.0f;
		if (tid > 0)
		{
			bool found = false;
			for (unsigned int i = 0; i < m_TextureSlots.size(); i++)
			{
				if (m_TextureSlots[i] == tid)
				{
					ts = (float)(i + 1);
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (m_TextureSlots.size() >= RENDERER_MAX_TEXTURES)
				{
					end();
					flush();
					begin(renderMode);
				}
				m_TextureSlots.push_back(tid);
				ts = (float)(m_TextureSlots.size());
			}
		}


		m_Buffer->vertex = position;
		m_Buffer->uv = vec2(x1, 0);
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_Buffer->vertex = vec3(position.x + size, position.y, position.z);
		m_Buffer->uv = vec2(x2, 0);
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_Buffer->vertex = vec3(position.x + size, position.y + size, position.z);
		m_Buffer->uv = vec2(x2, y2);
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_Buffer->vertex = vec3(position.x, position.y + size, position.z);
		m_Buffer->uv = vec2(x1, y2);
		m_Buffer->tid = ts;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::submit(const vec2& position, const vec2& size, unsigned int color) {
		float x = position.x;
		float y = position.y;
		float width = size.x;
		float height = size.y;

		m_Buffer->vertex = vec3(x, y, 0);
		m_Buffer->color = color;
		m_Buffer->tid = 0.0f;
		m_Buffer++;

		m_Buffer->vertex = vec3(x + width, y, 0);
		m_Buffer->color = color;
		m_Buffer->tid = 0.0f;
		m_Buffer++;

		m_Buffer->vertex = vec3(x + width, y + height, 0);
		m_Buffer->color = color;
		m_Buffer->tid = 0.0f;
		m_Buffer++;

		m_Buffer->vertex = vec3(x, y + height, 0);
		m_Buffer->color = color;
		m_Buffer->tid = 0.0f;
		m_Buffer++;

		m_IndexCount += 6;
	}


	void BatchRenderer2D::batchString(const std::string& str, const vec3& position, Font* font, unsigned int color) {
		TextData data;
		data.str = str;
		data.position = position;
		data.font = font;
		data.color = color;
		text.push_back(data);
	}


	void BatchRenderer2D::submitString(const std::string& str, const vec3& position, const Font& font, unsigned int color) {
		const font_data& data = font.getData();
		float x = position.x;
		float y = position.y;
		float z = position.z;
		float lineSpacing = 1.4f;
		for (unsigned int i = 0; i < str.length(); i++) {

			char c = str[i];

			if (c == '\n') {
				x = position.x;
				y += font.getSize() * lineSpacing;
				continue;
			}

			char_data cd = data.charData[c];
			GLuint tid = data.textures[c];

			float ts = 0.0f;
			bool found = false;
			for (unsigned int i = 0; i < m_TextureSlots.size(); i++)
			{
				if (m_TextureSlots[i] == tid)
				{
					ts = (float)(i + 1);
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (m_TextureSlots.size() >= 32)
				{
					end();
					flush();
					begin(renderMode);
				}
				m_TextureSlots.push_back(tid);
				ts = (float)(m_TextureSlots.size());
			}

			float x0 = x + cd.glyphLeft;
			float y0 = y - (float) cd.glyphTop;
			float x1 = x0 + cd.vertexWidth;
			float y1 = y + (cd.vertexHeight - cd.glyphTop);
			float u1 = cd.texX;
			float v1 = cd.texY;


			m_Buffer->vertex = vec3(x0, y0, z);
			m_Buffer->uv = vec2(0, 0);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = vec3(x0, y1, z);
			m_Buffer->uv = vec2(0, v1);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = vec3(x1, y1, z);
			m_Buffer->uv = vec2(u1, v1);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = vec3(x1, y0, z);
			m_Buffer->uv = vec2(u1, 0);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_IndexCount += 6;

			x += cd.advanceX;


		}
	}


	void BatchRenderer2D::submitString3D(const std::string& str, const vec3& position, const Font& font, unsigned int color) {

		const font_data& data = font.getData();
		float x = position.x;
		float y = position.y;
		float z = position.z;
		float lineSpacing = 1.5f;
		for (unsigned int i = 0; i < str.length(); i++) {

			char c = str[i];

			if (c == '\n') {
				x = position.x;
				y -= lineSpacing / (1.0f + lineSpacing / font.getSize());
				continue;
			}

			char_data cd = data.charData[c];
			GLuint tid = data.textures[c];

			float ts = 0.0f;
			bool found = false;
			for (unsigned int i = 0; i < m_TextureSlots.size(); i++)
			{
				if (m_TextureSlots[i] == tid)
				{
					ts = (float)(i + 1);
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (m_TextureSlots.size() >= 32)
				{
					end();
					flush();
					begin(renderMode);
				}
				m_TextureSlots.push_back(tid);
				ts = (float)(m_TextureSlots.size());
			}

			float x0 = x + cd.glyphLeft / font.getSize();
			float y0 = y + (float)(cd.glyphTop) / font.getSize();
			float x1 = x0 + cd.vertexWidth / font.getSize();
			float y1 = y0 - (cd.vertexHeight / font.getSize());
			float u1 = cd.texX;
			float v1 = cd.texY;


			m_Buffer->vertex = vec3(x0, y0, z);
			m_Buffer->uv = vec2(0, 0);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = vec3(x0, y1, z);
			m_Buffer->uv = vec2(0, v1);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = vec3(x1, y1, z);
			m_Buffer->uv = vec2(u1, v1);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = vec3(x1, y0, z);
			m_Buffer->uv = vec2(u1, 0);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_IndexCount += 6;

			x += cd.advanceX / font.getSize();


		}
	}

	void BatchRenderer2D::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BatchRenderer2D::flush()
	{
		if (renderMode == RENDER_FONT_MODE) {
			for (unsigned int i = 0; i < text.size(); i++) {
				const TextData& data = text[i];
				submitString(data.str, data.position, *data.font, data.color);
			}
			text.clear();
		}

		for (unsigned int i = 0; i < m_TextureSlots.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
		}

		glBindVertexArray(m_VAO);
		m_IBO->bind();

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

		m_IBO->unbind();
		glBindVertexArray(0);

		m_IndexCount = 0;
		m_TextureSlots.clear();
		shader->disable();
	}
}