#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
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
	static vector<GLuint> textures;

	GLfloat RocketVertices[1000];
	glm::mat4 backgroundMatrix = glm::mat4(1.0f);
	glm::mat4 backgroundScaleMatrix = glm::mat4(1.0f);
	glm::mat4 backgroundRotateMatrix = glm::mat4(1.0f);
	glm::mat4 backgroundTranslateMatrix = glm::mat4(1.0f);

	GLuint backgroundVao;
	GLuint backgroundVbo;
	GLuint backgroundEbo;
	GLuint backgroundColorBufferId;

	GLuint rocketVao;
	GLuint rocketVbo;
	GLuint rocketEbo;
	GLuint rocketColorBufferId;

	GLuint asteroidVao;
	GLuint asteroidVbo;
	GLuint asteroidEbo;
	GLuint asteroidColorBufferId;

	GLuint bulletVao;
	GLuint bulletVbo;
	GLuint bulletColorBufferId;

	GLuint squareVao;
	GLuint squareVbo;
	GLuint squareColorBufferId;

	GLuint heartVao;
	GLuint heartVbo;
	GLuint heartColorBufferId;

	glm::mat4 rocketMatrix = glm::mat4(1.0f);
	glm::mat4 rocketScaleMatrix = glm::mat4(1.0f);
	glm::mat4 rocketRotateMatrix = glm::mat4(1.0f);
	glm::mat4 rocketTranslateMatrix = glm::mat4(1.0f);

	float fireTailVelocity = 0.0075f;
	float fireSidesVelocity = fireTailVelocity / 4;
	float resetTailEvery = 10.0f;
	float resetSidesEvery = resetTailEvery / 4;
	float fireTail = 0.0f;
	float fireSides = 0.0f;
	bool fireGoingUp = true;
	bool fireGoingDown = false;

	double lastBulletTime = 0.0f;

	float rotationAngle = 0.0f;
	float rotationSpeed = 0.0005f;

	int width;
	int height;
	int codCol;

	GLuint ProgramId;
	GLuint TextureProgramId;

	int nrOfStars;

	int colorCode;
	int initPosX;
	int initPosY;
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
	Game(int, int);
	~Game() { }

	vector<Asteroid*> asteroids;
	vector<Bullet*> bullets;
	static Game* getInstance();

	void setHeight(int h) { height = h; }
	void setWidth(int w) { width = w; }
	void setInitPosX(int x) { initPosX = x; }
	void setInitPosY(int y) { initPosY = y; }

	int getHeight() const { return height; }
	int getWidth() const { return width; }
	int getInitPosX() const { return initPosX; }
	int getInitPosY() const { return initPosY; }

	void CreateBackgroundBuffers();
	void CreateRocketBuffers();
	void CreateAsteroidBuffers();
	void CreateBulletBuffers();
	void CreateHeartBuffers();

	void InitializeLibraries();
	void CreateShaders();
	void DestroyShaders(void);
	void Cleanup(void);
	void DestroyVBO(void);

	void InitializeGame();
	void RenderFunction(void);

	void GenerateAsteroids(int);
	Asteroid* GenerateSingleAsteroid();
	void UpdateAsteroids();
	float generateOffset(float, float, const string);

	void move();
	void mouseHandler(int, int, int, int);
	void FireAnimation();

	void UpdateBullets();
	void GenerateBullet();
  
	void LoadTexture(GLuint&, const char*);
	void loadTextures();
};
