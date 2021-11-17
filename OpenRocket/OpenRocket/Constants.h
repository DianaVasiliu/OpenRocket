#pragma once
#include <string>

class Constants
{
public:
    static const int width;
    static const int height; 
    static const int maxX;
    static const int maxY;
	static const char* title;
	static const float PI;
    static const int nrOfStars;
    static const int nrOfAsteroids;
    static const int nrOfAsteroidsPerFrame;
    static const float TWO_PI;
    static const int nrOfVerticesPerCircle;
    static const float distanceBetweenAsteroids;
    static const float asteroidMovingUnit;
    static const float maxXDistance;
    static const float maxYDistance;
    static const std::string xCoord;
    static const std::string yCoord;
    static const std::string RED;
    static const std::string SAFE;
    static const float bulletSpeed;
    static const double bulletSpawnCooldown;
    static const float bulletRadius;
    static const int nrOfBulletsPerFrame;
    static const int maxLives;
    static const double timeToEscapeAsteroid;
};
