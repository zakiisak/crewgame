#include "tiles.h"
#include "../../crewgame.h"
#include "tile.h"
#include <GL/glew.h>

namespace zakix {
	void Tiles::createTileset() {
		int tilesetWidth = 0, tilesetHeight = 0;

		std::vector<Bitmap*> textures;
		std::vector<int> tilesRegistered;

		for (unsigned int i = 0; i < MAX_TILES; i++) {
			Tile* tile = Tile::getTile(i);
			if (tile == nullptr) continue;
			Bitmap* texture = tile->getTexture();
			if (texture == nullptr) continue;
			//Do all the texturing here :D
			unsigned int textureWidth = texture->getWidth();
			unsigned int textureHeight = texture->getHeight();
			tilesetWidth += textureWidth;
			if (textureHeight > tilesetHeight) tilesetHeight = textureHeight;
			textures.push_back(texture);
			tilesRegistered.push_back(i);
		}
		
		unsigned int* tilesetTexture = new unsigned int[tilesetWidth * tilesetHeight];

		unsigned int widthIndex = 0;

		for (unsigned int i = 0; i < textures.size(); i++) {
			//Add Tiles texture index:
			Tile* tile = Tile::getTile(tilesRegistered[i]);
			tile->tilesetOffset = (float)widthIndex / (float) tilesetWidth;
			Bitmap* texture = textures[i];
			tile->tilesetHeightIndex = (float)texture->getHeight() / (float)tilesetHeight;
			tile->tilesetWidthIndex = (float)texture->getWidth() / (float)tilesetWidth;

			unsigned int* pixels = (unsigned int*)texture->pixels;
			unsigned int width = texture->getWidth();
			unsigned int height = texture->getHeight();
			for (unsigned int x = 0; x < width; x++) {
				for (unsigned int y = 0; y < height; y++) {
					tilesetTexture[(x + widthIndex) + y * tilesetWidth] = pixels[x + (height - y - 1) * width];
				}
			}
			widthIndex += width;
		}

		unsigned int glID = 0;
		glGenTextures(1, &glID);
		glBindTexture(GL_TEXTURE_2D, glID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tilesetWidth, tilesetHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, tilesetTexture);

		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] tilesetTexture;

		Tile::tileset = new Texture(glID, tilesetWidth, tilesetHeight);
	}


	void Tiles::load(CrewGame* cg) {
		Tile* prototype1 = new Tile(1); prototype1->setTexture(new Bitmap("G:/Windows Library/Dropbox/Crew Game (1)/Textures/World/grass.png"));
		Tile::registerTile(prototype1);

		Tile* prototype2 = new Tile(2); prototype2->setTexture(new Bitmap("G:/Windows Library/Dropbox/Crew Game (1)/Textures/World/Placeholder2.png"));
		Tile::registerTile(prototype2);

		Tile* prototype3 = new Tile(3); prototype3->setTexture(new Bitmap("G:/Windows Library/Dropbox/Crew Game (1)/Textures/World/Placeholder3.png"));
		Tile::registerTile(prototype3);

		Tile* test = new Tile(4); test->setTexture(new Bitmap("res/Untitled.png"));
		Tile::registerTile(test);

		//Create spritesheet at the end here: Tile::tileset - you know what i mean...
		createTileset();
	}

	void Tiles::dispose(CrewGame* cg) {
		Tile::tileset->dispose();
		//delete[] Tile::tiles;
	}
}