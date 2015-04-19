#pragma once
#include "player.h"
#include "ai.h"
#include "bullet.h"
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <Graphics/particleengine2D.h>

class SpriteBatch;
class Camera2D;

class Level {
public:
	Level(const char* levelData, ParticleBatch2D* batch);
	~Level();

	void update(Camera2D* camera, bool sound = true, bool music = true);
	void drawLevel(SpriteBatch* batch);

	bool restart();
	

private:
	void cameraShake(Camera2D* camera);
	bool collide(int x, int y, int z, int w, int x2, int y2, int z2, int w2); //GLM::VEC4 won't work for some unexplained reason!
	Player player;
	int cameraShakeTimer = 0;
	std::vector<Bullet> bullets;
	std::vector<AI> enemies;
	unsigned int levelArena = 0;
	GLubyte gameOverAlpha = 255;
	bool gameOver = false;
	bool pause = true;
	bool start = true;
	int damageStat = 0;
	bool upgradePoint = 0;
	int speedStat = 0;
	unsigned int health = 5;
	ParticleBatch2D* effectBatch = nullptr;
};

