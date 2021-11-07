/* TRANSFORMARI.
   - utilizarea bibliotecii glm: https://glm.g-truc.net/0.9.9/index.html
   - transformari variate asupra primitivelor, transmise catre shader
   - colorarea primitivelor folosind variabile uniforme si shader-ul de fragment
*/
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;

//////////////////////////////////////

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
matrScaleLocation,
matrTranslLocation,
matrRotlLocation,
codColLocation;

glm::mat4 myMatrix, resizeMatrix, matrTransl, matrScale, matrRot, mTest;


int codCol;
float PI = 3.141592;

int width = 800, height = 450;

void displayMatrix()
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
			cout << myMatrix[ii][jj] << "  ";
		cout << endl;
	};
	cout << "\n";

};


void CreateVBO(void)
{
	// varfurile 
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
		1.0f, 1.0f, 0.0f, 0.f,
		1.0f, 1.0f, 0.0f, 0.f,
		1.0f, 1.0f, 0.0f, 0.f,

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


	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	// 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

}
void DestroyVBO(void)
{


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);


}

void CreateShaders(void)
{
	ProgramId = LoadShaders("04_03_Shader.vert", "04_03_Shader.frag");
	glUseProgram(ProgramId);
}


void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0));
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(-width, -height, 0.0));

	myMatrix = resizeMatrix * matrTransl;
	displayMatrix();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // culoarea de fond a ecranului
	CreateVBO();
	CreateShaders();
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glPointSize(10.0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_POLYGON, 6, 4);
	glDrawArrays(GL_TRIANGLES, 10, 3);
	glDrawArrays(GL_POLYGON, 13, 5);
	glDrawArrays(GL_POLYGON, 18, 5);

	glutPostRedisplay();
	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(2 * width, 2 * height);
	glutCreateWindow("Utilizarea glm pentru transformari");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();


}
