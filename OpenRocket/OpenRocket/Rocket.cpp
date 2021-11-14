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
	switch (key)
	{
		case GLUT_KEY_RIGHT:
			if (positionX + 10 < Constants::maxX - boundsOffsetX) {
				positionX += 10;
			}
			//cout << positionX << " " << positionY << "\n";
			break;
		case GLUT_KEY_LEFT:  
			if (positionX - 10 > -boundsOffsetX) {
				positionX -= 10;
			}
			//cout << positionX << " " << positionY << "\n";
			break;
		case GLUT_KEY_UP:    
			if (positionY + 10 < Constants::maxY - boundsOffsetY) {
				positionY += 10;
			}
			//cout << positionX << " " << positionY << "\n"; 
			break;
		case GLUT_KEY_DOWN:  
			if (positionY - 10 > -boundsOffsetY) {
				positionY -= 10;
			}
			//cout << positionX << " " << positionY << "\n"; 
			break;
	}
}