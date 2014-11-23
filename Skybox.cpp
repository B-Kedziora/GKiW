#include "Skybox.h"


Skybox::Skybox(std::string directory, std::string format)
{
	float cubeVertices1[]={
	-2,-2,-2,
	-2, 2,-2,
	 2, 2,-2,
	 2,-2,-2
	 };

	float cubeVertices3[]={
	 2,-2,-2,
	 2, 2,-2,
	 2, 2, 2,
	 2, -2, 2,
	};

	float cubeVertices6[]={
	-2,-2,-2,
	-2,-2, 2,
	 2,-2, 2,
	 2,-2,-2
	 };

	float cubeVertices5[]={
	2, 2, 2,
	2, 2, -2,
	-2, 2, -2,
	-2, 2, 2
	};

	float cubeVertices4[]={
	-2,-2, 2,
	-2, 2, 2,
	-2, 2,-2,
	-2,-2,-2
	};

	float cubeVertices2[]={
	2,-2, 2,
	2, 2, 2,
	-2, 2, 2,
	-2,-2, 2
	 };

	GraphicsToolkit::createVBO(vertexBuffer[0], 4, 3, cubeVertices1, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(vertexBuffer[1], 4, 3, cubeVertices2, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(vertexBuffer[2], 4, 3, cubeVertices3, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(vertexBuffer[3], 4, 3, cubeVertices4, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(vertexBuffer[4], 4, 3, cubeVertices5, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(vertexBuffer[5], 4, 3, cubeVertices6, GL_STATIC_DRAW);

	float cubeTexCoords[]=	//The same for all walls
	{
	0,0, 0,1, 1,1, 1,0
	};
 
	for(int i = 0; i < 6; i ++)
	{
		GraphicsToolkit::createVBO(texCoordBuffer[i], 4, 2, cubeTexCoords, GL_STATIC_DRAW);
	}
 

	float cubeNormals1[]=
	{
		2,	2,	2,
		2, -2,	2,
		-2, -2,	2,
		-2,	2,	2
	}; 

	
	float cubeNormals3[]=
	{
		-2, 2,	2,
		-2,	-2,	2,
		-2,	-2,	-2,
		-2, 2,	-2,
	}; 


	float cubeNormals6[]=
	{
		2,	2,	2,
		2,	2, -2,
		-2,	2, -2,
		-2,	2,	2
	}; 

	float cubeNormals5[]=
	{
		-2, -2,	-2,
		-2, -2, 2,
		2, -2, 2,
		2,	-2,	-2
	}; 

	float cubeNormals4[]=
	{
		2,	2, -2,
		2, -2, -2,
		2, -2,	2,
		2,	2,	2
	}; 
	
	float cubeNormals2[]=
	{
		-2,	2, -2,
		-2, -2, -2,
		2,-2, -2,
		2,	2, -2
	}; 

	GraphicsToolkit::createVBO(normalBuffer[0], 4, 3, cubeNormals1, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(normalBuffer[1], 4, 3, cubeNormals2, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(normalBuffer[2], 4, 3, cubeNormals3, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(normalBuffer[3], 4, 3, cubeNormals4, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(normalBuffer[4], 4, 3, cubeNormals5, GL_STATIC_DRAW);
	GraphicsToolkit::createVBO(normalBuffer[5], 4, 3, cubeNormals6, GL_STATIC_DRAW);

	for(int i = 0; i < 6; i++)
	{
		skyboxTextures[i] = Texture::createTexture(directory + std::to_string(i+1) + format);
		skyboxTextures[i] ->repeatTexture (false); //Hides skybox edges
	}
}

void Skybox::renderSkybox()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);				
	glDepthMask(false);						//Disables writing to z-buffer
	for(int i = 0; i < 6; i++)
	{
		glEnableClientState(GL_VERTEX_ARRAY);	
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glBindTexture(GL_TEXTURE_2D, skyboxTextures[i]->getTexture());

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]);
		glVertexPointer(3, GL_FLOAT,0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[i]);
		glNormalPointer(GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer[i]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glDrawArrays(GL_QUADS, 0, 4);

		glDisableClientState(GL_VERTEX_ARRAY);	
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Skybox::~Skybox(void)
{
	for(int i = 0; i < 6; i ++ )
	{
		delete skyboxTextures[i];
	}
	glDeleteBuffers(6, vertexBuffer);
	glDeleteBuffers(6, normalBuffer);
	glDeleteBuffers(6, texCoordBuffer);
}
