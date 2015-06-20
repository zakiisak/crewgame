#pragma once
#include "engine/core.h"

namespace zakix {
	class Zakix : public Core {
	public:
		unsigned int fps;
		unsigned int ups;
		float delta = 0.0f;
		//Runs once per second
		virtual void tick() = 0;
		virtual void resized(unsigned int width, unsigned int height) = 0;

		virtual void create(WindowConfiguration& config) = 0;
		virtual void load() = 0;
		virtual void update(float delta) = 0;
		virtual void render() = 0;
		virtual void dispose() = 0;
	
	private:
		virtual void loop();
	};
}