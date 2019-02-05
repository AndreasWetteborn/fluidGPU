#pragma once
#include <stdlib.h> 
#include <GL/glew.h>
#include <GL/glut.h>

class Shader;
class FramebufferObject;
class DualTexture;

class ParticleSolver
{
public:
    ParticleSolver(int nrOfParticlesX, int nrOfParticlesY);
    ~ParticleSolver();

    void Draw();
    void Update(GLuint velocityTexture, float dt);

    void SetComputationModeOn();
    void SetDrawModeOn(float x, float y);

private:
    Shader *particleDrawShader, *particleUpdateShader;
    DualTexture *particleTexture;

    int nrOfParticlesX, nrOfParticlesY, totParticles;
    FramebufferObject *particleFbo;
};

