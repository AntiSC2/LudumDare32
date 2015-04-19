#include "player.h"
#include <Graphics/spritebatch.h>
#include <Managers/rm.h>
#include <Graphics/texture.h>
#include <input.h>

Player::Player() {
	destRect.x = 400.0f;
	destRect.y = 300.0f;
	destRect.z = 16.0f;
	destRect.w = 16.0f;
	dir.x = 0.0f;
	dir.y = 0.0f;

	textureID = RM::TextureCache->createTexture("resources/textures/player.png")->getID();
}


Player::~Player() {

}

void Player::update() {
	createBullet = false;
	if (BulletTimer > 0)
		BulletTimer--;
	keysPressed = true;

	if (Input::keyPressed(SDL_SCANCODE_W)) {
		dir.y = -1.0f;
	} else if (Input::keyPressed(SDL_SCANCODE_S)) {
		dir.y = 1.0f;
	} else {
		dir.y = 0.0f;
		keysPressed = false;
	}

	if (Input::keyPressed(SDL_SCANCODE_A)) {
		dir.x = -1.0f;
		keysPressed = true;
	} else if (Input::keyPressed(SDL_SCANCODE_D)) {
		dir.x = 1.0f;
		keysPressed = true;
	} else {
		dir.x = 0.0f;
		keysPressed = false;
	}
	if (keysPressed == true)
		dir = glm::normalize(dir);

	glm::vec2 temp = dir * speed;
	destRect += glm::vec4(temp, 0.0f, 0.0f);

	if (destRect.x < 0.0f)
		destRect.x = 0.0f;
	else if (destRect.x + destRect.z > 800.0f)
		destRect.x = 800.0f - destRect.z;
	if (destRect.y < 0.0f)
		destRect.y = 0.0f;
	else if (destRect.y + destRect.w > 600.0f)
		destRect.y = 600.0f - destRect.w;

	if (BulletTimer == 0) {
		if (Input::mousePressed(SDL_BUTTON_LEFT)) {
			createBullet = true;
			BulletTimer = 10;
		}
	}
}

void Player::draw(SpriteBatch* batch) {
    batch->draw(destRect, glm::vec4(0.0f, 0.0f, 0.5f, 0.5f), textureID, 0.0f, Color(20, 20, 255, 255));
}

bool Player::getCreateBullet() {
	return createBullet;
}

int Player::getX() {
	return (int)destRect.x;
}

int Player::getY() {
	return (int)destRect.y;
}

glm::vec4& Player::getDestRect() {
	return destRect;
}

void Player::setPosition(glm::vec2 newPosition) {
	destRect = glm::vec4(newPosition, 16.0f, 16.0f);
}

void Player::addSpeed() {
	speed += 1.0f;
}