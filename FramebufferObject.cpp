#include "FramebufferObject.h"

FramebufferObject::FramebufferObject() : framebufferID(0)
{
    glGenFramebuffersEXT(1, &framebufferID);
    CheckFramebufferStatus();
    Activate();
}

FramebufferObject::~FramebufferObject()
{
    glDeleteFramebuffersEXT(1, &framebufferID);
}

void FramebufferObject::Activate()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferID);
    CheckFramebufferStatus();
}

void FramebufferObject::Deactivate()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FramebufferObject::AttachTexture(GLenum attachmentPoint, GLuint textureID)
{
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachmentPoint, GL_TEXTURE_RECTANGLE, textureID, 0);
    CheckFramebufferStatus();
}

void FramebufferObject::SetDrawBuffer(GLenum attachmentPoint)
{
    glDrawBuffer(attachmentPoint);
    CheckFramebufferStatus();
}


/*
 * Checks framebuffer status. from Dominik Göddeke, http://www.mathematik.uni-dortmund.de/~goeddeke/gpgpu/tutorial.html
 * Copied directly out of the spec, modified to deliver a return value.
 */
bool FramebufferObject::CheckFramebufferStatus() {
    GLenum status;
    status = (GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        printf("Framebuffer incomplete, incomplete attachment\n");
        return false;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        printf("Unsupported framebuffer format\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        printf("Framebuffer incomplete, missing attachment\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        printf("Framebuffer incomplete, attached images must have same dimensions\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        printf("Framebuffer incomplete, attached images must have same format\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        printf("Framebuffer incomplete, missing draw buffer\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        printf("Framebuffer incomplete, missing read buffer\n");
        return false;
    }
    return false;
}

