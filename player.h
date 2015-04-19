#pragma once
#include <glm/glm.hpp>
#ifndef GLEW_BUILD
#define GLEW_BUILD
#endif
#include <GL/glew.h>
#include <vector>


class SpriteBatch;

class Player {
public:
	Player();
	~Player();

	void update();
	void draw(SpriteBatch* batch);

	bool getCreateBullet();
	int getX();
	int getY();
	glm::vec4& getDestRect();
	void setPosition(glm::vec2 newPosition);
	void addSpeed();

private:
	bool createBullet = false;
	unsigned int BulletTimer = 0;
	glm::vec4 destRect;
	glm::vec2 dir;
	float speed = 4.0f;
	bool keysPressed = false;
	GLuint textureID = 0;
};

