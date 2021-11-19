#include "Game.h"
#include "helpers.h"
#include "Constants.h"
#include "Asteroid.h"
#include "Rocket.h"

#include <ctime>
#include <vector>

#include "loadShaders.h"
#include "SOIL/SOIL.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>

vector<GLuint> Game::textures;

void Game::move(void)
{
	// when the left mouse is clicked, the game must start
	// so the asteroids and the bullets must start moving
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
	// when the left mouse button is clicked, the Game::move() function
	// is called through a callback function
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
	score(0),
	width(Constants::maxX),
	height(Constants::maxY),
	initPosX(initial_pos_x),
	initPosY(initial_pos_y)
{
	maxX = width / 2;
	maxY = height / 2;

	InitializeLibraries();
	
	// creating the buffers for each type of element to be rendered
	CreateBackgroundBuffers();
	CreateRocketBuffers();
	CreateAsteroidBuffers();
	CreateBulletBuffers();
	CreateHeartBuffers();
	CreateGameOverBuffers();

	// generating the first asteroids
	GenerateAsteroids(Constants::nrOfAsteroidsPerFrame);
}

void Game::InitializeLibraries() {
	// initializing: freeglut, glew, glfw
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

void Game::CreateShaders() {
	ProgramId = LoadShaders("vertShader.vert", "fragShader.frag");
	TextureProgramId = LoadShaders("textureShader.vert", "textureShader.frag");
	glUseProgram(ProgramId);
}

void Game::DestroyShaders(void) {
	glDeleteProgram(ProgramId);
	glDeleteProgram(TextureProgramId);
}

void Game::Cleanup(void) {
	DestroyShaders();
	DestroyVBO();
}

void Game::DestroyVBO(void) {
	glDisableVertexAttribArray(2);
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
	glDeleteBuffers(1, &squareColorBufferId);
	glDeleteBuffers(1, &squareVbo);
	glDeleteBuffers(1, &heartColorBufferId);
	glDeleteBuffers(1, &heartVbo);
	glDeleteBuffers(1, &gameOverVbo);

	glBindVertexArray(0);
}

void Game::InitializeGame() {
	CreateShaders();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	Game::loadTextures();
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
	float leftSideX = 775.0f - fireSides;
	float leftSideY = 85.0f - fireSides;
	float rightSideX = 825.0f + fireSides;
	float rightSideY = 85.0f - fireSides;
	float yellowTail = 70.0f + fireTail;
	glNamedBufferSubData(rocketVbo, 15 * 7 * sizeof(GLfloat), sizeof(GLfloat), &rightSideX);
	glNamedBufferSubData(rocketVbo, (15 * 7 + 1) * sizeof(GLfloat), sizeof(GLfloat), &rightSideY);
	glNamedBufferSubData(rocketVbo, (16 * 7 + 1) * sizeof(GLfloat), sizeof(GLfloat), &orangeTail);
	glNamedBufferSubData(rocketVbo, 17 * 7 * sizeof(GLfloat), sizeof(GLfloat), &leftSideX);
	glNamedBufferSubData(rocketVbo, (17 * 7 + 1) * sizeof(GLfloat), sizeof(GLfloat), &leftSideY);
	glNamedBufferSubData(rocketVbo, (21 * 7 + 1) * sizeof(GLfloat), sizeof(GLfloat), &yellowTail);
}

void Game::RenderFunction(void) {
	Rocket* rocket = Rocket::getInstance();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (rocket->getIsDead()) {
		// when the rocket dies, the texture programid must be used
		// in order to render the GAME OVER photo
		glUseProgram(TextureProgramId);
		myMatrixLocation = glGetUniformLocation(TextureProgramId, "myMatrix");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameOverTexture);

		glm::mat4 gameOverMatrix = backgroundMatrix;
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &gameOverMatrix[0][0]);
		glBindVertexArray(gameOverVao);
		glDrawArrays(GL_POLYGON, 0, 4);

		glFlush();
		return;
	}

	// reset the rotation angle of the background if is goes above an entire circle
	if (rotationAngle > 360.0f) {
		rotationAngle = 0.0f;
	}

	glUseProgram(ProgramId);

	rotationAngle += rotationSpeed;

	// we are scaling the [-1, 1]x[-1, 1] screen to a [0, 1600]x[0, 900] screen
	// and we are rotating the background with a rotation angle 
	backgroundScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / maxX, 1.0f / maxY, 1.0f));
	backgroundTranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0f));
	backgroundRotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, -1.0f));
	backgroundMatrix = backgroundScaleMatrix * backgroundTranslateMatrix * backgroundRotateMatrix;

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &backgroundMatrix[0][0]);

	// drawing the stars
	glPointSize(2.0);
	glBindVertexArray(backgroundVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEbo);
	glDrawArrays(GL_POINTS, 0, nrOfStars);	

	FireAnimation();

	// the position of the rocket is changed at keyboard input
	// so the rocket must be translated
	float posX = rocket->getPositionX();
	float posY = rocket->getPositionY();
	glm::mat4 rocketTranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, 0.0f));

	glm::mat4 matrix = backgroundScaleMatrix * backgroundTranslateMatrix * rocketTranslateMatrix;
	rocket->setRocketMatrix(matrix);

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &matrix[0][0]);

	// drawing the rocket
	glBindVertexArray(rocketVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_POLYGON, 6, 4);
	glDrawArrays(GL_TRIANGLES, 10, 3);
	glDrawArrays(GL_POLYGON, 13, 5);
	glDrawArrays(GL_POLYGON, 18, 5);	

	// resetting the background matrix so the other elements don't rotate
	backgroundMatrix = backgroundScaleMatrix * backgroundTranslateMatrix;

	Game::UpdateAsteroids();
	
	// using the texture programid so the asteroids have texture
	glUseProgram(TextureProgramId);
	myMatrixLocation = glGetUniformLocation(TextureProgramId, "myMatrix");
	
	glActiveTexture(GL_TEXTURE0);
	
	for (auto& asteroid : asteroids) {
		glBindTexture(GL_TEXTURE_2D, Game::textures[asteroid->getTextureIndex()]);
		
		glm::mat4 asteroidMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(asteroid->getRadius(), asteroid->getRadius(), 1.0));
		// moving the asteroids on the Oy axis
		glm::mat4 animateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, - asteroid->getTranslatedDistance(), 0.0));
		glm::mat4 asteroidTranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(asteroid->getX(), asteroid->getY(), 0.0));
		asteroidMatrix = backgroundMatrix *  animateMatrix * asteroidTranslateMatrix * asteroidMatrix;
		asteroid->setAsteroidMatrix(asteroidMatrix);

		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &asteroidMatrix[0][0]);
		
		// drawing the asteroid
		glBindVertexArray(asteroidVao);
		glDrawArrays(GL_POLYGON, 0, Constants::nrOfVerticesPerCircle);
	}	

	// reseting the programid so it doesn't use textures anymore
	glDisable(GL_TEXTURE_2D);
	glUseProgram(ProgramId);
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");

	Game::UpdateBullets();

	for (auto& bullet : bullets) {
		glm::mat4 bulletMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(bullet->getRadius(), bullet->getRadius(), 1.0f));
		// moving the bullets on the Oy axis
		glm::mat4 animateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, bullet->getY(), 0.0));
		bulletMatrix = backgroundMatrix * animateMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(bullet->getX(), bullet->getY(), 0.0)) * bulletMatrix;
		bullet->bulletMatrix = bulletMatrix;

		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &bulletMatrix[0][0]);
		
		// drawing the bullet
		glBindVertexArray(bulletVao);
		glDrawArrays(GL_POLYGON, 0, Constants::nrOfVerticesPerCircle);
	}	

	// checking rocket-asteroid collison
	rocket->RocketAsteroidsCollision(asteroids);

	// checking bullet-asteroid collision
	BulletAsteroidCollision();
	
	glm::mat4 heartMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4.7f, 4.7f, 1.0f));
	heartMatrix = backgroundMatrix * heartMatrix;

	glm::mat4 heartTranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
	for (int i = 0; i < rocket->getRemainingLives(); i++) {
		// translating the hearts starting with the second one
		if (i != 0) {
			heartMatrix = heartMatrix * heartTranslateMatrix;
		}
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &heartMatrix[0][0]);

		// drawing the heart
		glBindVertexArray(heartVao);
		glDrawArrays(GL_POLYGON, 0, 14);
	}

	glutPostRedisplay();
	glFlush();
}

