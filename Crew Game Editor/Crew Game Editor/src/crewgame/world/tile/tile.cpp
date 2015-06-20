#include "tile.h"
#include "../../../graphics/2d/batchrenderer2d.h"

namespace zakix {
	Tile* Tile::tiles[MAX_TILES];
	Texture* Tile::tileset;
	const float Tile::TILE_SIZE = 32.0f;

	void Tile::registerTile(Tile* tile) {
		tiles[tile->getID()] = tile;
	}

	Tile* Tile::getTile(unsigned int id) {
		return tiles[id];
	}

	Tile::Tile(unsigned int id) {
		this->id = id;
	}

	void Tile::render(const vec3& position, BatchRenderer2D* renderer) {
		renderer->submitTile(position, this);
	}


	Bitmap* Tile::getTexture() const {
		return texture;
	}

	void Tile::setTexture(Bitmap* texture) {
		this->texture = texture;
	}
}