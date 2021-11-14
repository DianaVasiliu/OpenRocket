#include "helpers.h"

void cleanupCallback() {
	Game* game = Game::getInstance();
	game->Cleanup();
}

void renderCallback() {
	Game* game = Game::getInstance();
	game->RenderFunction();
}

void keysCallback(int key, int x, int y) {
	Rocket* rocket = Rocket::getInstance();
	rocket->MoveRocket(key, x, y);
}
