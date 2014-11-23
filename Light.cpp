#include "Light.h"

GLboolean Light::lightsTaken[8];

Light::Light(glm::vec3 pos, GLboolean dirLight)
{
	index=-1;
	for(GLint i=0; i<8; i++)	//Searches for the free slot
	{
		if(!lightsTaken[i])
		{
			lightsTaken[i] = true;
			index = i;
			break;
		}
	}
	if(index == -1)
		printf("No free lights");
	position[0] = pos.x;
	position[1] = pos.y;
	position[2] = pos.z;
	if(dirLight)
		position[3] = 0.0f;
	else
		position[3] = 1.0f;

	glEnable(GL_LIGHT0 + index);
	glLightfv(GL_LIGHT0 + index, GL_POSITION, position);

	/*Default parameters*/
	float white[] = {1.0f, 1.0f, 1.0f, 0.0f};
	float ambientSpec[] = {0.15f, 0.15f, 0.15f, 0.0f};
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, ambientSpec);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, ambientSpec);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, white);
}

void Light::Move(glm::vec3 newPosition)
{
	position[0] = newPosition.x;
	position[1] = newPosition.y;
	position[2] = newPosition.z;
	glLightfv(GL_LIGHT0 + index, GL_POSITION, position);
}

void Light::setParameter(GLuint parameter, float* arguments)
{
	if((parameter == GL_AMBIENT) || (parameter == GL_DIFFUSE) || (parameter == GL_SPECULAR))
		glLightfv(GL_LIGHT0 + index, parameter, arguments);
}

Light::~Light(void)
{
	glDisable(GL_LIGHT0 + index);
	lightsTaken[index] = false;
}
