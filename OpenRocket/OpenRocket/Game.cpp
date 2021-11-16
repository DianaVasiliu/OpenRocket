#include "Game.h"
#include "loadShaders.h"
#include "helpers.h"
#include "Constants.h"
#include "Asteroid.h"
#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>


void displayMatrix(glm::mat4 matrix) {
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
			cout << matrix[ii][jj] << "  ";
		cout << endl;
	};
	cout << "\n";

};

void Game::move(void) {
	for (auto& asteroid : asteroids) {
		float translatedDistance = asteroid->getTranslatedDistance();
		asteroid->setTranslatedDistance(translatedDistance + Constants::asteroidMovingUnit);
	}

	for (auto& bullet : bullets) {
		float bulletY = bullet->getY();
		bullet->setY(bulletY + Constants::bulletSpeed);
	}

	glutPostRedisplay();
}

void Game::mouseHandler(int button, int state, int x, int y) {

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
	CreateBulletBuffers();
	GenerateAsteroids(Constants::nrOfAsteroidsPerFrame);
}

void Game::InitializeGlew() {
	glutInitWindowPosition(getInitPosX(), getInitPosY());
	glutInitWindowSize(getWidth(), getHeight());
	glutCreateWindow(Constants::title);
	glewInit();
	glutDisplayFunc(renderCallback);
	glutSpecialFunc(keysDownCallback);
	glutSpecialUpFunc(keysUpCallback);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(mouseCallback);
	glutCloseFunc(cleanupCallback);
	glfwInit();
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
	glDeleteBuffers(1, &backgroundColorBufferId);
	glDeleteBuffers(1, &backgroundVbo);
	glDeleteBuffers(1, &rocketColorBufferId);
	glDeleteBuffers(1, &rocketVbo);
	glDeleteBuffers(1, &asteroidColorBufferId);
	glDeleteBuffers(1, &asteroidVbo);
	glDeleteBuffers(1, &bulletColorBufferId);
	glDeleteBuffers(1, &bulletVbo);

	glBindVertexArray(0);
}

void Game::InitializeGame(const char* vertShader, const char* fragShader) {
	CreateShaders(vertShader, fragShader);

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
	Rocket* rocket = Rocket::getInstance();
	if (rocket->getIsDead()) {
		return;
	}
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

	backgroundMatrix = backgroundScaleMatrix * backgroundTranslateMatrix;

	Game::UpdateAsteroids();

	for (auto& asteroid : asteroids) {				
		glm::mat4 radiusMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(asteroid->getRadius(), asteroid->getRadius(), 1.0));
		glm::mat4 animateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, - asteroid->getTranslatedDistance(), 0.0)); // controleaza translatia de-a lungul lui Oy
		glm::mat4 originalPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(asteroid->getX(), asteroid->getY(), 0.0));
		glm::mat4 asteroidMatrix = backgroundMatrix *  animateMatrix * originalPositionMatrix * radiusMatrix;

		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &asteroidMatrix[0][0]);

		asteroid->setAsteroidMatrix(asteroidMatrix);
		glBindVertexArray(asteroidVao);
		glDrawArrays(GL_POLYGON, 0, Constants::nrOfVerticesPerCircle);
	}	

	Game::UpdateBullets();

	for (auto& bullet : bullets) {
		glm::mat4 bulletMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(bullet->getRadius(), bullet->getRadius(), 1.0f));
		glm::mat4 animateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, bullet->getY(), 0.0));
		bulletMatrix = backgroundMatrix * animateMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(bullet->getX(), bullet->getY(), 0.0)) * bulletMatrix;

		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &bulletMatrix[0][0]);
		glBindVertexArray(bulletVao);
		glDrawArrays(GL_POLYGON, 0, Constants::nrOfVerticesPerCircle);
	}	
	
	}

	/*glBindVertexArray(squareVao);
	matrix = scaleMatrix * translateMatrix;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &matrix[0][0]);

	glDrawArrays(GL_POLYGON, 0, 4);*/

	rocket->RocketAsteroidsCollision(asteroids);

	glutPostRedisplay();
	glFlush();
}

bool colliding(Rocket rocket, Asteroid asteroid)
{
	return false;
}



