#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Constants.h"
#include "Asteroid.h"

struct Point {
	float x;
	float y;
};

struct Triangle {
	glm::vec4 top;
	glm::vec4 left;
	glm::vec4 right;
};

struct Rect {
	glm::vec4 topLeft;
	glm::vec4 topRight;
	glm::vec4 bottomLeft;
	glm::vec4 bottomRight;
};

class Rocket {
private:
    static Rocket* instance;
    
	glm::mat4 rocketMatrix;
	float positionX = 0.0f;
	float positionY = 0.0f;
	float boundsOffsetX = 800.0f;
	float boundsOffsetY = 135.0f;
	const float initialOffsetX = 800.0f;
	const float initialOffsetY = 100.0f;
	float bulletStartX = 800.0f;
	float bulletStartY = 100.0f;

	bool rightIsPressed = false;
	bool leftIsPressed = false;
	bool upIsPressed = false;
	bool downIsPressed = false;

	float moveAmount = 20.0f;

	int remainingLives;
	bool isDead = false;
	double lastCollisionTime;

public:
	Triangle frontTriangle;
	Triangle topWingsTriangle;
	Triangle bottomWingsTriangle;
	Rect body;

	Triangle currentFrontTriangle;
	Triangle currentTopWingsTriangle;
	Triangle currentBottomWingsTriangle;
	Rect currentBody;

    Rocket();
    static Rocket* getInstance();

	float getIsDead() const { return isDead; }
	float getPositionX() const { return positionX; }
	float getPositionY() const { return positionY; }
	float getBulletStartX() const { return bulletStartX; }
	float getBulletStartY() const {	return bulletStartY; }
	int getRemainingLives() const { return remainingLives; }

	void setBulletStartX(float x) {	bulletStartX = x; }
	void setBulletStartY(float y) {	bulletStartY = y; }
	void setRocketMatrix(glm::mat4 matrix) { rocketMatrix = matrix; }

	void RocketAsteroidsCollision(vector<Asteroid*>);
	void CalculateCurrentPositions();

	void Update();
	void MarkKeyUp(int, int, int);
	void MarkKeyDown(int, int, int);
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
};
