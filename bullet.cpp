#include "bullet.h"
#include <Graphics/spritebatch.h>
#include <Managers/rm.h>
#include <Graphics/texture.h>

Bullet::Bullet(int targetX, int targetY, int x, int y, int damageMultiplier) {
	destRect.x = (float)x;
	destRect.y = (float)y;
	destRect.z = 16.0f;
	destRect.w = 16.0f;
	dir.x = (float)targetX - (float)x;
	dir.y = (float)targetY - (float)y;
	dir = glm::normalize(dir);

	damage *= damageMultiplier;

	textureID = RM::TextureCache->createTexture("resources/textures/player.png")->getID();
}


Bullet::~Bullet() {

}

void Bullet::update() {
	destRect += glm::vec4(dir * speed, 0.0f, 0.0f);
	if (destRect.x < 0.0f) {
		destRect.x = 0.0f;
		if (life > 0)
			life--;
		if (dir.x < 0.0f)
		    dir.x = -dir.x;
	} else if (destRect.x + destRect.z > 800.0f) {
		destRect.x = 800.0f - destRect.z;
		if (life > 0)
			life--;
		if (dir.x > 0.0f)
		    dir.x = -dir.x;
	}

	if (destRect.y < 0.0f) {
		destRect.y = 0.0f;
		if (life > 0)
			life--;
		if (dir.y < 0.0f)
		    dir.y = -dir.y;
	} else if (destRect.y + destRect.w > 600.0f) {
		destRect.y = 600.0f - destRect.w;
		if (life > 0)
			life--;
		if (dir.y > 0.0f)
		    dir.y = -dir.y;
	}
}

void Bullet::draw(SpriteBatch* batch) {
	if (evil == false)
	    batch->draw(destRect, glm::vec4(0.52f, 0.0f, 0.47f, 1.0f), textureID, 0.0f, Color(20, 20, 255, 255));
	else
		batch->draw(destRect, glm::vec4(0.52f, 0.0f, 0.47f, 1.0f), textureID, 0.0f, Color(200, 0, 0, 255));
}

glm::vec4 Bullet::getDestRect() {
	return destRect;
}

void Bullet::hitTarget() {
	if (evil == false)
		evil = true;
	else
		evil = false;

	if (life > 0)
		life--;
}

unsigned int Bullet::getDamage() {
	return damage;
}

bool Bullet::getEvil() {
	return evil;
}

unsigned int Bullet::getLife() {
	return life;
}