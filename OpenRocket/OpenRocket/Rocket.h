
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <ctime>
#include <chrono>
#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Constants.h"

class Rocket {
private:
    static Rocket* instance;
    
	glm::mat4 rocketMatrix = glm::mat4(1.0f);
	glm::mat4 rocketScaleMatrix = glm::mat4(1.0f);
	glm::mat4 rocketRotateMatrix = glm::mat4(1.0f);
	glm::mat4 rocketTranslateMatrix = glm::mat4(1.0f);

	float positionX = 0;
	float positionY = 0;
	float boundsOffsetX = 800;
	float boundsOffsetY = 135;

	bool rightIsPressed = false;
	bool leftIsPressed = false;
	bool upIsPressed = false;
	bool downIsPressed = false;

	float moveAmount = 20;

	std::chrono::time_point<std::chrono::system_clock> rightPressTime;
	std::chrono::time_point<std::chrono::system_clock> leftPressTime;
	std::chrono::time_point<std::chrono::system_clock> upPressTime;
	std::chrono::time_point<std::chrono::system_clock> downPressTime;

public:
    Rocket();
    static Rocket* getInstance();

	glm::mat4 getRocketMatrix() { return rocketMatrix; }
	glm::mat4 getRocketScaleMatrix() { return rocketScaleMatrix; }
	glm::mat4 getRocketTranslateMatrix() { return rocketTranslateMatrix; }
	glm::mat4 getRocketRotationMatrix() { return rocketRotateMatrix; }
	float getPositionX() { return positionX; }
	float getPositionY() { return positionY; }

	void Update();
	void MarkKeyUp(int key, int x, int y);
	void MarkKeyDown(int key, int x, int y);
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
};