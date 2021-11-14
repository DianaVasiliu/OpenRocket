#include "Asteroid.h"
#include "Constants.h"

Asteroid::Asteroid(float radius = 40.0f, int nrOfVertices = 16, glm::vec4 coordinates = glm::vec4(0.0f,0.0f, 0.0f, 0.0f), 
	glm::vec4 colors = glm::vec4(0.0f, 0.0f,0.0f,0.0f)) : radius(radius), nrOfVertices(nrOfVertices), coordinates(coordinates), colors(colors), translatedDistance(0) {
};
bool Asteroid::isInViewport() {
	return this->coordinates.y - this->translatedDistance < Constants::height && !this->belowViewport();
}
bool Asteroid::belowViewport() {
	return this->coordinates.y - this->translatedDistance < - this->radius;
}
float Asteroid::getX() { return this->coordinates.x; }
void Asteroid::setX(float newXOffset) { this->coordinates.x = newXOffset; }
void Asteroid::setY(float newYOffset) { this->coordinates.y = newYOffset; }
float Asteroid::getTranslatedDistance() { return this->translatedDistance; }
void Asteroid::setTranslatedDistance(float distance) { this->translatedDistance = distance; }
float Asteroid::getRadius() { return this->radius; }
float Asteroid::getY() { return this->coordinates.y; }