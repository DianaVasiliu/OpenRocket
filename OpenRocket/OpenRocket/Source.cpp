#include "loadShaders.h"
#include "Game.h"
#include "helpers.h"

using namespace std;

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	Game* game = Game::getInstance();
	game->InitializeGame("04_03_Shader.vert", "04_03_Shader.frag");
	game->RenderFunction();

	glutMainLoop();
}
