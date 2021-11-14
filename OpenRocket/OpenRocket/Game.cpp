#include "Game.h"
#include "loadShaders.h"
#include "helpers.h"
#include "Asteroid.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

void Game::move(void)
{
	for (auto& asteroid : asteroids) {
		float translatedDistance = asteroid->getTranslatedDistance();
		asteroid->setTranslatedDistance(translatedDistance + Constants::asteroidMovingUnit);
	}
	glutPostRedisplay();
}
void Game::mouseHandler(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		glutIdleFunc(moveCallback);
		break;
	default:
		break;
	}
}
Game* Game::instance = nullptr;

Game* Game::getInstance(){
	if (instance == nullptr) {
		instance = new Game(Constants::width * 2, Constants::height * 2, 60, 60);
	}
	return instance;
}

Game::Game(int window_width, int window_height, int initial_pos_x, int initial_pos_y) : 
	nrOfStars(Constants::nrOfStars),
	width(window_width),
	height(window_height)
{
	setInitPosX(initial_pos_x);
	setInitPosY(initial_pos_y);
	maxX = width / 2;
	maxY = height / 2;

	InitializeGlew();
	CreateBackgroundBuffers();
	CreateRocketBuffers();
	CreateAsteroidBuffers();
	GenerateAsteroids();
}

void Game::InitializeGlew() {
	glutInitWindowPosition(getInitPosX(), getInitPosY());
	glutInitWindowSize(getWidth(), getHeight());
	glutCreateWindow(Constants::title);
	glewInit();
	glutDisplayFunc(renderCallback);
	glutMouseFunc(mouseCallback);
	glutCloseFunc(cleanupCallback); 
}

void Game::CreateShaders(const char* vertShader, const char* fragShader) {
	ProgramId = LoadShaders(vertShader, fragShader);
	glUseProgram(ProgramId);
}

void Game::DestroyShaders(void) {
	glDeleteProgram(ProgramId);
}

void Game::Cleanup(void) {
	DestroyShaders();
	DestroyBackgroundVBO();
}

void Game::DestroyBackgroundVBO(void) {
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &backgroundColorBufferId);
	glDeleteBuffers(1, &backgroundVbo);

	glBindVertexArray(0);
	//glDeleteVertexArrays(1, &VaoId);
}

void Game::InitializeGame(const char* vertShader, const char* fragShader) {
	CreateShaders("04_03_Shader.vert", "04_03_Shader.frag");

	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / maxX, 1.f / maxY, 1.0));
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0));

	myMatrix = resizeMatrix * matrTransl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Game::RenderFunction(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 backgroundMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / maxX, 1.f / maxY, 1.0));
	backgroundMatrix = backgroundMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(-maxX, -maxY, 0.0));

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &backgroundMatrix[0][0]);

	glPointSize(2.0);
	glBindVertexArray(backgroundVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEbo);
	glDrawArrays(GL_POINTS, 0, nrOfStars);

	glBindVertexArray(rocketVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_POLYGON, 6, 4);
	glDrawArrays(GL_TRIANGLES, 10, 3);
	glDrawArrays(GL_POLYGON, 13, 5);
	glDrawArrays(GL_POLYGON, 18, 5);	

	Game::UpdateAsteroids();
	for (auto& asteroid : asteroids) {

		
		glm::mat4 asteroidMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(asteroid->getRadius(), asteroid->getRadius(), 1.0));
		glm::mat4 animateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, - asteroid->getTranslatedDistance(), 0.0)); // controleaza translatia de-a lungul lui Oy
		asteroidMatrix = backgroundMatrix *  animateMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(asteroid->getX(), asteroid->getY(), 0.0)) * asteroidMatrix;
		
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &asteroidMatrix[0][0]);
		glBindVertexArray(asteroidVao);
		glDrawArrays(GL_POLYGON, 0, Constants::nrOfVerticesPerCircle);
	
	}
	glutPostRedisplay();
	glFlush();	
}


