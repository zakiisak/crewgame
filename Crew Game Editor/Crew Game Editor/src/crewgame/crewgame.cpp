#include "crewgame.h"
#include "ui/button.h"

namespace zakix {
	void CrewGame::create(WindowConfiguration& config) {
		config.x = 128;
		config.y = 128;
		config.title = "Crew Game";
		config.vsync = false;
		config.samples = 0;
		config.updateRatio = 60;
		config.width = 1024;
		config.height = 768;
		config.resizable = false;
	}

	void CrewGame::resized(unsigned int width, unsigned int height) {}

	Font* font;

	void CrewGame::load() {
		renderer = new BatchRenderer2D("res/shaders/batchrenderer2d/basic.vert", "res/shaders/batchrenderer2d/basic.frag", "pr_matrix", "vw_matrix", "ml_matrix");
		renderer->applyProjection(ortho(0.0f, (float)window->getWidth(), (float)window->getHeight(), 0.0f));
		disableCulling();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Initialize all tiles
		Tiles::load(this);
		//printf("id of tileset: %i\n", Tile::tileset->getID());
		Texture* texture = new Texture("D:/block_wall.png");
		sprite = new Sprite(Tile::tileset->getHeight(), Tile::tileset->getWidth(), Tile::tileset);
		sprite->setUV(vec4(1.0f, 1.0f, 0.0f, 0.0f));

		world = new World(this, 32, 32);
		panel = new Panel();
		Button* button = new Button(vec2(128, 128), vec2(256, 128), "iii\"\"iiiWWW");
		button->setFont(new Font("res/fonts/calibri.ttf", 16));
		panel->add(button);

		font = new Font("res/fonts/calibri.ttf", 24);
	}

	double lx, ly;
	float cx = 0;
	float cy = 0;
	float movementSpeed = 10.0f;
	unsigned int idLeft = 1;
	unsigned int idRight = 2;
	std::string str;

	void CrewGame::placeTile(unsigned int tx, unsigned int ty, unsigned int id, double mx, double my) {
		double xDiff = mx - lx;
		double yDiff = my - ly;
		
		world->setTile(tx, ty, id);
		
		double xDiff4 = xDiff / 16.0f;
		double yDiff4 = yDiff / 16.0f;

		for (int i = 1; i < 16.0f; i++) {
			double curDiffX = xDiff4 * i;
			double curDiffY = yDiff4 * i;

			float rx = lx + curDiffX;
			float ry = ly + curDiffY;

			float wx = cx + rx;
			float wy = cy + ry;
			if (wx > 0.0f && wy > 0.0f && wx < world->getWidth() * Tile::TILE_SIZE && wy < world->getHeight() * Tile::TILE_SIZE) {
				unsigned int tileX = wx / Tile::TILE_SIZE;
				unsigned int tileY = wy / Tile::TILE_SIZE;
				printf("tileX: %i, tileY: %i\n", tileX, tileY);
				world->setTile(tileX, tileY, id);
			}
		}
		/*
		//Iterate over the x-axis
		for (int x = 0; x < abs(xDiff) / 2; x++) {
			for (int y = 0; y < abs(yDiff) / 2; y++) {
				int dirX = xDiff > 0 ? 1 : -1;
				int dirY = yDiff > 0 ? 1 : -1;
				int rx = lx + (x * 2) * dirX;
				int ry = ly + (y * 2) * dirY;
				//printf("%i, %i\n", rx, ry);

				float wx = cx + rx;
				float wy = cy + ry;
				if (wx > 0.0f && wy > 0.0f && wx < world->getWidth() * Tile::TILE_SIZE && wy < world->getHeight() * Tile::TILE_SIZE) {
					unsigned int tileX = wx / Tile::TILE_SIZE;
					unsigned int tileY = wy / Tile::TILE_SIZE;
					printf("tileX: %i, tileY: %i\n", tileX, tileY);
					if (world->getTile(tileX, tileY)->getID() != id) world->setTile(tileX, tileY, id);
				}
			}
		}
		*/
	}

