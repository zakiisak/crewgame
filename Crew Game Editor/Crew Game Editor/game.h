#pragma once
#include "src/zakix.h"

using namespace zakix;

class Game : public Core {
public:
	Game();
	virtual void create(WindowConfiguration& config);
	virtual void load();
	virtual void update();
	virtual void render();
	virtual void dispose();

private:
	virtual void loop();
};