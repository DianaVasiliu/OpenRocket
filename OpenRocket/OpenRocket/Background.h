#pragma once

#include "VAO.h"

using namespace std;

class Background : public VAO {
private:
	static Background* instance;
	int nrOfStars;
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
	Background();
	~Background();

	static Background* getInstance();

	void setNrOfStars(int nrOfStars1) { nrOfStars = nrOfStars1; }

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
