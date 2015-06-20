#pragma once
#include "tile/tile.h"
#include "tile/tiles.h"
#include <vector>

namespace zakix {
	class BatchRenderer2D;
	class CrewGame;

	class World {
	private:
		BatchRenderer2D* renderer;
		unsigned int width, height, windowWidth, windowHeight;
		std::vector<Tile*> tiles;

	public:
		vec2 camera;
		World(CrewGame* cg, unsigned int width, unsigned int height);
		Tile* getTile(unsigned int x, unsigned int y);
		void setTile(unsigned int x, unsigned int y, unsigned int id);

		void update(CrewGame* cg);
		void render();

		const inline unsigned int getWidth() const { return width; }
		const inline unsigned int getHeight() const { return height; }

	};
}