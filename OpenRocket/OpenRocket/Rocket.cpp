#include "loadShaders.h"
#include "helpers.h"
#include "Constants.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

Rocket* Rocket::instance = nullptr;

Rocket::Rocket() 
{
	frontTriangle.top = { 800.f, 210.f, 0.f, 1.f };
	frontTriangle.left = { 790.f, 175.f, 0.0f, 1.f };
	frontTriangle.right = { 810.f, 175.f, 0.0f, 1.f };

	topWingsTriangle.top = { 800.f, 185.f, 0.f, 1.f };
	topWingsTriangle.left = { 775.f, 160.f, 0.0f, 1.f };
	topWingsTriangle.right = { 825.f, 160.f, 0.f, 1.f };

	bottomWingsTriangle.top = { 800.f, 165.f, 0.f, 1.f };
	bottomWingsTriangle.left = { 775.f, 100.f, 0.f, 1.f };
	bottomWingsTriangle.right = { 825.f, 100.f, 0.f, 1.f };

	body.topLeft = { 790.f, 175.f, 0.f, 1.f };
	body.topRight = { 810.f, 175.f, 0.f, 1.f };
	body.bottomLeft = { 790.f, 100.f, 0.f, 1.f };
	body.bottomRight = { 810.f, 100.f, 0.f, 1.f };
}

Rocket* Rocket::getInstance() {
	if (!instance) {
		instance = new Rocket();
	}
	return instance;
}

void Rocket::MarkKeyUp(int key, int x, int y) {
	if (key == GLUT_KEY_RIGHT && rightIsPressed) {
		rightIsPressed = false;
	} 
	if (key == GLUT_KEY_LEFT && leftIsPressed) {
		leftIsPressed = false;
	}
	if (key == GLUT_KEY_UP && upIsPressed) {
		upIsPressed = false;
	}
	if (key == GLUT_KEY_DOWN && downIsPressed) {
		downIsPressed = false;
	}
}

void Rocket::MoveRight() {
	if (positionX + moveAmount < Constants::maxX - boundsOffsetX) {
		positionX += moveAmount;
		bulletStartX += moveAmount;
	}
}

void Rocket::MoveLeft() {
	if (positionX - moveAmount > -boundsOffsetX) {
		positionX -= moveAmount;
		bulletStartX -= moveAmount;
	}
}

void Rocket::MoveUp() {
	if (positionY + moveAmount < Constants::maxY - boundsOffsetY) {
		positionY += moveAmount;
		bulletStartY += moveAmount / 2;
	}
}

void Rocket::MoveDown() {
	if (positionY - moveAmount > -boundsOffsetY) {
		positionY -= moveAmount;
		bulletStartY -= moveAmount / 2;
	}
}

void Rocket::MarkKeyDown(int key, int x, int y) {
	if (key == GLUT_KEY_RIGHT) { // tocmai s-a apasat tasta RIGHT
		rightIsPressed = true;	 // marchez aceasta tasta ca pressed
		MoveRight();			 // mut racheta catre dreapta
		if (upIsPressed) 
		{
			MoveUp();			 // daca tasta Up era deja apasata, mut racheta si in sus
		}
		else if (downIsPressed) 
		{
			MoveDown(); // daca tasta Down era deja apasata, mut racheta si in jos
		}
	}
	if (key == GLUT_KEY_LEFT) {
		MoveLeft();
		leftIsPressed = true;
		if (upIsPressed)
		{
			MoveUp();
		}
		else if (downIsPressed)
		{
			MoveDown();
		}
	}
	if (key == GLUT_KEY_UP) {
		MoveUp();
		upIsPressed = true;
		if (rightIsPressed)
		{
			MoveRight();
		}
		else if (leftIsPressed)
		{
			MoveLeft();
		}
	}
	if (key == GLUT_KEY_DOWN) {
		MoveDown();
		downIsPressed = true;
		if (rightIsPressed)
		{
			MoveRight();
		}
		else if (leftIsPressed)
		{
			MoveLeft();
		}
	}
}

