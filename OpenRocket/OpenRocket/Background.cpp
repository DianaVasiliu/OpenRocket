#include "Background.h"
#include "Game.h"
#include "loadShaders.h"
#include "helpers.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

Background* Background::instance = nullptr;

Background* Background::getInstance() {
	if (instance == nullptr) {
		instance = new Background();
	}
	return instance;
}

Background::Background() {
}

void Background::CreateShaders(const char* vertShader, const char* fragShader) {
	ProgramId = LoadShaders(vertShader, fragShader);
	glUseProgram(ProgramId);
}

void Background::DestroyShaders(void) {
	glDeleteProgram(ProgramId);
}

void Background::Cleanup(void) {
	DestroyShaders();
	DestroyVBO();
}

void Background::DestroyVBO(void) {
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void Background::InitializeGame(const char* vertShader, const char* fragShader) {
	Game* game = Game::getInstance();
	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / game->getMaxX(), 1.f / game->getMaxY(), 1.0));
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(-game->getMaxX(), -game->getMaxY(), 0.0));

	myMatrix = resizeMatrix * matrTransl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	CreateVBO();
	CreateShaders(vertShader, fragShader);
}

void Background::CreateVBO() {
	Background* background = Background::getInstance();
	Game* game = Game::getInstance();
	// varfurile 
	GLfloat Vertices[1000];
	GLfloat Colors[1000];

	srand(time(NULL));
	int i = 0;
	while (i < nrOfStars) {
		Vertices[4 * i] = float(rand() % (2 * game->getWidth()) + 1);
		Vertices[4 * i + 1] = float(rand() % (2 * game->getHeight()) + 1);
		Vertices[4 * i + 2] = 0.f;
		Vertices[4 * i + 3] = 1.f;
		cout << i << " " << Vertices[4 * i] << " " << Vertices[4 * i + 1] << " " << Vertices[4 * i + 2] << " " << Vertices[4 * i + 3] << "\n";
		Colors[4 * i] = 1.f;
		Colors[4 * i + 1] = 1.f;
		Colors[4 * i + 2] = 1.f;
		Colors[4 * i + 3] = 1.f;
		cout << i << " " << Colors[4 * i] << " " << Colors[4 * i + 1] << " " << Colors[4 * i + 2] << " " << Colors[4 * i + 3] << "\n";
		i++;
	}

	// se creeaza un buffer nou
	glGenBuffers(2, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	// 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void Background::RenderFunction(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, 200);

	glutPostRedisplay();
	glFlush();
}
