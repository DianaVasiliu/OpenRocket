#include "Game.h"
#include "loadShaders.h"
#include "helpers.h"
#include "Constants.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

void displayMatrix(glm::mat4 matrix)
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
			cout << matrix[ii][jj] << "  ";
		cout << endl;
	};
	cout << "\n";

};

Game* Game::instance = nullptr;

Game* Game::getInstance() {
	if (instance == nullptr) {
		instance = new Game(60, 60);
	}
	return instance;
}

Game::Game(int initial_pos_x, int initial_pos_y) :
	nrOfStars(Constants::nrOfStars),
	width(Constants::maxX),
	height(Constants::maxY),
	maxX(Constants::maxX),
	maxY(Constants::maxY),
	rocketMatrix(glm::mat4(1.0f)),
	rocketScaleMatrix(glm::mat4(1.0f)),
	rocketRotateMatrix(glm::mat4(1.0f)),
	rocketTranslateMatrix(glm::mat4(1.0f))
{
	setInitPosX(initial_pos_x);
	setInitPosY(initial_pos_y);
	maxX = width / 2;
	maxY = height / 2;

	InitializeGlew();
	CreateBackgroundBuffers();
	CreateRocketBuffers();
}

void Game::InitializeGlew() {
	glutInitWindowPosition(getInitPosX(), getInitPosY());
	glutInitWindowSize(getWidth(), getHeight());
	glutCreateWindow(Constants::title);
	glewInit();
	glutDisplayFunc(renderCallback);
	glutSpecialFunc(keysCallback);
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
	DestroyBackgroundVBO();
}

void Game::DestroyBackgroundVBO(void) {
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &backgroundColorBufferId);
	glDeleteBuffers(1, &backgroundVbo);

	glBindVertexArray(0);
	//glDeleteVertexArrays(1, &VaoId);
}

void Game::InitializeGame(const char* vertShader, const char* fragShader) {
	CreateShaders("04_03_Shader.vert", "04_03_Shader.frag");

	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / maxX, 1.f / maxY, 1.0));
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0));

	myMatrix = resizeMatrix * matrTransl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Game::RenderFunction(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (rotationAngle > 360.f) {
		rotationAngle = 0.0f;
	}

	rotationAngle += rotationSpeed;

	backgroundScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / maxX, 1.f / maxY, 1.0));
	backgroundTranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0));
	backgroundRotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, -1.0f));
	backgroundMatrix = backgroundScaleMatrix * backgroundTranslateMatrix * backgroundRotateMatrix;

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &backgroundMatrix[0][0]);

	glPointSize(2.0);
	glBindVertexArray(backgroundVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEbo);
	glDrawArrays(GL_POINTS, 0, nrOfStars);	

	Rocket* rocket = Rocket::getInstance();
	int posX = rocket->getPositionX();
	int posY = rocket->getPositionY();
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / maxX, 1.f / maxY, 1.0));
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0));
	glm::mat4 rocketTranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, 0.0));

	glm::mat4 matrix = scaleMatrix * translateMatrix * rocketTranslateMatrix;

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &matrix[0][0]);

	glBindVertexArray(rocketVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_POLYGON, 6, 4);
	glDrawArrays(GL_TRIANGLES, 10, 3);
	glDrawArrays(GL_POLYGON, 13, 5);
	glDrawArrays(GL_POLYGON, 18, 5);
	
	glutSwapBuffers();
	glutPostRedisplay();
	glFlush();
}

void Game::CreateBackgroundBuffers() {

	GLfloat Vertices[10000];
	GLfloat Colors[10000];

	srand(time(NULL));
	int i = 0;
	while (i < nrOfStars) {
		Vertices[4 * i] = float(rand() % (4 * getWidth()) + 1) - 2 * getWidth();
		Vertices[4 * i + 1] = float(rand() % (4 * getHeight()) + 1) - 2 * getHeight();
		Vertices[4 * i + 2] = 0.f;
		Vertices[4 * i + 3] = 1.f;
		//cout << i << " " << Vertices[4 * i] << " " << Vertices[4 * i + 1] << " " << Vertices[4 * i + 2] << " " << Vertices[4 * i + 3] << "\n";
		Colors[4 * i] = 1.f;
		Colors[4 * i + 1] = 1.f;
		Colors[4 * i + 2] = 1.f;
		Colors[4 * i + 3] = 1.f;
		//cout << i << " " << Colors[4 * i] << " " << Colors[4 * i + 1] << " " << Colors[4 * i + 2] << " " << Colors[4 * i + 3] << "\n";
		i++;
	}

	int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

	glGenBuffers(1, &backgroundVbo);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &backgroundVao);
	glBindVertexArray(backgroundVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &backgroundColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void Game::CreateRocketBuffers() {
	GLfloat Vertices[] = {
		// Triunghiul de sus
		775.f, 160.f, 0.f, 1.f,
		825.f, 160.f, 0.f, 1.f,
		800.f, 185.f, 0.f, 1.f,

		// Triunghiul de jos
		775.f, 100.f, 0.f, 1.f,
		825.f, 100.f, 0.f, 1.f,
		800.f, 165.f, 0.f, 1.f,

		// Deptunghiul
		790.f, 100.f, 0.f, 1.f,  // stanga jos
		810.f, 100.f, 0.f, 1.f, // dr jos
		810.f, 175.f, 0.f, 1.f,
		790.f, 175.f, 0.f, 1.f,

		// Triunghiul din varf
		790.f, 175.f, 0.f, 1.f,
		810.f, 175.f, 0.f, 1.f,
		800.f, 210.f, 0.f, 1.f,

		// Focul portocaliu
		790.f, 100.f, 0.f, 1.f,
		810.f, 100.f, 0.f, 1.f,
		825.f, 85.f, 0.f, 1.f,
		800.f, 50.f, 0.f, 1.f,
		775.f, 85.f, 0.f, 1.f,

		// Focul galben
		790.f, 100.f, 0.f, 1.f,
		810.f, 100.f, 0.f, 1.f,
		815.f, 90.f, 0.f, 1.f,
		800.f, 80.f, 0.f, 1.f,
		785.f, 90.f, 0.f, 1.f,

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

	int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

	glGenBuffers(1, &rocketVbo);
	glBindBuffer(GL_ARRAY_BUFFER, rocketVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &rocketVao);
	glBindVertexArray(rocketVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &rocketColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, rocketColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}