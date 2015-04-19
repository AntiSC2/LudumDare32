#pragma once
#include <glm/glm.hpp>
#ifndef GLEW_BUILD
#define GLEW_BUILD
#endif
#include <GL/glew.h>

class SpriteBatch;

class Bullet {
public:
	Bullet(int targetX, int targetY, int x, int y, int damageMultiplier = 1);
	~Bullet();

	void update();
	void draw(SpriteBatch* batch);

	glm::vec4 getDestRect();
	void hitTarget();
	bool getEvil();
	unsigned int getLife();
	unsigned int getDamage();

private:
	unsigned int life = 5;
	glm::vec4 destRect;
	glm::vec2 dir;
	GLuint textureID;
	bool evil = false;
	float speed = 10.0f;
	unsigned int damage = 1;
};

