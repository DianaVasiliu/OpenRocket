#include "helpers.h"
#include "Rocket.h"

void makeCheckImage(void)
{
	int i, j, c;
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 200;

			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}

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
	rocket->MarkKeyUp(key, x, y);
}
void moveCallback() {
	Game* game = Game::getInstance();
	game->move();
}
void mouseCallback(int button, int state, int x, int y) {
	Game* game = Game::getInstance();
	game->mouseHandler(button, state, x, y);
}
