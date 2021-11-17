#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	Game* game = Game::getInstance();
	game->InitializeGame();
	std::cout << "Click anywhere to start a new game\n";
	game->RenderFunction();

	glutMainLoop();
}
