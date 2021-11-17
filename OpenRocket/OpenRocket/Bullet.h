#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

class Bullet {
private:
	float radius;
	float translatedDistance;
	float x;
	float y;
	bool toBeDeleted = false;
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

	bool getToBeDeleted() { return toBeDeleted; }

	void setX(float x) {
		this->x = x;
	}

	void setY(float y) {
		this->y = y;
	}

	void setTranslatedDistance(float distance) {
		translatedDistance = distance;
	}

	void setToBeDeleted(bool t) { toBeDeleted = t; }

	bool aboveViewport();
	static glm::vec4 bulletCenter;
	static glm::vec4 bulletPoint;
	glm::mat4 bulletMatrix;
};