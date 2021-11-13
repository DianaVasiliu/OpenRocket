#include "Game.h"
#include "Background.h"
#include "loadShaders.h"
#include "helpers.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

Game* Game::instance = nullptr;

Game* Game::getInstance(){
	if (instance == nullptr) {
		instance = new Game(1600, 900, 60, 60);
	}
	return instance;
}

Game::Game(int window_width, int window_height, int initial_pos_x, int initial_pos_y) : VAO() {
	setWidth(window_width);
	setHeight(window_height);
	setInitPosX(initial_pos_x);
	setInitPosY(initial_pos_y);
	maxX = width / 2;
	maxY = height / 2;
}

void Game::InitializeGlew() {
	glutInitWindowPosition(getInitPosX(), getInitPosY());
	glutInitWindowSize(getWidth(), getHeight());
	glutCreateWindow(getTitle());
	glewInit();
	glutDisplayFunc(renderCallback);
	glutCloseFunc(cleanupCallback);
}

void Game::CreateShaders(const char* vertShader, const char* fragShader) {
	ProgramId = LoadShaders(vertShader, fragShader);
	glUseProgram(ProgramId);
}

void Game::DestroyShaders(void) {
	glDeleteProgram(ProgramId);
}

void Game::Cleanup(void) {
	DestroyShaders();
	DestroyVBO();
}

void Game::DestroyVBO(void) {
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void Game::InitializeGame(const char* vertShader, const char* fragShader) {
	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / maxX, 1.f / maxY, 1.0));
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0));

	myMatrix = resizeMatrix * matrTransl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//CreateVBO();
	Background* background = Background::getInstance();
	background->CreateVBO();
	CreateShaders(vertShader, fragShader);
}

void Game::CreateVBO() {

	// varfurile 
	GLfloat Vertices[] = {
		// Triunghiul de sus
		0.f, 160.f, 0.f, 1.f,
		50.f, 160.f, 0.f, 1.f,
		25.f, 185.f, 0.f, 1.f,

		// Triunghiul de jos
		0.f, 100.f, 0.f, 1.f,
		50.f, 100.f, 0.f, 1.f,
		25.f, 165.f, 0.f, 1.f,

		// Desptunghiul
		15.f, 100.f, 0.f, 1.f,  // stanga jos
		35.f, 100.f, 0.f, 1.f, // dr jos
		35.f, 175.f, 0.f, 1.f,
		15.f, 175.f, 0.f, 1.f,

		// Triunghiul din varf
		15.f, 175.f, 0.f, 1.f,
		35.f, 175.f, 0.f, 1.f,
		25.f, 210.f, 0.f, 1.f,

		// Focul portocaliu
		15.f, 100.f, 0.f, 1.f,
		35.f, 100.f, 0.f, 1.f,
		50.f, 85.f, 0.f, 1.f,
		25.f, 50.f, 0.f, 1.f,
		0.f, 85.f, 0.f, 1.f,

		// Focul galben
		15.f, 100.f, 0.f, 1.f,
		35.f, 100.f, 0.f, 1.f,
		40.f, 90.f, 0.f, 1.f,
		25.f, 80.f, 0.f, 1.f,
		10.f, 90.f, 0.f, 1.f,

	};

	GLfloat Colors[] = {
		// Triunghiul de sus
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,

		// Triunghiul de jos
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,

		// Dreptunghiul
		0.8f, 0.5f, 0.2f, 0.f,
		0.8f, 0.5f, 0.2f, 0.f,
		0.8f, 0.5f, 0.2f, 0.f,
		0.8f, 0.5f, 0.2f, 0.f,

		// Triunghiul din varf
		1.0f, 1.0f, 1.0f, 0.f,
		1.0f, 1.0f, 1.0f, 0.f,
		1.0f, 1.0f, 1.0f, 0.f,

		// Focul portocaliu
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,

		// Focul galben
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f
	};

	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	// se activeaza lucrul cu atribute; atributul 0 = pozitie
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

void Game::RenderFunction(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glPointSize(10.0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_POLYGON, 6, 4);
	glDrawArrays(GL_TRIANGLES, 10, 3);
	glDrawArrays(GL_POLYGON, 13, 5);
	glDrawArrays(GL_POLYGON, 18, 5);

	glutPostRedisplay();
	glFlush();
}