void Game::CreateBackgroundBuffers() {

	const int nrOfVertices = 1000 * 5;

	GLfloat Vertices[nrOfVertices];
	GLfloat Colors[nrOfVertices];

	srand((unsigned int)time(NULL));
	int i = 0;
	while (i < nrOfStars) {
		Vertices[4 * i] = float(rand() % (4 * getWidth()) + 1) - 2 * getWidth();
		Vertices[4 * i + 1] = float(rand() % (4 * getHeight()) + 1) - 2 * getHeight();
		Vertices[4 * i + 2] = 0.0f;
		Vertices[4 * i + 3] = 1.0f;
		Colors[4 * i] = 1.0f;
		Colors[4 * i + 1] = 1.0f;
		Colors[4 * i + 2] = 1.0f;
		Colors[4 * i + 3] = 1.0f;
		i++;
	}

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
		// Top wings triangle
		rocket->topWingsTriangle.left[0], rocket->topWingsTriangle.left[1], 0.0f, 1.0f,           1.0f, 0.0f, 0.0f,
		rocket->topWingsTriangle.right[0], rocket->topWingsTriangle.right[1], 0.0f, 1.0f,         1.0f, 0.0f, 0.0f,
		rocket->topWingsTriangle.top[0], rocket->topWingsTriangle.top[1], 0.0f, 1.0f,             1.0f, 0.0f, 0.0f,

		// Bottom wings triangle
		rocket->bottomWingsTriangle.left[0], rocket->bottomWingsTriangle.left[1], 0.0f, 1.0f,       1.0f, 0.0f, 0.0f,
		rocket->bottomWingsTriangle.right[0], rocket->bottomWingsTriangle.right[1], 0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
		rocket->bottomWingsTriangle.top[0], rocket->bottomWingsTriangle.top[1], 0.0f, 1.0f,         1.0f, 0.0f, 0.0f,

		// Body rectangle
		rocket->body.bottomLeft[0], rocket->body.bottomLeft[1], 0.0f, 1.0f,                         0.8f, 0.5f, 0.2f,
		rocket->body.bottomRight[0], rocket->body.bottomRight[1], 0.0f, 1.0f,                       0.8f, 0.5f, 0.2f,
		rocket->body.topRight[0], rocket->body.topRight[1], 0.0f, 1.0f,                             0.8f, 0.5f, 0.2f,
		rocket->body.topLeft[0], rocket->body.topLeft[1], 0.0f, 1.0f,                               0.8f, 0.5f, 0.2f,

		// Top triangle
		rocket->frontTriangle.left[0], rocket->frontTriangle.left[1], 0.0f, 1.0f,                 1.0f, 1.0f, 1.0f,
		rocket->frontTriangle.right[0], rocket->frontTriangle.right[1], 0.0f, 1.0f,               1.0f, 1.0f, 1.0f,
		rocket->frontTriangle.top[0], rocket->frontTriangle.top[1], 0.0f, 1.0f,                   1.0f, 1.0f, 1.0f,
	
		// Orange fire
		790.0f, 100.0f, 0.0f, 1.0f,                         1.0f, 0.25f, 0.0f,
		810.0f, 100.0f, 0.0f, 1.0f,                         1.0f, 0.25f, 0.0f,
		825.0f, 85.0f, 0.0f, 1.0f,                          1.0f, 0.25f, 0.0f,
		800.0f, 50.0f, 0.0f, 1.0f,                          1.0f, 0.25f, 0.0f,
		775.0f, 85.0f, 0.0f, 1.0f,                          1.0f, 0.25f, 0.0f,

		// Yellow fire
		790.0f, 100.0f, 0.0f, 1.0f,                         1.0f, 0.8f, 0.0f,
		810.0f, 100.0f, 0.0f, 1.0f,                         1.0f, 0.8f, 0.0f,
		815.0f, 90.0f, 0.0f, 1.0f,                         1.0f, 0.8f, 0.0f,
		800.0f, 70.0f, 0.0f, 1.0f,                          1.0f, 0.8f, 0.0f,
		785.0f, 90.0f, 0.0f, 1.0f,                          1.0f, 0.8f, 0.0f,
	};

	glGenVertexArrays(1, &rocketVao);
	glGenBuffers(1, &rocketVbo);
	
	glBindVertexArray(rocketVao);

	glBindBuffer(GL_ARRAY_BUFFER, rocketVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));


	GLfloat Square[] = {
		400.0f, 000.0f, 0.0f, 1.0f, // down left
		800.0f, 000.0f, 0.0f, 1.0f, // down right
		800.0f, 200.0f, 0.0f, 1.0f, // up right
		400.0f, 200.0f, 0.0f, 1.0f // up left
	};

	GLfloat SquareColors[] = {
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
	};

	glGenBuffers(1, &squareVbo);
	glBindBuffer(GL_ARRAY_BUFFER, squareVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Square), Square, GL_STATIC_DRAW);

	glGenVertexArrays(1, &squareVao);
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
	for (int k = 0; k < Constants::nrOfVerticesPerCircle; k++) {
		// generating points on the circle
		float theta = Constants::TWO_PI * k / Constants::nrOfVerticesPerCircle;
		float x = cos(theta);
		float y = sin(theta);

		Vertices[9 * k] = x;
		Vertices[9 * k + 1] = y;
		Vertices[9 * k + 2] = 0.0f;
		Vertices[9 * k + 3] = 1.0f;

		Vertices[9 * k + 4] = 1.0f;
		Vertices[9 * k + 5] = 0.0f;
		Vertices[9 * k + 6] = 0.0f;
  
		Vertices[9 * k + 7] = (cos(theta) + 1) / 2;
		Vertices[9 * k + 8] = (sin(theta) + 1)/ 2;
	}
	
	glGenVertexArrays(1, &asteroidVao);
	glGenBuffers(1, &asteroidVbo);

	glBindVertexArray(asteroidVao);

	glBindBuffer(GL_ARRAY_BUFFER, asteroidVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

void Game::UpdateAsteroids() {
	// selecting the asteroids that are below viewport
	auto end = std::remove_if(asteroids.begin(),
		asteroids.end(),
		[](Asteroid* const& i) {
		return i->belowViewport();  
	});

	asteroids.erase(end, asteroids.end());

	// keeping the number of asteroids to a minimum
	while (asteroids.size() < Constants::nrOfAsteroidsPerFrame) {
		Game::GenerateAsteroids(1);
	}
}

float Game::generateOffset(float leftOffset, float rightOffset, const string offsetType) {
	float offset;
	bool collides = true;

	srand((unsigned int)time(NULL));

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
	float x_offset = Game::generateOffset(0.0f, 2.0f * Constants::width, Constants::xCoord);
	float y_offset = Game::generateOffset(2.0f * Constants::height, 4.0f * Constants::height, Constants::yCoord);
	return new Asteroid(40.0, 20, glm::vec4(x_offset, y_offset, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Game::GenerateAsteroids(int nrOfAsteroids) {
	for (int i = 0; i < nrOfAsteroids; i++) {
		this->asteroids.push_back(Game::GenerateSingleAsteroid());
	}
}

void Game::LoadTexture(GLuint &texture, const char* imageName)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image(imageName, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::loadTextures() {
	for (auto& imageName : Constants::textureImages) {
		GLuint texture;
		LoadTexture(texture, imageName);
		Game::textures.push_back(texture);
	}
	LoadTexture(gameOverTexture, Constants::gameOverTexture);
}

void Game::CreateBulletBuffers() {
	GLfloat Vertices[1000];
	GLfloat Colors[1000];

	Bullet::bulletCenter = { 0.0f, 0.0f, 0.0f, 1.0f };
	Bullet::bulletPoint = { Constants::bulletRadius, 0.0f, 0.0f, 1.0f };

	for (int k = 0; k < Constants::nrOfVerticesPerCircle; k++) {
		// generating points on the circle
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

	// stop generating bullets if the weapon is spammed
	// generate a new bullet only if a number of seconds have passed
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
	// remove the bullets if they went above the window
	auto end = std::remove_if(bullets.begin(),
		bullets.end(),
		[](Bullet* const& i) {
			return i->aboveViewport();
		});

	bullets.erase(end, bullets.end());
}

double distance(glm::vec4 p1, glm::vec4 p2) {
	return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2));
}

void Game::BulletAsteroidCollision() {
	// calculating the distance between each bullet and each asteroid
	// remove both at collision

	for (int i = 0; i < int(bullets.size()); i++) {
		for (int j = 0; j < int(asteroids.size()); j++) {
			glm::vec4 currentBulletCenter = bullets[i]->bulletMatrix * Bullet::bulletCenter;
			glm::vec4 currentBulletPoint = bullets[i]->bulletMatrix * Bullet::bulletPoint;

			glm::vec4 currentAsteroidCenter = asteroids[j]->asteroidMatrix * Asteroid::circleCenter;
			glm::vec4 currentAsteroidPoint = asteroids[j]->asteroidMatrix * Asteroid::circlePoint;

			double currentBulletRadius = sqrt(pow(currentBulletCenter[0] - currentBulletPoint[0], 2) + pow(currentBulletCenter[1] - currentBulletPoint[1], 2));
			double currentAsteroidRadius = sqrt(pow(currentAsteroidCenter[0] - currentAsteroidPoint[0], 2) + pow(currentAsteroidCenter[1] - currentAsteroidPoint[1], 2));

			if (distance(currentAsteroidCenter, currentBulletCenter) < currentBulletRadius + currentAsteroidRadius) {
				bullets[i]->setToBeDeleted(true);
				asteroids[j]->setToBeDeleted(true);
				score++;
			}
		}
	}

	auto end1 = std::remove_if(asteroids.begin(),
		asteroids.end(),
		[](Asteroid* const& i) {
		return i->getToBeDeleted();
	});

	asteroids.erase(end1, asteroids.end());

	auto end2 = std::remove_if(bullets.begin(),
		bullets.end(),
		[](Bullet* const& i) {
		return i->getToBeDeleted();
	});

	bullets.erase(end2, bullets.end());
}

void Game::CreateHeartBuffers() {
  
	GLfloat Vertices[] = {
		7.0f, 3.7f, 0.0f, 1.0f,
		
		8.0f, 4.5f, 0.0f, 1.0f,
		9.6f, 6.0f, 0.0f, 1.0f,
		10.4f, 7.5f, 0.0f, 1.0f,
		10.0f, 9.0f, 0.0f, 1.0f,
		8.7f, 9.8f, 0.0f, 1.0f,
		7.8f, 9.7f, 0.0f, 1.0f,
		
		7.0f, 8.9f, 0.0f, 1.0f,
		
		6.2f, 9.7f, 0.0f, 1.0f,
		5.3f, 9.8f, 0.0f, 1.0f,
		4.0f, 9.0f, 0.0f, 1.0f,
		3.6f, 7.5f, 0.0f, 1.0f,
		4.4f, 6.0f, 0.0f, 1.0f,
		6.0f, 4.5f, 0.0f, 1.0f,
	};

	const int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

	GLfloat Colors[verticesCount];

	for (int i = 0; i < verticesCount; i += 4) {
		Colors[i] = 1.0f;
		Colors[i + 1] = 0.0f;
		Colors[i + 2] = 0.0f;
		Colors[i + 3] = 1.0f;
	}

	glGenBuffers(1, &heartVbo);
	glBindBuffer(GL_ARRAY_BUFFER, heartVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &heartVao);
	glBindVertexArray(heartVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &heartColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, heartColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void Game::CreateGameOverBuffers() {
	GLfloat Vertices[] = {
		0.0f, 0.0f, 0.0f, 1.0f,								0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		Constants::maxX, 0.0f, 0.0f, 1.0f,					0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		Constants::maxX, Constants::maxY, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.0f, Constants::maxY, 0.0f, 1.0f,					0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	};

	glGenVertexArrays(1, &gameOverVao);
	glGenBuffers(1, &gameOverVbo);

	glBindVertexArray(gameOverVao);

	glBindBuffer(GL_ARRAY_BUFFER, gameOverVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}
