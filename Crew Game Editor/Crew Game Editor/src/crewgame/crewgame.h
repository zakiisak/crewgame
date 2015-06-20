#pragma once

#include "../zakix.h"
#include "../graphics/2d/batchrenderer2d.h"
#include "world/world.h"
#include "../graphics/2d/sprite.h"
#include "../graphics/glFuncs.h"
#include "ui/panel.h"

namespace zakix {

	class CrewGame : public Zakix {
	private:
	public:
		BatchRenderer2D* renderer;
		World* world;
		Sprite* sprite;
		Panel* panel;

		int variables[500];

		void placeTile(unsigned int tx, unsigned int ty, unsigned int id, double mx, double my);

		virtual void create(WindowConfiguration& config);
		virtual void resized(unsigned int width, unsigned int height);
		virtual void load();
		virtual void update(float delta);
		virtual void tick();
		virtual void render();
		virtual void dispose();
	};
}