#include "Shader.h"
#include "textfile.h"

Shader::Shader(std::string fileName)
{
    // create program object
    program = glCreateProgram();
    // create shader object (fragment shader)
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    // set source for shader
    char * fileSource = textFileRead(fileName.c_str());
    if (fileSource == NULL) {
        std::cout << "Error reading file: " << fileName << std::endl;
        exit(-1);
    }
    //const GLchar* source = textureParameters.shader_source;
    const GLchar* source = fileSource;
    glShaderSource(fragment, 1, &source, NULL);
    // compile shader
    glCompileShader(fragment);
    // check for errors
    PrintShaderInfoLog(fragment);
    // attach shader to program
    glAttachShader(program, fragment);
    // link into full program, use fixed function vertex pipeline
    glLinkProgram(program);
    // check for errors
    PrintProgramInfoLog(program);

    std::string label = "Shader::Shader() fileName = " + fileName;
    CheckGLErrors(label.c_str());
}

Shader::Shader(std::string vertexName, std::string fragmentName)
{
    // create program object
    program = glCreateProgram();
    // create shader object (fragment shader)

    vertex = glCreateShader(GL_VERTEX_SHADER);
    char * vertexFileSource = textFileRead(vertexName.c_str());
    const GLchar* vertexSource = vertexFileSource;
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    PrintShaderInfoLog(vertex);
    glAttachShader(program, vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    char * fragmentFileSource = textFileRead(fragmentName.c_str());
    const GLchar* fragmenSource = fragmentFileSource;
    glShaderSource(fragment, 1, &fragmenSource, NULL);
    glCompileShader(fragment);
    PrintShaderInfoLog(fragment);
    glAttachShader(program, fragment);

    glLinkProgram(program);
    // check for errors
    PrintProgramInfoLog(program);

    std::string label = "Shader::Shader() fileName = " + vertexName + " " + fragmentName;
    CheckGLErrors(label.c_str());
    // Get location of the texture samplers for future use   
}


Shader::~Shader(void)
{
    glDeleteShader(fragment);
    glDeleteProgram(program);
}

void Shader::Activate()
{
    // make shader active for rendering
    glUseProgram(program);
}

// set shader parameter with one value, prints error message if not working
void Shader::SetParameter(std::string parameterName, float value)
{
    GLint parameterLocation = glGetUniformLocation(program, parameterName.c_str());
    if (parameterLocation == -1)
        std::cout << "Error in  SetParameter parameterName = " << parameterName
        << " value " << value << std::endl;
    glUniform1f(parameterLocation, value);
}

// set shader parameter with two values, prints error message if not working
void Shader::SetParameter(std::string parameterName, float value1, float value2)
{
    GLint parameterLocation = glGetUniformLocation(program, parameterName.c_str());

    if (parameterLocation == -1)
        std::cout << "Error in  SetParameter parameterName = " << parameterName
        << " value1,value2 " << value1 << " , " << value2 << std::endl;

    float temp[2] = { value1,value2 };
    glUniform1fv(parameterLocation, 2, temp);
    CheckGLErrors("shader::SetParametr 2st");

}

// bind texture to shader. Prints error message if not working. 
void Shader::SetTexture(std::string textureParameterName, GLint textureID, int textureUnit)
{
    GLint parameterLocation = glGetUniformLocation(program, textureParameterName.c_str());
    if (parameterLocation == -1)
        std::cout << "Error in  SetTexture textureParameterName = " << textureParameterName
        << " textureID " << textureID << " textureUnit " << textureUnit << std::endl;

    // not very nice, can be done in a better way
    if (textureUnit == 0)
        glActiveTexture(GL_TEXTURE0);
    else if (textureUnit == 1)
        glActiveTexture(GL_TEXTURE1);
    else if (textureUnit == 2)
        glActiveTexture(GL_TEXTURE2);
    else
        std::cout << "error in Shader::SetTexture, textureUnit = " << textureUnit << std::endl;

    glBindTexture(GL_TEXTURE_RECTANGLE, textureID);
    glUniform1i(parameterLocation, textureUnit);
    std::string label = "shader::setTexture " + textureParameterName;
    CheckGLErrors(label.c_str());
}


/**
 * error checking for GLSL from Dominik Göddeke, http://www.mathematik.uni-dortmund.de/~goeddeke/gpgpu/tutorial.html
 */
void Shader::PrintProgramInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten = 0;
    char *infoLog;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf(infoLog);
        printf("\n");
        free(infoLog);
    }
}

// from Dominik Göddeke, http://www.mathematik.uni-dortmund.de/~goeddeke/gpgpu/tutorial.html
void Shader::PrintShaderInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten = 0;
    char *infoLog;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf(infoLog);
        printf("\n");
        free(infoLog);
    }
}

// from Dominik Göddeke, http://www.mathematik.uni-dortmund.de/~goeddeke/gpgpu/tutorial.html
void Shader::CheckGLErrors(const char *label) {
    GLenum errCode;
    const GLubyte *errStr;

    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errStr = gluErrorString(errCode);
        printf("OpenGL ERROR: ");
        printf((char*)errStr);
        printf("(Label: ");
        printf(label);
        printf(")\n.");
        // exit(ERROR_TEXTURE);
    }
}