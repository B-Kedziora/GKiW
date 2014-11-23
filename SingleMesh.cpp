#include "SingleMesh.h"


/*
*	We use 0xFFFFFFFF value to store information that buffers are unloaded. 
*	This prevents deleting buffers with random indexes
*/
SingleMesh::SingleMesh()
{
    VB = 0xFFFFFFFF;
    IB = 0xFFFFFFFF;
    NumIndices  = 0;
    MaterialIndex = 0xFFFFFFFF;
};

SingleMesh::~SingleMesh()
{
    if (VB != 0xFFFFFFFF)
    {
        glDeleteBuffers(1, &VB);
    }

    if (IB != 0xFFFFFFFF)
    {
        glDeleteBuffers(1, &IB);
    }
}

void SingleMesh::Init(const std::vector<Vertex>& Vertices,
                      const std::vector<GLuint>& Indices)
{
    NumIndices = Indices.size();

    glGenBuffers(1, &VB);
  	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * NumIndices, &Indices[0], GL_STATIC_DRAW);
}

void SingleMesh::render()
{
	glEnableClientState(GL_VERTEX_ARRAY);	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	GLint floatSize = sizeof(GL_FLOAT);
	GLint normalOffset = 3*floatSize; // pos has 3 floats
	GLint texOffset = 6*floatSize; // pos floats + normal floats

	/*Stride of parameters is one width of Vertex*/
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (const GLvoid*) normalOffset);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (const GLvoid*) texOffset);

	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
