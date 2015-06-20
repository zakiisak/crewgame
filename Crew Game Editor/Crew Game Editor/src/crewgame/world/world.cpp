#include "world.h"
#include "../crewgame.h"

namespace zakix {
	World::World(CrewGame* cg, unsigned int width, unsigned int height) {
		renderer = cg->renderer;
		this->width = width;
		this->height = height;
		this->windowWidth = cg->getWindow()->getWidth();
		this->windowHeight = cg->getWindow()->getHeight();

		tiles.resize(width * height);
		for (unsigned int i = 0; i < tiles.size(); i++) {
			unsigned int id = (rand() & 3) + 1;
			tiles[i] = new Tile(0);
			//memcpy(tiles[i], Tile::getTile(id), sizeof(Tile));
		}

	}
 
	Tile* World::getTile(unsigned int x, unsigned int y) {
		return tiles[x + y * width];
	}

	void World::setTile(unsigned int x, unsigned int y, unsigned int id) {
		memcpy(tiles[x + y * width], Tile::getTile(id), sizeof(Tile));
	}


	void World::update(CrewGame* cg) {
		
	}

	void World::render() {

		mat4 transformation(1.0f);
		transformation = translate(transformation, vec3(-camera.x, -camera.y, 0));

		int x1 = camera.x / Tile::TILE_SIZE;
		int y1 = camera.y / Tile::TILE_SIZE;
		int x2 = x1 + windowWidth / Tile::TILE_SIZE;
		int y2 = y1 + windowHeight / Tile::TILE_SIZE;

		renderer->begin(RENDER_2D_MODE);
		renderer->applyTransformation(transformation);
		for (int x = x1; x < x2 + 1; x++) {
			for (int y = y1; y < y2 + 1; y++) {
				if (x < 0 || x >= width || y < 0 || y >= height) continue;
				if (tiles[x + y * width]->getID() < 1) continue;
				tiles[x + y * width]->render(vec3(x * Tile::TILE_SIZE, y * Tile::TILE_SIZE, 0), renderer);
			}
		}
		renderer->end();
		renderer->flush();
	}

 }