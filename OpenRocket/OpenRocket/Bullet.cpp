#include "Bullet.h"
#include "Constants.h"
#include <iostream>

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

glm::vec4 Bullet::bulletCenter = { 0.0f, 0.0f, 0.f, 1.f };
glm::vec4 Bullet::bulletPoint = { 0.0f, 0.0f, 0.f, 1.f };