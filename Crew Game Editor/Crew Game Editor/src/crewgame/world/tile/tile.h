#pragma once
#include "../../../graphics/bitmap.h"
#include "../../../graphics/texture.h"
#include "../../../maths/maths.h"
#include "../../event/event.h"

namespace zakix {
#define MAX_TILES 256

	class BatchRenderer2D;

	class Tile {
	protected:
		unsigned int id;
		bool solid = false;
		bool alwaysOnTop = false;
		Bitmap* texture;
		unsigned int tint = 0xffffffff;

		Event* event;
	public:
		float tilesetOffset = 0;
		float tilesetWidthIndex;
		float tilesetHeightIndex;

		static Tile* tiles[MAX_TILES];
		static void registerTile(Tile* tile);
		static Tile* getTile(unsigned int id);
		static Texture* tileset;
		static const float TILE_SIZE;

		Tile(unsigned int id);

		void render(const vec3& position, BatchRenderer2D* renderer);

		//All different methods goes here :D

		const inline unsigned int getID() const { return id; }
		const inline bool isSolid() const { return solid; }
		const inline bool isAlwaysOnTop() const { return alwaysOnTop; }
		const inline unsigned int getTint() const { return tint; }
		Bitmap* getTexture() const;
		void setTexture(Bitmap* texture);

		Event* getEvent() const;
		void setEvent(const Event* event);
	};
}