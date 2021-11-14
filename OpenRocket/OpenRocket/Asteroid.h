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
	Asteroid(float, int, glm::vec4, glm::vec4);
	bool isInViewport();
	bool belowViewport();
	float getX();
	void setX(float);
	void setY(float);
	float getTranslatedDistance();
	void setTranslatedDistance(float);
	float getRadius();
	float getY();
	~Asteroid() { };
};
