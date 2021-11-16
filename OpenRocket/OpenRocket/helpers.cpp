#include "helpers.h"
#include <iostream>

void cleanupCallback() {
	Game* game = Game::getInstance();
	game->Cleanup();
}

void renderCallback() {
	Game* game = Game::getInstance();
	game->RenderFunction();
}

void keysDownCallback(int key, int x, int y) {
	Rocket* rocket = Rocket::getInstance();
	rocket->MarkKeyDown(key, x, y);
}

void keysUpCallback(int key, int x, int y) {
	Rocket* rocket = Rocket::getInstance();
	rocket->MoveRocket(key, x, y);
}

void moveCallback() {
	Game* game = Game::getInstance();
	game->move();
}

void mouseCallback(int button, int state, int x, int y) {
	Game* game = Game::getInstance();
	game->mouseHandler(button, state, x, y);
}

void processNormalKeys(unsigned char key, int x, int y)
{
	Game * game = Game::getInstance();
	switch (key) {
	case 'm':	
		game->GenerateBullet();
		break;
	default:
		break;

	}
}