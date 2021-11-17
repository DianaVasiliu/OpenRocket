#include "loadShaders.h"
#include "Game.h"
#include "helpers.h"

using namespace std;

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	Game* game = Game::getInstance();
	game->InitializeGame();
	game->RenderFunction();

	glutMainLoop();
}
