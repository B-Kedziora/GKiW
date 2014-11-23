#pragma once
#include<vector>
#include"Vertex.h"
#include "gl\glew.h"
#include "gl\glut.h"
#include "GraphicsToolkit.h"

/*Class representing single mesh in Renderable object. Should not be used outside Renderable*/
class SingleMesh
{
public:
	SingleMesh();

	~SingleMesh();
	
	/*Creates buffers from arrays*/
    void Init(const std::vector<Vertex>& Vertices,
              const std::vector<unsigned int>& Indices);

	void render();

	GLuint MaterialIndex; //Texture paired with this mesh
private:
    GLuint VB;	//Positions, normals, texCoords
    GLuint IB;	//Face indices
    GLuint NumIndices;	//Number of faces

};