	void CrewGame::update(float delta) {
		double mx, my;
		window->getMousePosition(mx, my);

		//Control the camera
		if (glfwGetKey(window->getID(), GLFW_KEY_W)) {
			cy -= movementSpeed;
		}
		if (glfwGetKey(window->getID(), GLFW_KEY_S)) {
			cy += movementSpeed;
		}
		if (glfwGetKey(window->getID(), GLFW_KEY_A)) {
			cx -= movementSpeed;
		}
		if (glfwGetKey(window->getID(), GLFW_KEY_D)) {
			cx += movementSpeed;
		}

		if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			bool shiftPressed = glfwGetKey(window->getID(), GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window->getID(), GLFW_KEY_RIGHT_SHIFT);
			float wx = cx + mx;
			float wy = cy + my;
			str = toStringSTD("x: %f, y: %f", wx, wy);
			if (wx > 0.0f && wy > 0.0f && wx < world->getWidth() * Tile::TILE_SIZE && wy < world->getHeight() * Tile::TILE_SIZE) {
				unsigned int tx = wx / Tile::TILE_SIZE;
				unsigned int ty = wy / Tile::TILE_SIZE;
				if (shiftPressed) {
					unsigned int id = world->getTile(tx, ty)->getID();
					if (id != idLeft) idLeft = id;
				}
				else {
					if (world->getTile(tx, ty)->getID() != idLeft) placeTile(tx, ty, idLeft, mx, my);
				}
			}
		}

		if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
			bool shiftPressed = glfwGetKey(window->getID(), GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window->getID(), GLFW_KEY_RIGHT_SHIFT);

			float wx = cx + mx;
			float wy = cy + my;
			if (wx > 0.0f && wy > 0.0f && wx < world->getWidth() * Tile::TILE_SIZE && wy < world->getHeight() * Tile::TILE_SIZE) {
				unsigned int tx = wx / Tile::TILE_SIZE;
				unsigned int ty = wy / Tile::TILE_SIZE;
				unsigned int id = world->getTile(tx, ty)->getID();
				if (shiftPressed) {
					if (id != idRight) idRight = id;
				}
				else if (id != idLeft) idLeft = id;
			}
		}

		if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			bool shiftPressed = glfwGetKey(window->getID(), GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window->getID(), GLFW_KEY_RIGHT_SHIFT);
			float wx = cx + mx;
			float wy = cy + my;
			str = toStringSTD("x: %f, y: %f", wx, wy);
			if (wx > 0.0f && wy > 0.0f && wx < world->getWidth() * Tile::TILE_SIZE && wy < world->getHeight() * Tile::TILE_SIZE) {
				unsigned int tx = wx / Tile::TILE_SIZE;
				unsigned int ty = wy / Tile::TILE_SIZE;

				if (shiftPressed) {
					unsigned int id = world->getTile(tx, ty)->getID();
					if (id != idRight) idRight = id;
				}
				else {
					if (world->getTile(tx, ty)->getID() != idRight) placeTile(tx, ty, idRight, mx, my);
				}
			}
		}

		lx = mx;
		ly = my;

		world->camera.x = cx;
		world->camera.y = cy;

		if (window->isKeyPressed(GLFW_KEY_S)) {
			Tile::tileset->save("tileset.png");
			window->setKeyPressed(GLFW_KEY_S, false);
		}
		world->update(this);
		panel->update(this);
	}

	void CrewGame::tick() {
		glfwSetWindowTitle(window->getID(), (std::string(startConfig.title) + "    |    " + std::to_string(fps) + " fps    |    " + std::to_string(ups) + " ups" + "    |    size: " + std::to_string(window->getWidth()) + "x" + std::to_string(window->getHeight())).c_str());
	}

	void CrewGame::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0.5f, 1.0f, 1.0f);

		world->render();
		panel->render(renderer);

		renderer->batchString(str, vec3(128, 32, 0), font, 0xffffffff);

		renderer->begin(RENDER_FONT_MODE);
		renderer->end();
		renderer->flush();

	}

	void CrewGame::dispose() {
		Tiles::dispose(this);
		delete world;
		delete sprite;
		delete renderer;
	}
}