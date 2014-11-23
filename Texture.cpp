#include "Texture.h"

Texture* Texture::createTexture(std::string filePath)
{
	//It will be destroyed if texture cannot be created
	Texture* generatedTexture = new Texture();

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE* bits(0);
	
	//Checking image type
	fif = FreeImage_GetFileType(filePath.c_str(), 0);
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filePath.c_str());
	if(fif == FIF_UNKNOWN)
	{
		printf("Cannot create texture! \n");
		delete generatedTexture;
		return nullptr;
	}


	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filePath.c_str());
	if(!dib)
	{
		printf("Cannot create texture! \n");
		delete generatedTexture;
		return nullptr;
	}

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	GLint BPP = FreeImage_GetBPP(dib);
	BPP = (BPP == 24 ? GL_BGR : BPP == 8 ? GL_LUMINANCE : 0);
	generatedTexture->width = FreeImage_GetWidth(dib);
	generatedTexture->height = FreeImage_GetHeight(dib);
	if((bits == 0) || (generatedTexture->width == 0) || (generatedTexture->height == 0))
	{
		printf("Cannot create texture! \n");
		delete generatedTexture;
		return nullptr;
	}

	glGenTextures(1, &generatedTexture->textureHandle);
	glBindTexture(GL_TEXTURE_2D, generatedTexture->textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, generatedTexture->width, generatedTexture->height,
		0, BPP, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//return success
	printf("Texture loaded: %s \n", filePath.c_str());
	return generatedTexture;
}

void Texture::setFiltering(GLint parameter, bool min)
{
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	if(min)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::repeatTexture(bool repeat)
{
	if(repeat)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

Texture::Texture()
{

}


Texture::~Texture(void)
{
	glDeleteTextures(1, &textureHandle);
}
