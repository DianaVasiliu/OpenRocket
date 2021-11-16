#include "Asteroid.h"
#include "Constants.h"

glm::vec4 Asteroid::circlePoint = { 1.0f, 0.0f, 0.f, 1.f };
glm::vec4 Asteroid::circleCenter = { 0.f, 0.0f, 0.f, 1.f };

Asteroid::Asteroid(float radius = 40.0f, int nrOfVertices = 16, glm::vec4 coordinates = glm::vec4(0.0f,0.0f, 0.0f, 0.0f), 
	glm::vec4 colors = glm::vec4(0.0f, 0.0f,0.0f,0.0f)) : 
	radius(radius), 
	nrOfVertices(nrOfVertices), 
	coordinates(coordinates), 
	colors(colors), 
	translatedDistance(0), 
	currentZone(Constants::SAFE) {
};

bool Asteroid::isInViewport() {
	return this->coordinates.y - this->translatedDistance < Constants::height && !this->belowViewport();
}

bool Asteroid::belowViewport() {
	return this->coordinates.y - this->translatedDistance < - this->radius;
}

bool Asteroid::inLowerHalf() {
	return !this->belowViewport() && this->isInViewport() && this->getRealY() < Constants::height;
}

string Asteroid::getCurrentZone() {
	return this->currentZone;
}

void Asteroid::setCurrentZone(string newZone) {
	this->currentZone = newZone;
}

void Asteroid::updateState() {
	if (Asteroid::inLowerHalf()) {
		Asteroid::setCurrentZone(Constants::RED);
	}
}

float Asteroid::getX() { 
	return this->coordinates.x; 
}

float Asteroid::getRealY() { 
	return this->coordinates.y - this->translatedDistance; 
}

void Asteroid::setX(float newXOffset) { 
	this->coordinates.x = newXOffset; 
}

void Asteroid::setY(float newYOffset) { 
	this->coordinates.y = newYOffset;
}

float Asteroid::getTranslatedDistance() { 
	return this->translatedDistance; 
}

void Asteroid::setTranslatedDistance(float distance) { 
	this->translatedDistance = distance; 
}

float Asteroid::getRadius() { 
	return this->radius; 
}

float Asteroid::getY() { 
	return this->coordinates.y;
}
