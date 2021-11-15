
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
	float positionX = 0;
	float positionY = 0;
	float boundsOffsetX = 800;
	float boundsOffsetY = 135;

	bool rightIsPressed = false;
	bool leftIsPressed = false;
	bool upIsPressed = false;
	bool downIsPressed = false;

	float moveAmount = 20;

	bool isDead = false;

public:
	Triangle frontTriangle;
	Triangle topWingsTriangle;
	Triangle bottomWingsTriangle;
	Rect body;

	Triangle currentFrontTriangle;
	Triangle currentTopWingsTriangle;
	Triangle currentBottomWingsTriangle;
	Rect currentBody;

public:
    Rocket();
    static Rocket* getInstance();

	glm::mat4 getRocketMatrix() { return rocketMatrix; }
	float getPositionX() { return positionX; }
	float getPositionY() { return positionY; }

	void setRocketMatrix(glm::mat4 matrix) { rocketMatrix = matrix; }
	void RocketAsteroidsCollision(vector<Asteroid*> asteroids);
	void CalculateCurrentPositions();

	void Update();
	void MarkKeyUp(int key, int x, int y);
	void MarkKeyDown(int key, int x, int y);
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
};