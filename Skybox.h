#ifndef SKYBOX_H
#define SKYBOX_H

#include "Texture.h"
#include <string>
#include "glm\glm.hpp"
#include "gl\glew.h"
#include "gl\glut.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "GraphicsToolkit.h"

/*	Basic class to generate a skybox. It requires 6 textures named: 1.format, 2.format and so on. Sequence of the pictures is not specified and
*	It must be done by trial and error method. The class doesn't write to z-buffer, so renderSkybox() can be freely use as first in rendering
*	routine without performance drops. In order to hide skybox edges textures must have GL_CLAMP_TO_EDGE parameter (it's default in our Texture class).
*	Written by Bartosz Kêdziora
*/

/*	
	TODO: 
	-Merge all of the buffers in one buffer.
	-Make dot in format optional, but still valid.
*/

class Skybox
{
public:
	/*Creates buffers and loads skybox textures to GPU. Image format must be preceded by dot. For example: ".jpg" is valid format, "jpg" is not.*/
	Skybox(std::string directory, std::string format);

	/*Deletes Textures and buffers from GPU*/
	~Skybox(void);

	/*Called in order to display skybox. It disables depth test and writing to z-buffer and then renders walls one by one. */
	void renderSkybox();

private:
	Texture* skyboxTextures[6];
	GLuint vertexBuffer[6];
	GLuint normalBuffer[6];
	GLuint texCoordBuffer[6];
};

#endif

