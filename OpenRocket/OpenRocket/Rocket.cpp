#include "loadShaders.h"
#include "helpers.h"
#include "Constants.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

Rocket* Rocket::instance = nullptr;

Rocket::Rocket() {}

Rocket* Rocket::getInstance() {
	if (!instance) {
		instance = new Rocket();
	}
	return instance;
}

void Rocket::MoveRocket(int key, int x, int y) {
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
	}
}

void Rocket::MoveLeft() {
	if (positionX - moveAmount > -boundsOffsetX) {
		positionX -= moveAmount;
	}
}

void Rocket::MoveUp() {
	if (positionY + moveAmount < Constants::maxY - boundsOffsetY) {
		positionY += moveAmount;
	}
}

void Rocket::MoveDown() {
	if (positionY - moveAmount > -boundsOffsetY) {
		positionY -= moveAmount;
	}
}

void Rocket::MarkKeyDown(int key, int x, int y) {
	if (key == GLUT_KEY_RIGHT) {
		rightIsPressed = true;
		MoveRight();
		if (upIsPressed) 
		{
			MoveUp();
		}
		else if (downIsPressed) 
		{
			MoveUp();
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
