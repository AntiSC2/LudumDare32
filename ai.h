#pragma once
#include <glm/glm.hpp>
#ifndef GLEW_BUILD
#define GLEW_BUILD
#endif
#include <GL/glew.h>

class SpriteBatch;

class AI {
public:
	AI(int x, int y, int life);
	~AI();

	void update(int targetX, int targetY);
	void draw(SpriteBatch* batch);
	void getHit(unsigned int damage);
	void goBack();

	int getLife();

	glm::vec4 getDestRect();
private:
	glm::vec4 destRect;
	glm::vec2 dir;
	float speed = 3.0f;
	GLuint textureID = 0;
	int life = 3;
	int timerDir = 0;
};

