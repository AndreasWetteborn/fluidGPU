
#pragma once
#include <vector>
#include <iostream>
#include <stdlib.h> 
#include <GL/glew.h>
#include <GL/glut.h>

#include "pfm.h"


// error codes
#define  ERROR_GLSL        -1
#define  ERROR_GLEW        -2
#define  ERROR_TEXTURE     -3
#define  ERROR_BINDFBO     -4
#define  ERROR_FBOTEXTURE  -5
#define  ERROR_PARAMS      -6

class Shader;
class Texture;
class DualTexture;
class FramebufferObject;


class FluidSolver
{
public:
    FluidSolver(int nx, int ny, float diffRate, float visc, float force, float source);
    ~FluidSolver();

    void DensityStep(float dt);
    void VelocityStep(float dt);

    void AddForce(int i, int j, float dirU, float dirV, float dt);
    void AddSource(int i, int j, float dt);

    void DrawVorticity(float x, float y);
    void DrawDensity(float x, float y);
    void DrawVelocity(float x, float y);

    void SetComputationModeOn();
    void SetDrawModeOn(float x, float y);

    int GetNx() { return Nx; }
    int GetNy() { return Ny; }

    GLuint GetVelocityTexture();

    void ToggleBouayncy() { buoyancyActive = !buoyancyActive; }
    void ToggleVorticity() { vorticityActive = !vorticityActive; }
    void ToggleInterpolation() { interpolateTexture = !interpolateTexture; }

private:
    void Advect(FramebufferObject *framebuffer, DualTexture *velocityTexture, DualTexture *advectTexture, float dt);
    void Diffuse(std::vector<float>& x, std::vector<float>& x0, float dt, int b);
    void Projection();

    // vorticity confinment 
    void VorticityStep(float dt);
    void CalcVorticity();
    void CalcN();
    void AddVorticity();

    void Buoyancy(float dt);

    void SetBounds(DualTexture *texture, int scale, int marker);

    void Calculate();
    void InitateShaders();
    void InitateFBO();

    void DrawToScreen(float x, float y);

    void CheckGLErrors(const char *label);

    /* N is disired visible cells in one dimension
     * size is total amunt of cells, including bounding cells, (N+2)*(N+2)
     * textureSize is total amount of cells in one dimension,(N+2)
    */
    bool buoyancyActive;
    bool vorticityActive;
    bool interpolateTexture;

    int Nx, Ny, size, textureSizeX, textureSizeY;
    float diffusionRate;
    float viscosity;
    float force;
    float source;
    float rdx;

    // used as data for a texture with only zeros
    float *zeroData, *data, *randData;

    DualTexture *velocityTexture, *pressureTexture, *densityTexture;

    Texture *divergenceTexture, *vorticityTexture, *NTexture;

    Shader *drawDensityShader, *drawVelocityShader, *addSourceShader,
        *addForceShader, *advectShader, *divergenceShader, *jacobiShader,
        *gradientShader, *boundaryShader, *bouyancyShader,
        // vorticity confinment
        *vorticityShader, *computeNShader, *addVorticityShader;

    FramebufferObject* densityFbo, *velocityFbo, *projectionFbo, *vorticityFbo;
};
