#pragma once

class Bullet {
private:
	float radius;
	float translatedDistance;
	float x;
	float y;

public:
	Bullet(float, float, float);
	~Bullet() {}

	float getRadius() const;
	float getX() const;
	float getY() const;
	float getTranslatedDistance() const;

	void setX(float);
	void setY(float);
	void setTranslatedDistance(float);

	bool aboveViewport();
};
