#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Constants.h"

using namespace std;

class Game {
private:
	static Game* instance;

	GLuint backgroundVao;
	GLuint backgroundVbo;
	GLuint backgroundEbo;
	GLuint backgroundColorBufferId;

	GLuint rocketVao;
	GLuint rocketVbo;
	GLuint rocketColorBufferId;

	GLuint asteroidVao;
	GLuint asteroidVbo;

	int width;
	int height;

	GLuint ProgramId;

	int nrOfStars;

	int colorCode;
	int init_pos_x;
	int init_pos_y;
	int maxX;
	int maxY;

	GLuint myMatrixLocation;
	GLuint matrScaleLocation;
	GLuint matrTranslLocation;
	GLuint matrRotlLocation;
	GLuint codColLocation;
	glm::mat4 myMatrix;
	glm::mat4 resizeMatrix;
	glm::mat4 matrTransl;
	glm::mat4 matrScale;
	glm::mat4 matrRot;
	glm::mat4 mTest;

public:
	Game(int window_width, int window_height, int initial_pos_x, int initial_pos_y);
	~Game();

	static Game* getInstance();

	void setHeight(int h) { height = h; }
	void setWidth(int w) { width = w; }
	void setInitPosX(int x) { init_pos_x = x; }
	void setInitPosY(int y) { init_pos_y = y; }

	int getHeight() { return height; }
	int getWidth() { return width; }
	int getInitPosX() { return init_pos_x; }
	int getInitPosY() { return init_pos_y; }

	void CreateBackgroundBuffers();
	void CreateRocketBuffers();
	void InitializeGlew();
	void CreateShaders(const char* vertShader, const char* fragShader);
	void DestroyShaders(void);
	void Cleanup(void);
	void DestroyBackgroundVBO(void);

	void InitializeGame(const char* vertShader, const char* fragShader);
	void RenderFunction(void);
};