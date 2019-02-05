#pragma once
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


class Shader
{
public:
	Shader(std::string fileName);
	Shader(std::string vertexName, std::string fragmentName);
	~Shader(void);

	void Activate();

	void SetParameter(std::string parameterName , float value);
	void SetParameter(std::string parameterName , float value1, float value2);

	void SetTexture(std::string textureParameterName, GLint textureID , int textureUnit);

	GLuint GetProgram() {return program;}

private:
	void PrintShaderInfoLog(GLuint obj);
	void PrintProgramInfoLog(GLuint obj);
	void CheckGLErrors (const char *label);

	GLuint program;
	GLuint fragment, vertex;
};
