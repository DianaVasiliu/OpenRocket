#include "Game.h"

void cleanupCallback() {
	Game* game = Game::getInstance();
	game->Cleanup();
}

void renderCallback() {
	Game* game = Game::getInstance();
	game->RenderFunction();
}
void moveCallback() {
	Game* game = Game::getInstance();
	game->move();
}
void mouseCallback(int button, int state, int x, int y) {
	Game* game = Game::getInstance();
	game->mouseHandler(button, state, x, y);
}
