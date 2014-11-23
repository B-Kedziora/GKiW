#ifndef GRAPHICSTOOLKIT_H
#define GRAPHICSTOOLKIT_H

#include "gl\glew.h"
//#include "gl\glut.h"
#include "Vertex.h"

/*
*	Bunch of functions that hopefully will be useful in creating 3D apps. 
*/

class GraphicsToolkit
{
public:
	static void createVBO(GLuint& handle, GLuint verticesNumber, GLuint elementsPerVertex, float* vertices, GLint usageHint);
	static void createVBO(GLuint& handle, GLuint verticesNumber, Vertex* vertices, GLint usageHint);
	static void createIBO(GLuint& handle, GLuint indicesNumber, GLuint* indices, GLint usageHint);
private:
	GraphicsToolkit(void);
	~GraphicsToolkit(void);
};

#endif

