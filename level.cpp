#include "level.h"
#include <input.h>
#include <Camera/camera2D.h>
#include <fstream>
#include <Graphics/spritebatch.h>
#include <Managers/rm.h>
#include <Graphics/texture.h>

Level::Level(const char* levelData, ParticleBatch2D* batch) : effectBatch(batch) {

}


Level::~Level() {
	effectBatch = nullptr;
}

void Level::update(Camera2D* camera, bool sound, bool music) {
	if (enemies.size() == 0) {
		if (pause == false) {
			pause = true;
			upgradePoint = true;
		}
		if (pause == true && start == true) {
			pause = false;
			start = false;
		}
		if (pause == true && camera->getPosition().x < 1200.0f)
			camera->setPosition(glm::vec2(camera->getPosition().x + 10.0f, 300.0f));
		else if (pause == false) {
			bullets.clear();
			if (levelArena < 9)
			    levelArena++;
			for (int i = 0; i < levelArena; i++) {
				enemies.emplace_back(rand() % 784, rand() % 584, levelArena + 2);
				enemies.emplace_back(rand() % 784, rand() % 584, levelArena + 2);
			}
		}
	}
	//Restore health
	if (pause == true && Input::mouseClicked(SDL_BUTTON_LEFT) && Input::getMouseY(camera) > 516 && camera->getPosition().x >= 1199.9f) {
		start = true;
	}
	if (pause == true && Input::mousePressed(SDL_BUTTON_LEFT) && Input::getMouseX(camera) > 1532 && Input::getMouseX(camera) < 1596
		&& Input::getMouseY(camera) > 180 && Input::getMouseY(camera) < 246) {
		if (upgradePoint && health < 5) {
			upgradePoint = false;
			health++;
		}
	}
	//Damage
	if (pause == true && Input::mousePressed(SDL_BUTTON_LEFT) && Input::getMouseX(camera) > 1532 && Input::getMouseX(camera) < 1596
		&& Input::getMouseY(camera) > 256 && Input::getMouseY(camera) < 320) {
		if (upgradePoint && damageStat < 4) {
			upgradePoint = false;
			damageStat++;
		}
	}
	//Speed
	if (pause == true && Input::mousePressed(SDL_BUTTON_LEFT) && Input::getMouseX(camera) > 1532 && Input::getMouseX(camera) < 1596
		&& Input::getMouseY(camera) > 342 && Input::getMouseY(camera) < 406) {
		if (upgradePoint && speedStat < 6) {
			upgradePoint = false;
			player.addSpeed();
			speedStat++;
		}
	}
	//Start with space
	if (Input::keyPressed(SDL_SCANCODE_SPACE) && pause == true && camera->getPosition().x >= 1199.9f) {
		start = true;
	}
	if (camera->getPosition().x > 400.0f && pause == false)
		camera->setPosition(glm::vec2(camera->getPosition().x - 10.0f, 300.0f));
	if (pause == false && camera->getPosition().x <= 400.1f) {
		if (cameraShakeTimer > 0) {
			camera->setPosition(glm::vec2(400.0f, 300.0f));
			cameraShakeTimer--;
			if (cameraShakeTimer != 0)
				cameraShake(camera);
			else
				camera->setPosition(glm::vec2(400, 300));
		}

		player.update();
		//player.setPosition(collideLevel(player.getDestRect().x, player.getDestRect().y, player.getDestRect().z, player.getDestRect().y));
		if (player.getCreateBullet()) {
			bullets.emplace_back(Input::getMouseX(camera), Input::getMouseY(camera), player.getX(), player.getY(), damageStat + 1);
			if (sound == true) {
				Mix_PlayChannel(-1, RM::SoundCache->createSound("resources/sounds/Shoot.wav"), 0);
			}
		}
		//BULLETS
		for (size_t i = 0; i < bullets.size(); i++) {
			bullets[i].update();
			if (bullets[i].getEvil() && collide(bullets[i].getDestRect().x,
				bullets[i].getDestRect().y,
				bullets[i].getDestRect().z,
				bullets[i].getDestRect().w,
				player.getDestRect().x,
				player.getDestRect().y,
				player.getDestRect().z,
				player.getDestRect().w
				)) {
				bullets[i].hitTarget();
				if (health > 0)
					health--;
				cameraShakeTimer = 15;
				cameraShake(camera);
				if (sound == true) {
					Mix_PlayChannel(-1, RM::SoundCache->createSound("resources/sounds/Hit.wav"), 0);
				}
			}

			if (bullets[i].getLife() == 0) {
				bullets[i] = bullets.back();
				bullets.pop_back();
				i--;
			}
		}
		//ENEMIES
		for (size_t i = 0; i < enemies.size(); i++) {
			enemies[i].update(player.getDestRect().x, player.getDestRect().y);
			for (size_t b = 0; b < bullets.size(); b++)
				if (bullets[b].getEvil() == false && collide(bullets[b].getDestRect().x,
					bullets[b].getDestRect().y,
					bullets[b].getDestRect().z,
					bullets[b].getDestRect().w,
					enemies[i].getDestRect().x,
					enemies[i].getDestRect().y,
					enemies[i].getDestRect().z,
					enemies[i].getDestRect().w)
					) {

					bullets[b].hitTarget();
					enemies[i].getHit(bullets[b].getDamage());
					for (size_t p = 0; p < 20; p++) {
						int dirX = rand() % 32 - 16;
						int dirY = rand() % 32 - 16;
						float speed = rand() % 6 + 3;
						glm::vec2 tempDir(dirX, dirY);
						tempDir = glm::normalize(tempDir);
						effectBatch->addParticle(glm::vec2(enemies[i].getDestRect().x, enemies[i].getDestRect().y), tempDir * speed, rand() % 8 + 8, rand() % 8 + 8, Color(200, 0, 0, 255));
					}
				}
			if (collide(player.getDestRect().x, player.getDestRect().y, player.getDestRect().z, player.getDestRect().w,
				enemies[i].getDestRect().x, enemies[i].getDestRect().y, enemies[i].getDestRect().z, enemies[i].getDestRect().w)) {
				health--;
				if (sound == true) {
					Mix_PlayChannel(-1, RM::SoundCache->createSound("resources/sounds/Hit.wav"), 0);
				}
				cameraShakeTimer = 15;
				cameraShake(camera);
				enemies[i].goBack();
			}

			if (enemies[i].getLife() <= 0) {
				enemies[i] = enemies.back();
				enemies.pop_back();
				i--;
			}
		}
	}
	if (health == 0) {
		pause = true;
		camera->setPosition(glm::vec2(400.0f, 300.0f));
		if (gameOverAlpha == 255 && gameOver == 0) {
			gameOver = 1;
		} else if (gameOver == 1 && gameOverAlpha > 0) {
			gameOverAlpha--;
		}
	}
}

