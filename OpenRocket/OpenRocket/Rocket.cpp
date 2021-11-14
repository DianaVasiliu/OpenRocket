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
			positionX += 10;   
			cout << positionX << " " << positionY << "\n";
			break;
		case GLUT_KEY_LEFT:  
			positionX -= 10;   
			cout << positionX << " " << positionY << "\n";
			break;
		case GLUT_KEY_UP:    
			positionY += 10;  
			cout << positionX << " " << positionY << "\n"; 
			break;
		case GLUT_KEY_DOWN:  
			positionY -= 10;  
			cout << positionX << " " << positionY << "\n"; 
			break;
	}
}