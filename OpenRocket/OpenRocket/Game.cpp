#include "Game.h"
#include "loadShaders.h"
#include "helpers.h"
#include "Constants.h"
#include "Asteroid.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

void Game::move(void)
{
	for (auto& asteroid : asteroids) {
		float translatedDistance = asteroid->getTranslatedDistance();
		asteroid->setTranslatedDistance(translatedDistance + Constants::asteroidMovingUnit);
	}
	glutPostRedisplay();
}
void Game::mouseHandler(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		glutIdleFunc(moveCallback);
		break;
	default:
		break;
	}
}
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

	//// Triunghiul de sus
	//775.f, 160.f, 0.f, 1.f,
	//825.f, 160.f, 0.f, 1.f,
	//800.f, 185.f, 0.f, 1.f,

	//// Triunghiul de jos
	//775.f, 100.f, 0.f, 1.f,
	//825.f, 100.f, 0.f, 1.f,
	//800.f, 165.f, 0.f, 1.f,

	//// Deptunghiul
	//790.f, 100.f, 0.f, 1.f,  // stanga jos
	//810.f, 100.f, 0.f, 1.f, // dr jos
	//810.f, 175.f, 0.f, 1.f,
	//790.f, 175.f, 0.f, 1.f,

	//// Triunghiul din varf
	//790.f, 175.f, 0.f, 1.f,
	//810.f, 175.f, 0.f, 1.f,
	//800.f, 210.f, 0.f, 1.f,


	InitializeGlew();
	CreateBackgroundBuffers();
	CreateRocketBuffers();
	CreateAsteroidBuffers();
	GenerateAsteroids();
}

void Game::InitializeGlew() {
	glutInitWindowPosition(getInitPosX(), getInitPosY());
	glutInitWindowSize(getWidth(), getHeight());
	glutCreateWindow(Constants::title);
	glewInit();
	glutDisplayFunc(renderCallback);
	glutSpecialFunc(keysDownCallback);
	glutSpecialUpFunc(keysUpCallback);
	glutMouseFunc(mouseCallback);
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

void Game::FireAnimation() {
	if (fireTail >= resetTailEvery) {
		fireTail = resetTailEvery;
		fireSides = 0;
		fireGoingDown = true;
		fireGoingUp = false;
	}
	else if (fireTail <= 0) {
		fireTail = 0;
		fireSides = resetSidesEvery;
		fireGoingUp = true;
		fireGoingDown = false;
	}
	if (fireGoingUp) {
		fireTail += fireTailVelocity;
		fireSides -= fireSidesVelocity;
	}
	else if (fireGoingDown) {
		fireTail -= fireTailVelocity;
		fireSides += fireSidesVelocity;
	}

	float orangeTail = 50.0f + fireTail;
	float leftSideX = 775 + 0.f - fireSides;
	float leftSideY = 85.f - fireSides;
	float rightSideX = 775 + 50.f + fireSides;
	float rightSideY = 85.f - fireSides;
	float yellowTail = 70.f + fireTail;
	glNamedBufferSubData(rocketVbo, 15 * 4 * sizeof(GLfloat), sizeof(GLfloat), &rightSideX);
	glNamedBufferSubData(rocketVbo, (15 * 4 + 1) * sizeof(GLfloat), sizeof(GLfloat), &rightSideY);
	glNamedBufferSubData(rocketVbo, 16 * 4 * sizeof(GLfloat) + 1 * sizeof(GLfloat), sizeof(GLfloat), &orangeTail);
	glNamedBufferSubData(rocketVbo, 17 * 4 * sizeof(GLfloat), sizeof(GLfloat), &leftSideX);
	glNamedBufferSubData(rocketVbo, (17 * 4 + 1) * sizeof(GLfloat), sizeof(GLfloat), &leftSideY);
	glNamedBufferSubData(rocketVbo, 21 * 4 * sizeof(GLfloat) + 1 * sizeof(GLfloat), sizeof(GLfloat), &yellowTail);
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

	FireAnimation();

	Rocket* rocket = Rocket::getInstance();
	int posX = rocket->getPositionX();
	int posY = rocket->getPositionY();
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / maxX, 1.f / maxY, 1.0));
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0));
	glm::mat4 rocketTranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, 0.0));

	glm::mat4 matrix = scaleMatrix * translateMatrix * rocketTranslateMatrix;
	rocket->setRocketMatrix(matrix);

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &matrix[0][0]);

	glBindVertexArray(rocketVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_POLYGON, 6, 4);
	glDrawArrays(GL_TRIANGLES, 10, 3);

	glDrawArrays(GL_POLYGON, 13, 5);
	glDrawArrays(GL_POLYGON, 18, 5);	

	Game::UpdateAsteroids();
	for (auto& asteroid : asteroids) {

		
		glm::mat4 asteroidMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(asteroid->getRadius(), asteroid->getRadius(), 1.0));
		glm::mat4 animateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, - asteroid->getTranslatedDistance(), 0.0)); // controleaza translatia de-a lungul lui Oy
		asteroidMatrix = backgroundMatrix *  animateMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(asteroid->getX(), asteroid->getY(), 0.0)) * asteroidMatrix;
		asteroid->setAsteroidMatrix(asteroidMatrix);

		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &asteroidMatrix[0][0]);
		glBindVertexArray(asteroidVao);
		glDrawArrays(GL_POLYGON, 0, Constants::nrOfVerticesPerCircle);
	
	}

	rocket->RocketAsteroidsCollision(asteroids);
	glutPostRedisplay();
	glFlush();
}

