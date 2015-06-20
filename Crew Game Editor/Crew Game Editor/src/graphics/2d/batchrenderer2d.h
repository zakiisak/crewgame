#pragma once

#include "renderable2d.h"
#include "../shader.h"

#include "../indexbuffer.h"
#include "font.h"
#include "../../crewgame/world/tile/tile.h"

namespace zakix {

	struct VertexData
	{
		vec3 vertex;
		vec2 uv = vec2(0, 0);
		float tid = 0.0f;
		unsigned int color;
	};

	struct TextData 
	{
		std::string str;
		vec3 position;
		Font* font;
		unsigned int color;
	};

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX		1
#define SHADER_TID_INDEX	2
#define SHADER_COLOR_INDEX	3

#define RENDER_2D_MODE   1
#define RENDER_FONT_MODE 2


	class BatchRenderer2D {
		Shader *shader;

		GLint projectionLocation;
		GLint modelViewLocation;
		GLint transformationLocation;
		GLint fontFlagLocation;

		unsigned int renderMode;
		GLuint m_VAO;
		GLuint m_VBO;
		IndexBuffer* m_IBO;
		GLsizei m_IndexCount;
		VertexData* m_Buffer;
		std::vector<GLuint> m_TextureSlots;
		std::vector<TextData> text;
	public:
		BatchRenderer2D(const std::string& vertexShader, const std::string& fragmentShader, const std::string& projectionLocation, const std::string& modelViewLocation, const std::string& transformationLocation);
		~BatchRenderer2D();

		void applyProjection(const mat4& projectionMatrix) const;
		void applyModelView(const mat4& modelViewMatrix) const;
		void applyTransformation(const mat4& transformationMatrix) const;
		
		void begin(unsigned int renderMode);
		void submit(const vec3& position, const Renderable2D* renderable);
		void submitTile(const vec3& position, Tile* tile);
		void submit(const vec2& position, const vec2& size, unsigned int color);
		void submitString(const std::string& str, const vec3& position, const Font& font, unsigned int color);
		void submitString3D(const std::string& str, const vec3& position, const Font& font, unsigned int color);
		void batchString(const std::string& str, const vec3& position, Font* font, unsigned int color);
		void end();
		void flush();
	private:
		void init();
	};
}