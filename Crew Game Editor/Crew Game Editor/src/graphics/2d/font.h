#pragma once

#include "../../maths/maths.h"
#include <vector>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

namespace zakix {

#define MAX_CHARACTERS 128

	struct char_data {
		float glyphLeft;
		float glyphBottom;
		float glyphTop;
		float texX;
		float texY;
		float vertexWidth;
		float vertexHeight;
		float advanceX;
		float advanceY;
	};

	struct font_data {
		GLuint textures[MAX_CHARACTERS];
		char_data charData[MAX_CHARACTERS];
	};

	class Font {
	private:
		font_data data;
		FT_Library library;
		FT_Face face;
		unsigned int m_Size;
		vec2 m_Scale;
		std::string m_Name;
		std::string m_Filename;
	public:
		Font();
		Font(std::string filename, unsigned int size);

		void setScale(float x, float y);

		void drawImmediateString(std::string str, float x, float y);
		
		inline const vec2& getScale() const { return m_Scale; }
		inline const std::string& getName() const { return m_Name; }
		inline const std::string& getFileName() const { return m_Filename; }
		inline const int getSize() const { return m_Size; }
		inline const font_data& getData() const { return data; }

		float getWidth(const std::string& text) const;

	private:
		void createCharacters();
		void createCharacter(char ch);
	};
}