
#include <GL/glew.h>
#include <GL/freeglut.h>

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

public:
    Rocket();
    static Rocket* getInstance();

	glm::mat4 getRocketMatrix() { return rocketMatrix; }
	glm::mat4 getRocketScaleMatrix() { return rocketScaleMatrix; }
	glm::mat4 getRocketTranslateMatrix() { return rocketTranslateMatrix; }
	glm::mat4 getRocketRotationMatrix() { return rocketRotateMatrix; }
	float getPositionX() { return positionX; }
	float getPositionY() { return positionY; }

	void multRocketScaleMatrix(glm::mat4 matrix) { 
        rocketScaleMatrix = matrix * rocketScaleMatrix; 
		updateRocketMatrix();
    }
	
    void multRocketRotateMatrix(glm::mat4 matrix) { 
        rocketRotateMatrix = matrix * rocketRotateMatrix; 
		updateRocketMatrix();
    }
	
    void multRocketTranslateMatrix(glm::mat4 matrix) { 
        rocketTranslateMatrix = rocketTranslateMatrix; 
		updateRocketMatrix();
    }

    void updateRocketMatrix() { 
        rocketMatrix = rocketScaleMatrix * rocketTranslateMatrix;
    }

	void Update();
	void MoveRocket(int key, int x, int y);
};