void Level::drawLevel(SpriteBatch* batch) {
	batch->draw(glm::vec4(800.0f, 0.0f, 800.0f, 600.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/upgradeScreen.png")->getID(), 0.0f, Color(255, 255, 255, gameOverAlpha));
	if (levelArena == 1)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/one.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 2)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/two.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 3)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/three.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 4)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/four.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 5)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/five.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 6)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/six.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 7)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/seven.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 8)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/eight.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	else if (levelArena == 9)
		batch->draw(glm::vec4(1348.0f, -48.0f, 256, 256), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/nine.png")->getID(), 0.0f, Color(0, 0, 0, 255));
	//Restore One health point
	batch->draw(glm::vec4(1532.0f, 180.0f, 64, 64), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/arrow.png")->getID(), 0.0f, Color(255, 255, 255, 255));
	for (size_t i = 0; i < health; i++) {
		batch->draw(glm::vec4(1408.0f + (i * 34), 204, 32.0f, 16.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/healthBrick.png")->getID(), 0.0f, Color(255, 255, 255, gameOverAlpha));
	}
	//Damage
	batch->draw(glm::vec4(1532.0f, 256.0f, 64, 64), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/arrow.png")->getID(), 0.0f, Color(255, 255, 255, 255));
	for (size_t i = 0; i < damageStat; i++) {
		batch->draw(glm::vec4(1440.0f + (i * 34.0f), 282.0f, 32.0f, 16.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/healthBrick.png")->getID(), 0.0f, Color(255, 255, 255, gameOverAlpha));
	}
	//Speed
	batch->draw(glm::vec4(1532.0f, 342, 64, 64), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/arrow.png")->getID(), 0.0f, Color(255, 255, 255, 255));
	for (size_t i = 0; i < speedStat; i++) {
		batch->draw(glm::vec4(1376.0f + (i * 34.0f), 366, 32.0f, 16.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/healthBrick.png")->getID(), 0.0f, Color(255, 255, 255, gameOverAlpha));
	}
	batch->draw(glm::vec4(0.0f, 0.0f, 800.0f, 600.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/background.png")->getID(), 0.0f, Color(255, 255, 255, gameOverAlpha));
	batch->draw(glm::vec4(256.0f, 284.0f, 288.0f, 64.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/healthbar.png")->getID(), 0.0f, Color(255, 255, 255, gameOverAlpha));
	if (health != 0)
		player.draw(batch);
	if (!gameOver) {
		for (size_t i = 0; i < enemies.size(); i++) {
			enemies[i].draw(batch);
		}
		for (size_t i = 0; i < bullets.size(); i++) {
			bullets[i].draw(batch);
		}
		for (size_t i = 0; i < health; i++) {
			batch->draw(glm::vec4(280.0f + (i * 52.0f), 324.0f, 32.0f, 16.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), RM::TextureCache->createTexture("resources/textures/healthBrick.png")->getID(), 0.0f, Color(255, 255, 255, gameOverAlpha));
		}
	}
}

bool Level::restart() {
	return gameOverAlpha == 0;
}

void Level::cameraShake(Camera2D* camera) {
	int offsetX = rand() % 32 - 16;
	int offsetY = rand() % 32 - 16;
	glm::vec2 temp = camera->getPosition();
	camera->setPosition(glm::vec2(temp.x + offsetX, temp.y + offsetY));
}

bool Level::collide(int x, int y, int z, int w, int x2, int y2, int z2, int w2) {
	if (x + z < x2)
		return false;
	else if (x > z2 + x2)
		return false;
	else if (y + w < y2)
		return false;
	else if (y > y2 + w2)
		return false;
	return true;
}