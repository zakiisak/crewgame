#if 0
#include "game.h"
#include "src/maths/maths.h"
#include "src/graphics/shader.h"
#include "src/graphics/texture.h"
#include "src/graphics/2d/sprite.h"
#include "src/graphics/2d/batchrenderer2d.h"
#include "src/graphics/renderer.h"
#include "src/graphics/2d/font.h"
#include "src/graphics/3d/loader3d.h"
#include "src/graphics/3d/entity/firstperson_camera.h"
#include "src/graphics/glFuncs.h"
#include "src/utils/util_funcs.h"

Game::Game() {}

void Game::create(WindowConfiguration& config) { config.samples = 16; config.vsync = true; }
void Game::load() {}

void updateCamera(Window* window, FirstPersonCamera* camera, float dt) {
	float ms = 10.0f * dt;
	float rad = radians(camera->getRotY());
	if (glfwGetKey(window->getID(), GLFW_KEY_W)) {
		float x = sin(rad);
		float z = -cos(rad);
		camera->move(x * ms, 0, z * ms);
	}
	else if (glfwGetKey(window->getID(), GLFW_KEY_S)) {
		float x = -sin(rad);
		float z = cos(rad);
		camera->move(x * ms, 0, z * ms);
	}
	if (glfwGetKey(window->getID(), GLFW_KEY_A)) {
		float x = -cos(rad);
		float z = -sin(rad);
		camera->move(x * ms, 0, z * ms);
	}
	else if (glfwGetKey(window->getID(), GLFW_KEY_D)) {
		float x = cos(rad);
		float z = sin(rad);
		camera->move(x * ms, 0, z * ms);
	}
	if (glfwGetKey(window->getID(), GLFW_KEY_SPACE)) {
		camera->move(0, ms, 0);
	}
	else if (glfwGetKey(window->getID(), GLFW_KEY_LEFT_SHIFT)) {
		camera->move(0, -ms, 0);
	}

	if (window->isMouseButtonClicked(GLFW_MOUSE_BUTTON_LEFT)) {
		glfwSetInputMode(window->getID(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		camera->setGrabbed(true);
		glfwSetCursorPos(window->getID(), window->getWidth() / 2, window->getHeight() / 2);
	}
	else if (window->isKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetInputMode(window->getID(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		camera->setGrabbed(false);
	}
	if (camera->getRotX() > 90)
		camera->setRotX(90);
	else if (camera->getRotX() < -90)
		camera->setRotX(-90);
}

void Game::loop() {
	//texture.bind();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Texture *texture = new Texture("res/stone.png");
	Texture *texture2 = new Texture("D:/block_wall.png");
	Texture texture3("res/ParticleAtlas.png", GL_LINEAR, GL_LINEAR);texture3.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);texture3.setMipmapping();

	Sprite *sprite = new Sprite(32, 32, texture);
	Sprite *sprite2 = new Sprite(16, 16, texture2);
	
	Font font("Fonts/monaco.ttf", 16);
	Renderer renderer;
	Loader3D loader;
	renderer.createTerrainRenderer("res/shaders/terrain/vertexShader.txt", "res/shaders/terrain/fragmentShader.txt", "projectionMatrix", "viewMatrix", "transformationMatrix");
	renderer.createEntityRenderer("res/shaders/entity/vertexShader.txt", "res/shaders/entity/fragmentShader.txt", "projectionMatrix", "viewMatrix", "transformationMatrix", true, "res/shaders/entity/olVertexShader.txt", "res/shaders/entity/olFragmentShader.txt");
	renderer.createParticleRenderer("res/shaders/particle/vertexShader.vs", "res/shaders/particle/fragmentShader.fs", "projectionMatrix", "viewMatrix");
	renderer.createBatchRenderer2D("res/shaders/batchrenderer2d/basic.vert", "res/shaders/batchrenderer2d/basic.frag", "pr_matrix", "vw_matrix", "ml_matrix");

	RawModel stallRawModel = loader.loadModel("res/models/ball.obj", true);
	std::cout << "stall model created!" << std::endl;
	ModelTexture stallTexture(texture->getID());
	TexturedModel stallModel(stallRawModel, stallTexture);
	Entity entity(stallModel, vec3(160, 100, 160), 0, 0, 0, 1);
	std::cout << "stall entity created!" << std::endl;

	RawModel randomModel = loader.loadModel("res/models/cube.obj");
	Entity randomEntity(TexturedModel(randomModel, stallTexture), vec3(0, 0, -12), 0, 0, 0, 1.0f);

	FirstPersonCamera *camera = new FirstPersonCamera(updateCamera);

	std::vector<Light> lights;
	lights.push_back(Light(camera->getPosition(), vec3(5, 2.5f, 0), vec3(0.4f, 0.4f, 0.4f)));
	lights.push_back(Light(entity.getPosition(), vec3(2000, 1500, 1500), vec3(0.0f, 0.04f, 0.02f)));


	std::vector<Particle> particles;
	Particle particle;
	particle.setPosition(vec3(0, 1, -4));
	particle.setSize(0.5f);
	particle.setTexture(0);
	particle.setColor(vec4(1, 0, 1, 0.5f));
	particle.setUV(vec4(0.25f, 0.25f, 0.3f, 0.3f));
	particles.push_back(particle);

	Particle particle2;
	particle2.setPosition(vec3(0, 1, -5));
	particle2.setSize(0.5f);
	particle2.setTexture(texture2->getID());
	particle2.setColor(vec4(1, 1, 1, 1));
	particle2.setUV(vec4(0.25f, 0.25f, 0.3f, 0.3f));
	particles.push_back(particle2);

	Particle particle3;
	particle3.setPosition(vec3(0, 1, -6));
	particle3.setSize(0.5f);
	particle3.setTexture(1);
	particle3.setColor(vec4(1, 1, 1, 1));
	particle3.setUV(vec4(0.25f, 0.25f, 0.3f, 0.3f));
	particles.push_back(particle3);

	Particle particle4;
	particle4.setPosition(vec3(0, 1, -2));
	particle4.setSize(1.0f);
	particle4.setTexture(texture3.getID());
	particle4.setColor(vec4(1, 1, 1, 1));
	particle4.setUV(vec4(0, 0, 1, 1));
	particles.push_back(particle4);

	Texture *backgroundTexture = new Texture("res/terrain/grassy2.png", GL_LINEAR, GL_LINEAR); backgroundTexture->setMipmapping();
	Texture *rTexture = new Texture("res/terrain/mud.png", GL_LINEAR, GL_LINEAR); rTexture->setMipmapping();
	Texture *gTexture = new Texture("res/terrain/grassFlowers.png", GL_LINEAR, GL_LINEAR); gTexture->setMipmapping();
	Texture *bTexture = new Texture("res/terrain/path.png", GL_LINEAR, GL_LINEAR); bTexture->setMipmapping();
	Texture *blendMap = new Texture("res/terrain/blendMap.png", GL_LINEAR, GL_LINEAR);
	std::cout << "background texture: " << backgroundTexture << std::endl;


	TerrainTexturePack terrainPack(backgroundTexture->getID(), rTexture->getID(), gTexture->getID(), bTexture->getID());
	Terrain terrain(0.0f, 0.0f, 128.0f, 10.0f, terrainPack, blendMap->getID(), "res/terrain/heightmap.png", &loader);

	std::vector<Terrain> terrains;
	terrains.push_back(terrain);

	int afSpeed = 60;
	float cooldown = 1.0f / (float) afSpeed;
	float tick = 0;
	int uvtick = 0;
	int yTick = 0;
	int lastTick = 0;

	float frames = 0.0f;
	float dt = 0.0f;
	float lt = 0.0f;
	
	while (!window->closed()) {
		double bt = glfwGetTime();
		double sy = window->getScrollY();

		if (sy != 0.0) {
			afSpeed += sy;
			cooldown = 1.0f / (float)afSpeed;
		}

		float lightSpeed = 10.0f;

		if (glfwGetKey(window->getID(), GLFW_KEY_UP)) {
			entity.move(0, lightSpeed * dt, 0);
		}
		if (glfwGetKey(window->getID(), GLFW_KEY_DOWN)) {
			entity.move(0, -lightSpeed * dt, 0);
		}
		if (glfwGetKey(window->getID(), GLFW_KEY_LEFT)) {
			entity.move(-lightSpeed * dt, 0, 0);
		}
		if (glfwGetKey(window->getID(), GLFW_KEY_RIGHT)) {
			entity.move(lightSpeed * dt, 0, 0);
		}
		lights[1].setPosition(entity.getPosition());

		tick += dt;
		if (tick >= cooldown) {
			tick = 0;
			uvtick++;
			if (uvtick >= 8) {
				uvtick = 0;
				yTick++;
				if (yTick >= 8) yTick = 0;
			}
			Particle& p = particles[3];
			const vec4& uv = p.getUV();
			float w = (float) texture3.getWidth();
			float h = (float) texture3.getHeight();
			float xI = 128.0f / w;
			float yI = 128.0f / h;
			float xA = xI * (uvtick);
			float yA = 1.0f - yI * yTick;//lastTick > uvtick ? uv.y + yI : uv.y;
			//if (yA + yI > 1.0f) yA = 0;
			//uv.x + xI > 1.0f ? (uv.y + yI > 1.0f ? 0 : uv.y + yI) : uv.y;
			p.setUV(vec4(xA, yA, xA + xI, yA + yI));
			lastTick = uvtick;
		}

		if (window->resized()) {
			renderer.applyProjection3D(perspective(70.0f, ((float)window->getWidth() / (float)window->getHeight()), 0.1f, 1000.0f));
			renderer.applyProjection2D(ortho((float)0, (float)window->getWidth(), (float)window->getHeight(), (float)0, -1.0f, 1.0f));
			//renderer.applyProjection2D(perspective(70.0f, (float)(window->getWidth() / window->getHeight()), 0.1f, 1000.0f));
		}

		glClearColor(0, 0.5f, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->update(window, dt);
		lights[0].setPosition(camera->getPosition());

		randomEntity.rotate(0, dt * 250.0f, 0);

		glEnable(GL_DEPTH_TEST);
			renderer.processEntity(entity);
			renderer.processEntity(randomEntity);
		renderer.render3D(terrains, lights, particles, camera);


		disableCulling();
		std::string code = toString("AF Speed: %i", afSpeed);//"package com.main\n\npublic class Main {\n\n    public static void main(String[] args) {\n        System.out.println(\"Hello World! :D\");\n    }";
		renderer.beginBatch2D(RENDER_FONT_MODE);
		//renderer.getBatchRenderer2D()->applyModelView(renderer.getModelViewMatrix());
		renderer.submitString(code, vec3(32, 33, 0), font, 0xff000000);
		renderer.submitString(code, vec3(32, 32, 1), font, 0xffffffff);
		renderer.endBatch2D();

		window->update();
		double et = glfwGetTime();
		frames = (float)(1.0 / (et - bt));
		dt = 1.0f / frames;
		lt += dt;
		if (lt >= 1.0f) {
			std::cout << "frames: " << frames << std::endl;
			lt = 0.0f;
		}
	}
}

void Game::update() {}
void Game::render() {}
void Game::dispose() {}
#endif 