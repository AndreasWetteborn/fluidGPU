#include "ParticleSolver.h"
#include "Shader.h"
#include "FramebufferObject.h"
#include "DualTexture.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GLenum attachmentpointsParticles[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };

ParticleSolver::ParticleSolver(int nrPX, int nrPY) : nrOfParticlesX(nrPX), nrOfParticlesY(nrPY)
{
    totParticles = nrPX * nrPY;

    float *zeroData = (float*)malloc(totParticles * 4 * sizeof(float));

    srand(time(NULL));

    for (int i = 0; i < totParticles * 4; i++)
    {
        //zeroData[i] =   0.0f;
        zeroData[i] = rand() % 540;
    }

    particleTexture = new DualTexture(nrOfParticlesX, nrOfParticlesY, zeroData);

    particleDrawShader = new Shader("particleDraw.vert", "particleDraw.frag");
    particleUpdateShader = new Shader("particleUpdate.frag");

    particleFbo = new FramebufferObject();
    particleFbo->AttachTexture(attachmentpointsParticles[particleTexture->GetWriteID()], particleTexture->GetWriteTexture());
    particleFbo->AttachTexture(attachmentpointsParticles[particleTexture->GetReadID()], particleTexture->GetReadTexture());
}


ParticleSolver::~ParticleSolver()
{
}


void ParticleSolver::SetComputationModeOn()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, nrOfParticlesX, 0.0, nrOfParticlesY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, nrOfParticlesX, nrOfParticlesY);
}

void ParticleSolver::SetDrawModeOn(float x, float y)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, x, 0.0, y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, x, y);
}


void ParticleSolver::Draw()
{

    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    //drawVelocityShader->Activate();

    particleDrawShader->Activate();
    particleDrawShader->SetTexture("particleTex", particleTexture->GetReadTexture(), 0);

    //glEnable(GL_PROGRAM_POINT_SIZE );

    glBegin(GL_POINTS);

    for (int i = 0; i < nrOfParticlesX; i++)
    {
        for (int j = 0; j < nrOfParticlesY; j++)
        {
            glColor4f(i, j, 0, 0);
            //glTexCoord2f((float)i/(float)nrOfParticlesX,(float) j/(float)nrOfParticlesY);
            //glTexCoord2f(i,j);
            glVertex3f(0.0f, 0.0f, 0.0f);
        }
    }

    glEnd();
}

void ParticleSolver::Update(GLuint velocityTexture, float dt)
{
    particleFbo->Activate();
    particleFbo->SetDrawBuffer(attachmentpointsParticles[particleTexture->GetWriteID()]);

    particleUpdateShader->Activate();

    particleUpdateShader->SetTexture("velocitySource", velocityTexture, 0);

    particleUpdateShader->SetTexture("particleSource", particleTexture->GetReadTexture(), 1);

    glBegin(GL_QUADS);
    //glColor3f(1.0 ,0.0 , 1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(nrOfParticlesX, 0.0);
    glVertex2f(nrOfParticlesX, 0.0);
    glTexCoord2f(nrOfParticlesX, nrOfParticlesY);
    glVertex2f(nrOfParticlesX, nrOfParticlesY);
    glTexCoord2f(0.0, nrOfParticlesY);
    glVertex2f(0.0, nrOfParticlesY);
    glEnd();

    particleTexture->Swap();
}