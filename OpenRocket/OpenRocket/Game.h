#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Rocket.h"

#include <vector>
#include <algorithm>
#include <string>
#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Constants.h"
#include "Asteroid.h"
#include "Bullet.h"

using namespace std;

class Game {
private:
	static Game* instance;
	GLuint backgroundVao;
	GLuint backgroundVbo;
	GLuint backgroundEbo;
	GLuint backgroundColorBufferId;
	glm::mat4 backgroundMatrix = glm::mat4(1.0f);
	glm::mat4 backgroundScaleMatrix = glm::mat4(1.0f);
	glm::mat4 backgroundRotateMatrix = glm::mat4(1.0f);
	glm::mat4 backgroundTranslateMatrix = glm::mat4(1.0f);
	float rotationAngle = 0.0f;
	float rotationSpeed = 0.0005f;

	GLfloat RocketVertices[1000];
	GLuint rocketVao;
	GLuint rocketVbo;
	GLuint rocketColorBufferId;
	glm::mat4 rocketMatrix = glm::mat4(1.0f);
	glm::mat4 rocketScaleMatrix = glm::mat4(1.0f);
	glm::mat4 rocketRotateMatrix = glm::mat4(1.0f);
	glm::mat4 rocketTranslateMatrix = glm::mat4(1.0f);

	float fireTailVelocity = 0.0075;
	float fireSidesVelocity = fireTailVelocity / 4;
	float resetTailEvery = 10.0;
	float resetSidesEvery = resetTailEvery / 4;
	float fireTail = 0.0;
	float fireSides = 0.0;
	float fireGoingUp = true;
	float fireGoingDown = false;

	GLuint asteroidVao;
	GLuint asteroidVbo;
	GLuint asteroidColorBufferId;

	GLuint bulletVao;
	GLuint bulletVbo;
	GLuint bulletColorBufferId;
	double lastBulletTime = 0.0f;

	int width;
	int height;

	GLuint ProgramId;

	int nrOfStars;

	int colorCode;
	int init_pos_x;
	int init_pos_y;
	int maxX;
	int maxY;

	GLuint myMatrixLocation;
	GLuint matrScaleLocation;
	GLuint matrTranslLocation;
	GLuint matrRotlLocation;
	GLuint codColLocation;
	glm::mat4 myMatrix;
	glm::mat4 resizeMatrix;
	glm::mat4 matrTransl;
	glm::mat4 matrScale;
	glm::mat4 matrRot;
	glm::mat4 mTest;

public:
	Game(int initial_pos_x, int initial_pos_y);
	~Game();
	vector<Asteroid*> asteroids;
	vector<Bullet*> bullets;
	static Game* getInstance();

	void setHeight(int h) { height = h; }
	void setWidth(int w) { width = w; }
	void setInitPosX(int x) { init_pos_x = x; }
	void setInitPosY(int y) { init_pos_y = y; }

	int getHeight() { return height; }
	int getWidth() { return width; }
	int getInitPosX() { return init_pos_x; }
	int getInitPosY() { return init_pos_y; }

	void CreateBackgroundBuffers();
	void CreateRocketBuffers();
	void CreateAsteroidBuffers();
	void CreateBulletBuffers();

	void moveRocket(int key, int x, int y);

	void InitializeGlew();
	void CreateShaders(const char* vertShader, const char* fragShader);
	void DestroyShaders(void);
	void Cleanup(void);
	void DestroyVBO(void);

	void InitializeGame(const char* vertShader, const char* fragShader);
	void RenderFunction(void);

	void GenerateAsteroids(int nrOfAsteroids);
	Asteroid* GenerateSingleAsteroid();
	void UpdateAsteroids();
	float generateOffset(float, float, static const string);

	void move();
	void mouseHandler(int, int, int, int);
	void FireAnimation();

	void UpdateBullets();
	void GenerateBullet();
};