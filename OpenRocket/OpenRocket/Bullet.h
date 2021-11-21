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
	Bullet(float, float, float);
	~Bullet() {}

	float getRadius() const;
	float getX() const;
	float getY() const;
	float getTranslatedDistance() const;  
	bool getToBeDeleted() const;

	void setX(float);
	void setY(float);
	void setTranslatedDistance(float);
	void setToBeDeleted(bool);

	static glm::vec4 bulletCenter;
	static glm::vec4 bulletPoint;
	glm::mat4 bulletMatrix;
	bool aboveViewport();
};