float dist(glm::vec4 pt, glm::vec4 p1, glm::vec4 p2)
{
	float dx = p2[0] - p1[0];
	float dy = p2[1] - p1[1];
	if ((dx == 0) && (dy == 0))
	{
		// It's a point not a line segment.
		dx = pt[0] - p1[0];
		dy = pt[1] - p1[1];
		return sqrt(dx * dx + dy * dy);
	}

	// Calculate the t that minimizes the distance.
	float t = ((pt[0] - p1[0]) * dx + (pt[1] - p1[1]) * dy) / (dx * dx + dy * dy);

	// See if this represents one of the segment's
	// end points or a point in the middle.
	glm::vec4 closest;
	if (t < 0)
	{
		closest = p1;
		dx = pt[0] - p1[0];
		dy = pt[1] - p1[1];
	}
	else if (t > 1)
	{
		closest = p2;
		dx = pt[0] - p2[0];
		dy = pt[1] - p2[1];
	}
	else
	{
		closest = { p1[0] + t * dx, p1[1] + t * dy, 0.f, 1.f };
		dx = pt[0] - closest[0];
		dy = pt[1] - closest[1];
	}

	//cout << sqrt(dx * dx + dy * dy) << "\n";
	return sqrt(dx * dx + dy * dy);
}


void Rocket::CalculateCurrentPositions() {

	currentFrontTriangle.left = rocketMatrix * frontTriangle.left;
	currentFrontTriangle.right = rocketMatrix * frontTriangle.right;
	currentFrontTriangle.top = rocketMatrix * frontTriangle.top;

	currentTopWingsTriangle.left = rocketMatrix * topWingsTriangle.left;
	currentTopWingsTriangle.right = rocketMatrix * topWingsTriangle.right;
	currentTopWingsTriangle.top = rocketMatrix * topWingsTriangle.top;

	currentBottomWingsTriangle.left = rocketMatrix * bottomWingsTriangle.left;
	currentBottomWingsTriangle.right = rocketMatrix * bottomWingsTriangle.right;
	currentBottomWingsTriangle.top = rocketMatrix * bottomWingsTriangle.top;

	currentBody.topLeft = rocketMatrix * body.topLeft;
	currentBody.topRight = rocketMatrix * body.topRight;
	currentBody.bottomRight = rocketMatrix * body.bottomRight;
	currentBody.bottomLeft = rocketMatrix * body.bottomLeft;
}


void Rocket::RocketAsteroidsCollision(vector<Asteroid*> asteroids) 
{
	for (int i = 0; i < int(asteroids.size()); i++) {
		// For each asteroid calculate the current position of the center and a point on the circle.
		// For this, I use the initial position of the point and the transform matrix applied to the asteroid.
		glm::vec4 currentCenter = asteroids[i]->asteroidMatrix * Asteroid::circleCenter;
		glm::vec4 currentPoint = asteroids[i]->asteroidMatrix * Asteroid::circlePoint;
		// I then get the current radius of the circle by getting the distance between these points.
		double radius = sqrt(pow(currentCenter[0] - currentPoint[0], 2) + pow(currentCenter[1] - currentPoint[1], 2));
		CalculateCurrentPositions();

		// I then calculate the distance between each line segment of the rocket and the asteroid.
		bool condition1 = dist(currentCenter, currentFrontTriangle.left, currentFrontTriangle.right) < radius;
		bool condition2 = dist(currentCenter, currentFrontTriangle.top, currentFrontTriangle.right) < radius;
		bool condition3 = dist(currentCenter, currentFrontTriangle.top, currentFrontTriangle.left) < radius;

		bool condition4 = dist(currentCenter, currentTopWingsTriangle.left, currentTopWingsTriangle.right) < radius;
		bool condition5 = dist(currentCenter, currentTopWingsTriangle.top, currentTopWingsTriangle.right) < radius;
		bool condition6 = dist(currentCenter, currentTopWingsTriangle.top, currentTopWingsTriangle.left) < radius;

		bool condition7 = dist(currentCenter, currentBottomWingsTriangle.left, currentBottomWingsTriangle.right) < radius;
		bool condition8 = dist(currentCenter, currentBottomWingsTriangle.top, currentBottomWingsTriangle.right) < radius;
		bool condition9 = dist(currentCenter, currentBottomWingsTriangle.top, currentBottomWingsTriangle.left) < radius;

		bool condition10 = dist(currentCenter, currentBody.topLeft, currentBody.topRight) < radius;
		bool condition11 = dist(currentCenter, currentBody.bottomLeft, currentBody.bottomRight) < radius;
		bool condition12 = dist(currentCenter, currentBody.topLeft, currentBody.bottomLeft) < radius;
		bool condition13 = dist(currentCenter, currentBody.topRight, currentBody.bottomRight) < radius;
		if (condition1 || condition2 || condition3 || condition4 || condition5 || condition6 || 
			condition7 || condition8 || condition9 || condition10 || condition11 || condition12 )
		{
			cout << "coliziunee\n";
			isDead = true;
		}

	}
}
