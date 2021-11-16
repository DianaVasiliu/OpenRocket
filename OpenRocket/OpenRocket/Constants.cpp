#include "Constants.h"

const char* Constants::title = "Open rocket";
const float Constants::PI = 3.141592;
const int Constants::width = 800;
const int Constants::height = 450;
const int Constants::nrOfStars = 2000;
const int Constants::maxX = 1600;
const int Constants::maxY = 900;
const int Constants::nrOfAsteroids = 20;
const int Constants::nrOfAsteroidsPerFrame = 10;
const float Constants::TWO_PI = 6.28;
const int Constants::nrOfVerticesPerCircle = 18;
const float Constants::distanceBetweenAsteroids = 150.0f;
const float Constants::asteroidMovingUnit = 0.15f;
const float Constants::maxXDistance = 4 * Constants::width;
const float Constants::maxYDistance = 4 * Constants::height;
const std::string Constants::xCoord = "x";
const std::string Constants::yCoord = "y";
const std::string Constants::RED = "Red Zone";
const std::string Constants::SAFE= "Safe Zone";
const std::vector<const char*> Constants::textureImages{
	"blueAsteroid.png",
	"brightOrangeAsteroid.png",
	"orangeAsteroid.png",
	"turquoiseAsteroid.png"
};
const int Constants::nrOfTextures = 4;