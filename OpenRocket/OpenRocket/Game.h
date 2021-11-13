#pragma once

#include "VAO.h"

using namespace std;

class Game : public VAO {
private:
	static Game* instance;
	const char* title = "Open Rocket";
	const float PI = 3.141592;
	int colorCode;
	int height;
	int width;
	int init_pos_x;
	int init_pos_y;
	int maxX;
	int maxY;
	GLuint ProgramId;
	GLuint VboId;
	GLuint ColorBufferId;
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

	const char* getTitle() { return title; }
	int getHeight() { return height; }
	int getWidth() { return width; }
	int getInitPosX() { return init_pos_x; }
	int getInitPosY() { return init_pos_y; }
	int getMaxX() { return maxX; }
	int getMaxY() { return maxY; }
	GLuint getVboId() { return VboId; }
	GLuint getColorBufferId() { return ColorBufferId; }

	void InitializeGlew();
	void CreateShaders(const char* vertShader, const char* fragShader);
	void DestroyShaders(void);
	void Cleanup(void);
	void CreateVBO();
	void DestroyVBO(void);

	void InitializeGame(const char* vertShader, const char* fragShader);
	void RenderFunction(void);
};
