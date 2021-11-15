#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;

class Asteroid {
private:
	glm::vec4 coordinates;
	glm::vec4 colors;
	int nrOfVertices;
	float radius;
	float translatedDistance;
public:
	glm::mat4 asteroidMatrix;
	static glm::vec4 circlePoint;
	static glm::vec4 circleCenter;
public:
	Asteroid(float, int, glm::vec<4, float, (glm::qualifier)0>, glm::vec<4, float, (glm::qualifier)0>);
	~Asteroid() { };

	bool isInViewport();
	bool belowViewport();

	float getX();
	float getY();
	float getRadius();
	float getTranslatedDistance();
	glm::mat4 getAsteroidMatrix() { return asteroidMatrix; }

	void setX(float);
	void setY(float);
	void setTranslatedDistance(float);
	void setAsteroidMatrix(glm::mat4 matrix) { asteroidMatrix = matrix; }
};