void Game::CreateBackgroundBuffers() {

	const int nrOfVertices = 1000 * 5;

	GLfloat Vertices[nrOfVertices];
	GLfloat Colors[nrOfVertices];

	srand(time(NULL));
	int i = 0;
	while (i < nrOfStars) {
		Vertices[4 * i] = float(rand() % (4 * getWidth()) + 1) - 2 * getWidth();
		Vertices[4 * i + 1] = float(rand() % (4 * getHeight()) + 1) - 2 * getHeight();
		Vertices[4 * i + 2] = 0.f;
		Vertices[4 * i + 3] = 1.f;
		Colors[4 * i] = 1.f;
		Colors[4 * i + 1] = 1.f;
		Colors[4 * i + 2] = 1.f;
		Colors[4 * i + 3] = 1.f;
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
		790.f, 100.f, 0.f, 1.f,
		810.f, 100.f, 0.f, 1.f,
		825.f, 85.f, 0.f, 1.f, // dreapta
		800.f, 50.f, 0.f, 1.f, // varful de jos
		775.f, 85.f, 0.f, 1.f, // stanga

		// Focul galben
		790.f, 100.f, 0.f, 1.f,
		810.f, 100.f, 0.f, 1.f,
		815.f, 90.f, 0.f, 1.f,
		800.f, 70.f, 0.f, 1.f, // varful de jos
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


	GLfloat Square[] = {
		400.f, 000.f, 0.f, 1.f, // st jos
		800.f, 000.f, 0.f, 1.f, // dr jos
		800.f, 200.f, 0.f, 1.f, // dr sus,
		400.f, 200.f, 0.f, 1.f // st sus
	};

	GLfloat SquareColors[] = {
		1.f, 0.f, 1.f, 0.f,
		1.f, 0.f, 1.f, 0.f,
		1.f, 0.f, 1.f, 0.f,
		1.f, 0.f, 1.f, 0.f,
	};

	glGenBuffers(1, &squareVbo);
	glBindBuffer(GL_ARRAY_BUFFER, squareVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Square), Square, GL_STATIC_DRAW);

	glGenVertexArrays(1, &squareVbo);
	glBindVertexArray(squareVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &squareColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, squareColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SquareColors), SquareColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void Game::CreateAsteroidBuffers() {

	GLfloat Vertices[1000];
	GLfloat Colors[1000];
	for (int k = 0; k < Constants::nrOfVerticesPerCircle; k++) {
		float theta = Constants::TWO_PI * k / Constants::nrOfVerticesPerCircle;
		cout << "angle " << float(Constants::TWO_PI * float(k)) / float(Constants::nrOfVerticesPerCircle) << "\n";
		float x = cos(theta);
		float y = sin(theta);
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
	int count = 0;
	for (auto& asteroid : asteroids) {
		string previousState = asteroid->getCurrentZone();
		asteroid->updateState();
		string newState = asteroid->getCurrentZone();
		if (previousState != newState) {
			count++;
		}
	}
	Game::GenerateAsteroids(count);
}

float Game::generateOffset(float leftOffset, float rightOffset, static const string offsetType) {
	float offset;
	bool collides = true;
	srand(time(NULL));
	while (collides) {
		offset = leftOffset + rand() % ((int)(rightOffset - leftOffset + 1));
		collides = false;
		for (int i = 0; i < static_cast<int>(asteroids.size()) - 1 && !collides; i++) {
			if (abs(offset - (offsetType == Constants::xCoord ? asteroids[i]->getX() : asteroids[i]->getRealY())) < Constants::distanceBetweenAsteroids)
				collides = true;
		} 
	}
	return offset;
}

Asteroid* Game::GenerateSingleAsteroid() {
	float x_offset = Game::generateOffset(0, 2 * Constants::width, Constants::xCoord);
	float y_offset = Game::generateOffset(2 * Constants::height, 4 * Constants::height, Constants::yCoord);
	return new Asteroid(40.0, 20, glm::vec4(x_offset, y_offset, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Game::GenerateAsteroids(int nrOfAsteroids) {
	for (int i = 0; i < nrOfAsteroids; i++) {
		this->asteroids.push_back(Game::GenerateSingleAsteroid());
	}
}

void Game::CreateBulletBuffers() {

	GLfloat Vertices[1000];
	GLfloat Colors[1000];
	for (int k = 0; k < Constants::nrOfVerticesPerCircle; k++) {
		float theta = Constants::TWO_PI * (float)k / (float)Constants::nrOfVerticesPerCircle;
		float x = Constants::bulletRadius * cos(theta);
		float y = Constants::bulletRadius * sin(theta);

		Vertices[4 * k] = x;
		Vertices[4 * k + 1] = y;
		Vertices[4 * k + 2] = 0.0f;
		Vertices[4 * k + 3] = 1.0f;

		Colors[4 * k] = 0.0f;
		Colors[4 * k + 1] = 1.0f;
		Colors[4 * k + 2] = 0.0f;
		Colors[4 * k + 3] = 1.0f;
	}

	int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

	glGenBuffers(1, &bulletVbo);
	glBindBuffer(GL_ARRAY_BUFFER, bulletVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &bulletVao);
	glBindVertexArray(bulletVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &bulletColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void Game::GenerateBullet() {
	Rocket* rocket = Rocket::getInstance();
	double now = glfwGetTime();

	if (bullets.size() < Constants::nrOfBulletsPerFrame) {
		if (now - lastBulletTime > Constants::bulletSpawnCooldown) {
			lastBulletTime = glfwGetTime();
			bullets.push_back(new Bullet(Constants::bulletRadius, rocket->getBulletStartX(), rocket->getBulletStartY()));
		}
		else {
			cout << "Reloading\n";
		}
	}
	else {
		cout << "Fire Cooldown\n";
	}
}

void Game::UpdateBullets() {
	auto end = std::remove_if(bullets.begin(),
		bullets.end(),
		[](Bullet* const& i) {
			return i->aboveViewport();
		});

	bullets.erase(end, bullets.end());
}