void Game::CreateRocketBuffers() {
	GLfloat Vertices[] = {
		// Triunghiul de sus
		0.f, 160.f, 0.f, 1.f,
		50.f, 160.f, 0.f, 1.f,
		25.f, 185.f, 0.f, 1.f,

		// Triunghiul de jos
		0.f, 100.f, 0.f, 1.f,
		50.f, 100.f, 0.f, 1.f,
		25.f, 165.f, 0.f, 1.f,

		// Desptunghiul
		15.f, 100.f, 0.f, 1.f,  // stanga jos
		35.f, 100.f, 0.f, 1.f, // dr jos
		35.f, 175.f, 0.f, 1.f,
		15.f, 175.f, 0.f, 1.f,

		// Triunghiul din varf
		15.f, 175.f, 0.f, 1.f,
		35.f, 175.f, 0.f, 1.f,
		25.f, 210.f, 0.f, 1.f,

		// Focul portocaliu
		15.f, 100.f, 0.f, 1.f,
		35.f, 100.f, 0.f, 1.f,
		50.f, 85.f, 0.f, 1.f,
		25.f, 50.f, 0.f, 1.f,
		0.f, 85.f, 0.f, 1.f,

		// Focul galben
		15.f, 100.f, 0.f, 1.f,
		35.f, 100.f, 0.f, 1.f,
		40.f, 90.f, 0.f, 1.f,
		25.f, 80.f, 0.f, 1.f,
		10.f, 90.f, 0.f, 1.f,

	};

	GLfloat Colors[] = {
		// Triunghiul de sus
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,

		// Triunghiul de jos
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,
		1.0f, 0.0f, 0.0f, 0.f,

		// Dreptunghiul
		0.8f, 0.5f, 0.2f, 0.f,
		0.8f, 0.5f, 0.2f, 0.f,
		0.8f, 0.5f, 0.2f, 0.f,
		0.8f, 0.5f, 0.2f, 0.f,

		// Triunghiul din varf
		1.0f, 1.0f, 1.0f, 0.f,
		1.0f, 1.0f, 1.0f, 0.f,
		1.0f, 1.0f, 1.0f, 0.f,

		// Focul portocaliu
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,
		1.f, 0.25f, 0.0f, 1.0f,

		// Focul galben
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f,
		1.0f, 0.8f, 0.0f, 1.0f
	};
	
	int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

	glGenBuffers(1, &rocketVbo);
	glBindBuffer(GL_ARRAY_BUFFER, rocketVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &rocketVao);
	glBindVertexArray(rocketVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &rocketColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, rocketColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void Game::CreateBackgroundBuffers() {	

	GLfloat Vertices[1000];
	GLfloat Colors[1000];

	srand(time(NULL));
	int i = 0;
	while (i < nrOfStars) { 
		Vertices[4 * i] = float(rand() % getWidth() + 1);
		Vertices[4 * i + 1] = float(rand() % getHeight() + 1);
		Vertices[4 * i + 2] = 0.f;
		Vertices[4 * i + 3] = 1.f;
		//cout << i << " " << Vertices[4 * i] << " " << Vertices[4 * i + 1] << " " << Vertices[4 * i + 2] << " " << Vertices[4 * i + 3] << "\n";
		Colors[4 * i] = 1.f;
		Colors[4 * i + 1] = 1.f;
		Colors[4 * i + 2] = 1.f;
		Colors[4 * i + 3] = 1.f;
		//cout << i << " " << Colors[4 * i] << " " << Colors[4 * i + 1] << " " << Colors[4 * i + 2] << " " << Colors[4 * i + 3] << "\n";
		i++;
	}

    int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

    glGenBuffers(1, &backgroundVbo);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &backgroundVao);
    glBindVertexArray(backgroundVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

	glGenBuffers(1, &backgroundColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
}
void Game::CreateAsteroidBuffers() {

	GLfloat Vertices[1000];
	GLfloat Colors[1000];
	for (int k = 0; k < Constants::nrOfVerticesPerCircle; k++) {
		int theta = Constants::TWO_PI * k / Constants::nrOfVerticesPerCircle;
		float x = cos(theta);
		float y = sin(theta);
		// varfurile corespunzatoare cercului
		Vertices[4 * k] = x;
		Vertices[4 * k + 1] = y;
		Vertices[4 * k + 2] = 0.0f;
		Vertices[4 * k + 3] = 1.0f;

		Colors[4 * k] = 1.0f;
		Colors[4 * k + 1] = 0.0f;
		Colors[4 * k + 2] = 0.0f;
		Colors[4 * k + 3] = 1.0f;
	}
	for (int i = 0; i < Constants::nrOfVerticesPerCircle * 4 - 4; i+=4) {
		//cout << Vertices[i] << " " << Vertices[i + 1] << " " << Vertices[i + 2] << " " << Vertices[i + 3] << "\n";
	}
	int verticesCount = sizeof(Vertices) / sizeof(GLfloat);

	glGenBuffers(1, &asteroidVbo);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &asteroidVao);
	glBindVertexArray(asteroidVao);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &asteroidColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

}
void Game::UpdateAsteroids() {
	auto end = std::remove_if(asteroids.begin(),
		asteroids.end(),
		[](Asteroid* const& i) {
		return i->belowViewport();  
	});

	asteroids.erase(end, asteroids.end());

	if (static_cast<int>(asteroids.size()) < Constants::nrOfAsteroidsPerFrame) {
		int nrToGenerate = Constants::nrOfAsteroidsPerFrame - asteroids.size();
		for (int k = 0; k < nrToGenerate; k++) {
			this->asteroids.push_back(Game::GenerateSingleAsteroid());
		}
	}
}
float Game::generateXOffset() {
	float maxDistance = 0, leftOffset = 0, rightOffset = 2 * Constants::width; 
	if (static_cast<int>(asteroids.size()) == 0) {
		leftOffset += Constants::distanceBetweenAsteroids;
		rightOffset -= Constants::distanceBetweenAsteroids;
		return leftOffset + rand() % ((int)(rightOffset - leftOffset + 1));
	}
	int i = 0;
	float leftAsteroid = asteroids[0]->getX(), rightAsteroid = asteroids[static_cast<int>(asteroids.size()) - 1]->getX();
	while(i < static_cast<int>(asteroids.size()) - 1) {
		leftAsteroid = min(leftAsteroid, asteroids[i]->getX());
		rightAsteroid = max(rightAsteroid, asteroids[i]->getX());
		if (maxDistance < abs(asteroids[i]->getX() - asteroids[i + 1]->getX())) {
			maxDistance = abs(asteroids[i]->getX() - asteroids[i + 1]->getX());
			rightOffset = asteroids[i + 1]->getX();
			leftOffset = asteroids[i]->getX();
			if (leftOffset > rightOffset) {
				swap(leftOffset, rightOffset);
			}
		}
		i++;
	}
	leftAsteroid = min(leftAsteroid, asteroids[i]->getX());
	rightAsteroid = max(rightAsteroid, asteroids[i]->getX());
	if (maxDistance < leftAsteroid) {
		maxDistance = leftAsteroid;
		rightOffset = leftAsteroid;
		leftOffset = 0;
	}
	if (maxDistance < 2 * Constants::width - rightAsteroid) {
		maxDistance = 2 * Constants::width - rightAsteroid;
		rightOffset = 2 * Constants::width;
		leftOffset = rightAsteroid;
	}
	return leftOffset + rand() % ((int)(rightOffset - leftOffset + 1));
}
Asteroid* Game::GenerateSingleAsteroid() {
	// generam random coordonata x
	float y_offset = 2 * Constants::height + rand() % (3 * Constants::height - 2 * Constants::height + 1);
	float x_offset = Game::generateXOffset();
	return new Asteroid(40.0, 20, glm::vec4(x_offset, y_offset, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}
void Game::GenerateAsteroids() {
	// generam un numar de asteroizi initial intr-o pozitie care nu se afla in view-ul nostru
	for (int i = 0; i < Constants::nrOfAsteroidsPerFrame; i++) {
		this->asteroids.push_back(Game::GenerateSingleAsteroid());
	}
}