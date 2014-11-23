#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Camera.h"

/*
* Light class support point and directional lights. It allows to create, destroy, move and change parameters of light.
* Cone lighting not yet supported (and probably will never be [certainly]).
* User should take care about appropriate space externally (except for world space). 
*/
class Light
{
public:
	/*Creates light in actual space.*/
	Light(glm::vec3 pos, GLboolean dirLight);

	/*Position should be in correct space*/
	void Move(glm::vec3 newPosition);

	/*Set ambient, difuse and specular*/
	void setParameter(GLuint parameter, float* arguments);
	
	/*Disables light and free space*/
	~Light(void);
private:
	GLint index;							//Stores index of this light
	GLfloat position[4];
	static GLboolean lightsTaken[8];		//Stores information about free light slots
};

