#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet(float radius, float posX, float posY) :
	radius(radius),
	x(posX),
	y(posY),
	translatedDistance(0)
{
}

bool Bullet::aboveViewport() {
	return this->y > Constants::height;
}

glm::vec4 Bullet::bulletCenter = { 0.0f, 0.0f, 0.0f, 1.0f };
glm::vec4 Bullet::bulletPoint = { 0.0f, 0.0f, 0.0f, 1.0f };

float Bullet::getRadius() const {
	return radius;
}

float Bullet::getX() const {
	return x;
}

float Bullet::getY() const {
	return y;
}

float Bullet::getTranslatedDistance() const {
	return translatedDistance;
}

void Bullet::setX(float x) {
	this->x = x;
}

void Bullet::setY(float y) {
	this->y = y;
}

void Bullet::setTranslatedDistance(float distance) {
	translatedDistance = distance;
}