bool colliding(Rocket rocket, Asteroid asteroid)
{
	return false;
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

	Rocket* rocket = Rocket::getInstance();
	GLfloat Vertices[] = {
		// Triunghiul de sus
		rocket->topWingsTriangle.left[0], rocket->topWingsTriangle.left[1], 0.f, 1.f,
		rocket->topWingsTriangle.right[0], rocket->topWingsTriangle.right[1], 0.f, 1.f,
		rocket->topWingsTriangle.top[0], rocket->topWingsTriangle.top[1], 0.f, 1.f,

		// Triunghiul de jos
		rocket->bottomWingsTriangle.left[0], rocket->bottomWingsTriangle.left[1], 0.f, 1.f,
		rocket->bottomWingsTriangle.right[0], rocket->bottomWingsTriangle.right[1], 0.f, 1.f,
		rocket->bottomWingsTriangle.top[0], rocket->bottomWingsTriangle.top[1], 0.f, 1.f,

		// Dreptunghiul
		rocket->body.bottomLeft[0], rocket->body.bottomLeft[1], 0.f, 1.f, 
		rocket->body.bottomRight[0], rocket->body.bottomRight[1], 0.f, 1.f,
		rocket->body.topRight[0], rocket->body.topRight[1], 0.f, 1.f,
		rocket->body.topLeft[0], rocket->body.topLeft[1], 0.f, 1.f,

		// Triunghiul din varf
		rocket->frontTriangle.left[0], rocket->frontTriangle.left[1], 0.f, 1.f,
		rocket->frontTriangle.right[0], rocket->frontTriangle.right[1], 0.f, 1.f,
		rocket->frontTriangle.top[0], rocket->frontTriangle.top[1], 0.f, 1.f,
		// Focul portocaliu
		775 + 15.f, 100.f, 0.f, 1.f,
		775 + 35.f, 100.f, 0.f, 1.f,
		775 + 50.f, 85.f, 0.f, 1.f, // dreapta
		775 + 25.f, 50.f, 0.f, 1.f, // varful de jos
		775 + 0.f, 85.f, 0.f, 1.f, // stanga

		// Focul galben
		775 + 15.f, 100.f, 0.f, 1.f,
		775 + 35.f, 100.f, 0.f, 1.f,
		775 + 40.f, 90.f, 0.f, 1.f,
		775 + 25.f, 70.f, 0.f, 1.f, // varful de jos
		775 + 10.f, 90.f, 0.f, 1.f,

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
void Game::CreateAsteroidBuffers() {

	GLfloat Vertices[1000];
	GLfloat Colors[1000];
	for (int k = 0; k < Constants::nrOfVerticesPerCircle; k++) {
		int theta = Constants::TWO_PI * float(k) / float(Constants::nrOfVerticesPerCircle);
		cout << "angle " << float(Constants::TWO_PI * float(k)) / float(Constants::nrOfVerticesPerCircle) << "\n";
		float x = cos(theta);
		float y = sin(theta);
		Asteroid::circleCenter = { 0.0f, 0.0f, 0.0f, 1.0f };
		Asteroid::circlePoint = { cos(theta), sin(theta), 0.0f, 1.0f };
		// varfurile corespunzatoare cercului
		Vertices[4 * k] = x;
		Vertices[4 * k + 1] = y;
		Vertices[4 * k + 2] = 0.0f;
		Vertices[4 * k + 3] = 1.0f;
		cout << 4*k << " " << Vertices[4 * k] << " " << Vertices[4 * k + 1] << " " << Vertices[4 * k + 2] << " " << Vertices[4 * k + 3] << "\n";

		Colors[4 * k] = 1.0f;
		Colors[4 * k + 1] = 0.0f;
		Colors[4 * k + 2] = 0.0f;
		Colors[4 * k + 3] = 1.0f;
	}
	for (int i = 0; i < Constants::nrOfVerticesPerCircle * 4 - 4; i+=4) {
		//cout << Vertices[i] << " " << Vertices[i + 1] << " " << Vertices[i + 2] << " " << Vertices[i + 3] << "\n";
	}
	int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

	glGenBuffers(1, &asteroidVbo);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &asteroidVao);
	glBindVertexArray(asteroidVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &asteroidColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

}
void Game::UpdateAsteroids() {
	auto end = std::remove_if(asteroids.begin(),
		asteroids.end(),
		[](Asteroid* const& i) {
		return i->belowViewport();  
	});

	asteroids.erase(end, asteroids.end());

	if (static_cast<int>(asteroids.size()) < Constants::nrOfAsteroidsPerFrame) {
		int nrToGenerate = Constants::nrOfAsteroidsPerFrame - asteroids.size();
		for (int k = 0; k < nrToGenerate; k++) {
			this->asteroids.push_back(Game::GenerateSingleAsteroid());
		}
	}
}
float Game::generateXOffset() {
	float maxDistance = 0, leftOffset = 0, rightOffset = 2 * Constants::width; 
	if (static_cast<int>(asteroids.size()) == 0) {
		leftOffset += Constants::distanceBetweenAsteroids;
		rightOffset -= Constants::distanceBetweenAsteroids;
		return leftOffset + rand() % ((int)(rightOffset - leftOffset + 1));
	}
	int i = 0;
	float leftAsteroid = asteroids[0]->getX(), rightAsteroid = asteroids[static_cast<int>(asteroids.size()) - 1]->getX();
	while(i < static_cast<int>(asteroids.size()) - 1) {
		leftAsteroid = min(leftAsteroid, asteroids[i]->getX());
		rightAsteroid = max(rightAsteroid, asteroids[i]->getX());
		if (maxDistance < abs(asteroids[i]->getX() - asteroids[i + 1]->getX())) {
			maxDistance = abs(asteroids[i]->getX() - asteroids[i + 1]->getX());
			rightOffset = asteroids[i + 1]->getX();
			leftOffset = asteroids[i]->getX();
			if (leftOffset > rightOffset) {
				swap(leftOffset, rightOffset);
			}
		}
		i++;
	}
	leftAsteroid = min(leftAsteroid, asteroids[i]->getX());
	rightAsteroid = max(rightAsteroid, asteroids[i]->getX());
	if (maxDistance < leftAsteroid) {
		maxDistance = leftAsteroid;
		rightOffset = leftAsteroid;
		leftOffset = 0;
	}
	if (maxDistance < 2 * Constants::width - rightAsteroid) {
		maxDistance = 2 * Constants::width - rightAsteroid;
		rightOffset = 2 * Constants::width;
		leftOffset = rightAsteroid;
	}
	return leftOffset + rand() % ((int)(rightOffset - leftOffset + 1));
}
Asteroid* Game::GenerateSingleAsteroid() {
	// generam random coordonata x
	float y_offset = 2 * Constants::height + rand() % (3 * Constants::height - 2 * Constants::height + 1);
	float x_offset = Game::generateXOffset();
	return new Asteroid(40.0, 20, glm::vec4(x_offset, y_offset, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}
void Game::GenerateAsteroids() {
	// generam un numar de asteroizi initial intr-o pozitie care nu se afla in view-ul nostru
	for (int i = 0; i < Constants::nrOfAsteroidsPerFrame; i++) {
		this->asteroids.push_back(Game::GenerateSingleAsteroid());
	}
}