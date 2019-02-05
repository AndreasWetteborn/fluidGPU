#pragma once
#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

#define  ERROR_GLSL        -1
#define  ERROR_GLEW        -2
#define  ERROR_TEXTURE     -3
#define  ERROR_BINDFBO     -4
#define  ERROR_FBOTEXTURE  -5
#define  ERROR_PARAMS      -6


class Texture
{
public:
	Texture(int size, float *data);
	Texture(int sizeX, int sizeY,  float *data);
	~Texture();

	void Read(float *data);
	void Write(float *data);

	GLuint GetID() { return textureID; }
	GLuint GetReadTexture() { return textureID; }

private:
	void CheckGLErrors(const char *label);
	void CheckGLErrors(const char *label, const GLenum errCode);

	GLuint textureID;
	int sizeX, sizeY; 
};
