#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

class FramebufferObject
{
public:
    FramebufferObject();
    ~FramebufferObject();

    void Activate();
    void Deactivate();

    void AttachTexture(GLenum attachmentPoint, GLuint textureID);
    void SetDrawBuffer(GLenum attachmentPoint);

private:
    bool CheckFramebufferStatus();
    GLuint framebufferID;
};
