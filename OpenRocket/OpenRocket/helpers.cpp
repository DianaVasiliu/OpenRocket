#include "Game.h"

void cleanupCallback() {
	Game* game = Game::getInstance();
	game->Cleanup();
}

void renderCallback() {
	Game* game = Game::getInstance();
	game->RenderFunction();
}
