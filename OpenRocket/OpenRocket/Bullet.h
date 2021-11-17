#pragma once

class Bullet {
private:
	float radius;
	float translatedDistance;
	float x;
	float y;

public:
	Bullet(float radius, float posX, float posY);
	~Bullet();

	float getRadius() const {
		return radius;
	}

	float getX() const {
		return x;
	}

	float getY() const {
		return y;
	}

	float getTranslatedDistance() const {
		return translatedDistance;
	}

	void setX(float x) {
		this->x = x;
	}

	void setY(float y) {
		this->y = y;
	}

	void setTranslatedDistance(float distance) {
		translatedDistance = distance;
	}

	bool aboveViewport();

};