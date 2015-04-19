#include "game.h"
#include <glm/glm.hpp>
#include <Managers/rm.h>
#include <Graphics/spritefont.h>
#include <Graphics/particleengine2D.h>
#include "level.h"

Game::Game() {
	;
}

Game::~Game() {
	delete level;
	level = nullptr;
	newFont->dispose();
	delete newFont;
	newFont = nullptr;
}

void Game::run() {
	init();
	gameLoop();
	quit();
}

void Game::init() {
	e.initSubSystems();
	e.initScreen(800, 600, "Arena King");
	e.initResources("resources/data/Game.data");
	e.initShaders("resources/shaders/shader.vert", "resources/shaders/shader.frag");
	e.camera->setPosition(glm::vec2(400, 300));
	effectBatch = new ParticleBatch2D;
	level = new Level("resources/data/Level.data", effectBatch);
	newFont = new SpriteFont("resources/fonts/Differentiator_MS_Windows_1252_Western.ttf", 32);
	effectBatch->initParticles(1000, 0.08f, "resources/textures/circle.png", [](Particle& p) {p.position += p.velocity; p.color.a = 255 * p.life; });
	effectEngine.addBatch(effectBatch);
}

void Game::gameLoop() {
	const double dt = 16.6666666;

	uint32_t currentTime = SDL_GetTicks();
	double accumulator = 0;
	uint32_t timer = 0;
	uint16_t frames = 0, updates = 0;

	while (!e.input->windowClosed()) {
		uint32_t newTime = SDL_GetTicks();
		accumulator += newTime - currentTime;
		currentTime = newTime;

		while (accumulator >= dt) {
			update();
			accumulator -= dt;
			updates++;
		}
		draw();

		frames++;
		if (SDL_GetTicks() - timer > 1000) {
			printf("\nFPS: %d\nUPS: %d", frames, updates);
			frames = 0;
			updates = 0;
			timer = SDL_GetTicks();
		}
	}
}

void Game::quit() {
	;
}

void Game::update() {
	if (Mix_PlayingMusic() == 0 && music == true) {
		Mix_PlayMusic(RM::MusicCache->createMusic("resources/music/music.mp3"), -1);
	} else if (Mix_PlayingMusic() && music == false) {
		Mix_HaltMusic();
	}
	static bool mousePressed = false;
	e.input->update();
	e.camera->update();
	if (level != nullptr)
	    level->update(e.camera, sound, music);
	if (level != nullptr && level->restart()) {
		e.camera->setPosition(glm::vec2(400.0f, 300.0f));
		delete level;
		level = nullptr;
	}
	if (level == nullptr && Input::keyTyped(SDL_SCANCODE_SPACE)) {
		level = new Level("resources/data/Level.data", effectBatch);
	}
	if (Input::keyTyped(SDL_SCANCODE_M)) {
		if (music == false)
			music = true;
		else
			music = false;
	}
	if (Input::keyTyped(SDL_SCANCODE_N)) {
		if (sound == false)
			sound = true;
		else
			sound = false;
	}
	effectEngine.update();
}

void Game::draw() {
	glActiveTexture(GL_TEXTURE0);
	e.screen->render();
	e.shaders[0]->setCameraMatrix(e.camera->getCameraMatrix());
	e.TheBatch->begin();
	//newFont->draw(e.TheBatch, "Testing123", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, Color(255, 255, 255, 255));
	if (level != nullptr)
		level->drawLevel(e.TheBatch);
	else
		e.TheBatch->draw(glm::vec4(0.0f, 0.0f, 800.0f, 600.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/GameOver.png")->getID(), 0.0f, Color(255, 255, 255, 255));
	effectEngine.render(e.TheBatch);
	e.TheBatch->end();
	e.TheBatch->renderDraw();
	e.screen->update();
}