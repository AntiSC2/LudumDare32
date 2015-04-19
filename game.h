#pragma once
#include <Engine.h>
#include <Graphics/spritefont.h>
#include <Graphics/particleengine2D.h>

class Level;

class Game {
public:
	Game();
	~Game();

	void run();
	Engine e;
private:
	Level* level;
	void init();
	void quit();
	void gameLoop();
	void update();
	void draw();
	bool sound = true;
	bool music = true;
	SpriteFont* newFont;
	ParticleEngine2D effectEngine;
	ParticleBatch2D* effectBatch;
};