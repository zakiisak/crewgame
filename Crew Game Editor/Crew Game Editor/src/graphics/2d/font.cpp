#include <iostream>
#include "font.h"
	
int next_p2(int a)
{
	int rval = 1;
	while (rval<a) rval <<= 1;
	return rval;
}

namespace zakix {
	Font::Font() {}

	void Font::createCharacter(char ch) {

		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
			std::cout << "FreeType Glyph load failed!" << std::endl;

		FT_Glyph glyph;
		if (FT_Get_Glyph(face->glyph, &glyph))
			std::cout << "FreeType Glyph get failed!" << std::endl;

		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		int width = next_p2(bitmap.width);
		int height = next_p2(bitmap.rows);

		//Allocate memory for the texture data.
		GLubyte* expanded_data = new GLubyte[2 * width * height];

		//Here we fill in the data for the expanded bitmap.
		//Notice that we are using two channel bitmap (one for
		//luminocity and one for alpha), but we assign
		//both luminocity and alpha to the value that we
		//find in the FreeType bitmap. 
		//We use the ?: operator so that value which we use
		//will be 0 if we are in the padding zone, and whatever
		//is the the Freetype bitmap otherwise.
		for (int j = 0; j <height; j++) {
			for (int i = 0; i < width; i++){
				expanded_data[2 * (i + j*width)] = expanded_data[2 * (i + j*width) + 1] =
					(i >= bitmap.width || j >= bitmap.rows) ?
					0 : bitmap.buffer[i + bitmap.width*j];
			}
		}

		glBindTexture(GL_TEXTURE_2D, data.textures[ch]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

		delete[] expanded_data;

		float x = (float)bitmap.width / (float)width;
		float y = (float)bitmap.rows / (float)height;

		data.charData[ch].glyphLeft = bitmap_glyph->left;
		data.charData[ch].glyphBottom = bitmap_glyph->top - bitmap.rows;
		data.charData[ch].glyphTop = bitmap_glyph->top;
		data.charData[ch].texX = x;
		data.charData[ch].texY = y;
		data.charData[ch].vertexWidth = (float)bitmap.width;
		data.charData[ch].vertexHeight = (float)bitmap.rows;
		data.charData[ch].advanceX = (float)(face->glyph->advance.x >> 6);
		data.charData[ch].advanceY = (float)(face->glyph->advance.y >> 6);
	}

	void Font::createCharacters() {
		for (unsigned char character = 0; character < MAX_CHARACTERS; character++) {
			createCharacter(character);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Font::drawImmediateString(std::string str, float x, float y) {
		glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(180, 1, 0, 0);
		for (unsigned int i = 0; i < str.size(); i++) {
			char c = str[i];
			char_data cd = data.charData[c];
			glBindTexture(GL_TEXTURE_2D, data.textures[c]);
			glPushMatrix();
			glTranslatef(cd.glyphLeft, 0, 0);
			glTranslatef(0, cd.glyphBottom, 0);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(0, cd.vertexHeight);
			glTexCoord2d(0, cd.texY); glVertex2f(0, 0);
			glTexCoord2d(cd.texX, cd.texY); glVertex2f(cd.vertexWidth, 0);
			glTexCoord2d(cd.texX, 0); glVertex2f(cd.vertexWidth, cd.vertexHeight);
			glEnd();

			glPopMatrix();
			glTranslatef(cd.advanceX, 0, 0);
		}


		glPopMatrix();
		glPopAttrib();
	}

	Font::Font(std::string filename, unsigned int size)
		: m_Filename(filename), m_Size(size)
	{
		FT_Error error = FT_Init_FreeType(&library);
		if (error) {
			std::cout << "Could not initialize FreeType! [" << filename.c_str() << "]" << std::endl;
		}
		
		error = FT_New_Face(library, filename.c_str(), 0, &face);
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cout << "Font unknown file format! [" << filename.c_str() << "]" << std::endl;
		}
		else if (error) {
			std::cout << "Font could not be read! [" << filename.c_str() << "]" << std::endl;
		}

		FT_Set_Char_Size(face, size << 6, size << 6, 96, 96);
		glGenTextures(128, data.textures);
		createCharacters();

		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}

	void Font::setScale(float x, float y)
	{
		m_Scale = vec2(x, y);
	}

	float Font::getWidth(const std::string& text) const {
		float width = 0;
		for (unsigned int i = 0; i < text.size(); i++) {
			const char c = text[i];
			const char_data& cd = data.charData[c];
			//width += cd.glyphLeft;
			//width += cd.vertexWidth;
			width += cd.advanceX;
		}
		return width;
	}
}