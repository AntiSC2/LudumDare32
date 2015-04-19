#include "ai.h"
#include <Managers/rm.h>
#include <Graphics/texture.h>
#include <Graphics/spritebatch.h>


AI::AI(int x, int y, int life) {
	this->life = life;
	destRect.x = x;
	destRect.y = y;
	destRect.z = 16.0f;
	destRect.w = 16.0f;
	textureID = RM::TextureCache->createTexture("resources/textures/player.png")->getID();
}


AI::~AI() {
}

void AI::update(int targetX, int targetY) {
	if (timerDir == 0) {
		timerDir = rand() % 60 + 60;

		glm::vec2 dist((float)targetX - destRect.x, targetY - destRect.y);
		dir = dist;
		dir.x += (float)(rand() % 48 - 16);
		dir.y += (float)(rand() % 48 - 16);

		dir = glm::normalize(dir);
	} else {
		timerDir--;
	}

	destRect += glm::vec4(dir * speed, 0.0f, 0.0f);

	if (destRect.x < 0.0f)
		destRect.x = 0.0f;
	else if (destRect.x + destRect.z > 800.0f)
		destRect.x = 800.0f - destRect.z;
	if (destRect.y < 0.0f)
		destRect.y = 0.0f;
	else if (destRect.y + destRect.w > 600.0f)
		destRect.y = 600.0f - destRect.w;
}

void AI::getHit(unsigned int damage) {
    life -= damage;
}

void AI::goBack() {
	destRect -= glm::vec4(dir * (speed * 40), 0.0f, 0.0f);
}

void AI::draw(SpriteBatch* batch) {
    batch->draw(destRect, glm::vec4(0.0f, 0.0f, 0.5f, 0.5f), textureID, 0.0f, Color(255, 20, 20, 255));
}

int AI::getLife() {
	return life;
}

glm::vec4 AI::getDestRect() {
	return destRect;
}