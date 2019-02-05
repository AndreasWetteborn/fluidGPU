#include "Texture.h"

Texture::Texture(int s , float *data): sizeX(s), sizeY(s)
{
	glGenTextures(1, &textureID);
	 // make active and bind
    glBindTexture(GL_TEXTURE_RECTANGLE,textureID);
    // turn off filtering and wrap modes
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // define texture with floating point format
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F , sizeX, sizeY, 0, GL_RGBA, GL_FLOAT, 0);

	Write(data);
    // check if that worked
	GLenum errorCode = glGetError();
    // if (glGetError() != GL_NO_ERROR) {
	if (errorCode != GL_NO_ERROR) {
		printf("glTexImage2D():\t\t\t [FAIL]\n");
		CheckGLErrors("Texture::Texture()", errorCode);
		exit (ERROR_TEXTURE);
    } else {
		printf("glTexImage2D():\t\t\t [PASS]\n");
    }	
}

Texture::Texture(int x , int  y , float *data): sizeX(x), sizeY(y)
{

	glGenTextures(1, &textureID);
	CheckGLErrors("Texture::Texture() gen");
	 // make active and bind
    glBindTexture(GL_TEXTURE_RECTANGLE, textureID);
	CheckGLErrors("Texture::Texture() bind");
    // turn off filtering and wrap modes
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
			//glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	CheckGLErrors("Texture::Texture() param 1");

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	CheckGLErrors("Texture::Texture() param 2");
    // define texture with floating point format
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, sizeX, sizeY, 0, GL_RGBA, GL_FLOAT, 0);
	CheckGLErrors("Texture::Texture() format");

	Write(data);
	CheckGLErrors("Texture::Texture() write");

	// check if that worked
	// GLenum errorCode = glGetError();
    // if (errorCode != GL_NO_ERROR) {
	// 	printf("glTexImage2D():\t\t\t [FAIL]\n");
	// 	CheckGLErrors("Texture::Texture()", errorCode);
	// 	exit (ERROR_TEXTURE);
    // } else {
	// 	printf("glTexImage2D():\t\t\t [PASS]\n");
    // }
}

Texture::~Texture()
{
}

void Texture::Read(float *data)
{	
	// bind instace texture
	glBindTexture(GL_TEXTURE_RECTANGLE,textureID );
	// write data to GL_TEXTURE_RECTANGLE
	glGetTexImage(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, GL_FLOAT, data);
}

void Texture::Write(float *data)
{ 
	// bind instace texture
	glBindTexture(GL_TEXTURE_RECTANGLE, textureID);
	// write data to texture
	glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0, sizeX ,sizeY, GL_RGB, GL_FLOAT,data);
}

// from Dominik Göddeke, http://www.mathematik.uni-dortmund.de/~goeddeke/gpgpu/tutorial.html
void Texture::CheckGLErrors(const char *label, const GLenum errCode) {
    // GLenum errCode;
    const GLubyte *errStr;
    
    // if ((errCode = glGetError()) != GL_NO_ERROR) {
	if (errCode != GL_NO_ERROR) {
	errStr = gluErrorString(errCode);
	printf("OpenGL ERROR: ");
	printf((char*)errStr);

	printf("(Label: ");
	printf(label);
	printf(")\n.");
    }
}

void Texture::CheckGLErrors(const char *label) {
	const GLubyte *errStr;
	GLenum errorCode = glGetError();

	if (errorCode != GL_NO_ERROR) {
		printf("glTexImage2D():\t\t\t [FAIL]\n");
		printf(label);
		printf("\n");
		errStr = gluErrorString(errorCode);
		printf("OpenGL ERROR: ");
		printf((char*)errStr);
		
		printf("\n.");
		exit(ERROR_TEXTURE);
	}
	else {
		printf("glTexImage2D():\t\t\t [PASS]\n");
	}
}
