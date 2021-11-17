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
