#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "gl\glew.h"
#include "gl\glut.h"
#include "FreeImage.h"

/*
*	Simple class written using FreeImage to load and hold textures.
*	Works only for 24bit or 32bit 2D textures. 
*	Written by Bartosz Kêdziora
*/

class Texture
{
public:
	/*Only way to get Texture instance. It returns pointer to Texture* if success and nullptr if texture cannot be created*/
	static Texture* createTexture(std::string filePath);

	/*Remove texture from GPU memory*/
	~Texture(void);

	/*Used to access texture data on GPU. Returns index of texture.*/
	GLuint getTexture()
	{
		return textureHandle;
	}

	/*Set texture parametrs. 
	First argument is standard OpenGL parameter. 
	Second means if you want to set min or mag
	Example: setFiltering(GL_LINEAR, TRUE)*/
	void setFiltering(GLint parameter, bool min);

	/*Objects with UV outside [0;1] can be repeated or extended*/
	void repeatTexture(bool repeat);
private:
	Texture();
	GLuint textureHandle;
	GLsizei width, height;
};


#endif
