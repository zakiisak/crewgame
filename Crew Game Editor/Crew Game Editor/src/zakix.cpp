#include "zakix.h"

namespace zakix {
	void Zakix::loop() {
		float lt = 0.0f;

		double updateTimer = 0.0;
		double updateTick = 1.0f / (double) startConfig.updateRatio;
		double tickTimer = 0.0;

		unsigned int frames  = 0;
		unsigned int updates = 0;
		
		resized(window->getWidth(), window->getHeight());

		while (!window->closed()) {
			double bt = glfwGetTime();
			if (glfwGetTime() - updateTimer > updateTick) {
				update(delta);
				updates++;
				updateTimer += updateTick;
			}
			render();
			window->update();
			SoundManager::update();


			if (glfwGetTime() - tickTimer > 1.0f) {
				tick();
				tickTimer += 1.0;
				ups = updates;
				frames = 0;
				updates = 0;
			}
			double et = glfwGetTime();
			fps = (unsigned int)(1.0 / (et - bt));
			delta = (float)(1.0 / (double)fps);
		}
		dispose();
		SoundManager::clean();
	}
}