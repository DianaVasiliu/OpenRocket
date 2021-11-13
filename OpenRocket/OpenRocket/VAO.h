#ifndef VAO_HEADER
#define VAO_HEADER
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm/glm.hpp"  
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;


class VAO {
protected:
	GLuint VaoId;
public:
	VAO() {
		glGenVertexArrays(1, &VaoId);
		glBindVertexArray(VaoId);
	}
	~VAO() {

	}
};
#endif