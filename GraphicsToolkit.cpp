#include "GraphicsToolkit.h"


GraphicsToolkit::GraphicsToolkit(void)
{
}


GraphicsToolkit::~GraphicsToolkit(void)
{
}


//GraphicsToolkit::createVBO(vertexBuffer[0], 4, 3, cubeVertices1, GL_STATIC_DRAW;
void GraphicsToolkit::createVBO(GLuint& handler, GLuint verticesNumber, GLuint elementsPerVertex, float* vertices, GLint usageHint)
{
	glGenBuffers(1, &handler);

		glBindBuffer(GL_ARRAY_BUFFER, handler);
		glBufferData(GL_ARRAY_BUFFER, verticesNumber*elementsPerVertex*sizeof(float), vertices, usageHint);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GraphicsToolkit::createVBO(GLuint& handler, GLuint verticesNumber, Vertex* vertices, GLint usageHint)
{
	glGenBuffers(1, &handler);

		glBindBuffer(GL_ARRAY_BUFFER, handler);
		glBufferData(GL_ARRAY_BUFFER, verticesNumber*8*sizeof(float), vertices, usageHint);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GraphicsToolkit::createIBO(GLuint& handler, GLuint indicesNumber, GLuint* indices, GLint usageHint)
{
	glGenBuffers(1, &handler);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handler);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indicesNumber, indices, usageHint);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}