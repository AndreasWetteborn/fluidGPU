#include "FluidSolver.h"
#include "textfile.h"
#include "Shader.h"
#include "Texture.h"
#include "DualTexture.h"
#include "FramebufferObject.h"

GLenum attachmentpoints[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

FluidSolver::FluidSolver(int nx, int ny,float diffRate, float visc , float f , float s ): 	Nx(nx), Ny(ny) , 	diffusionRate(diffRate), viscosity(visc), force(f),
						source(s) , buoyancyActive(false) , interpolateTexture(true), vorticityActive(false)
{	
	textureSizeX = Nx+2;
	textureSizeY = Ny+2;
	size = textureSizeX*textureSizeY;
	rdx = 1.0/Nx;
	
	zeroData = (float*)malloc(size *4*sizeof(float));
	data = (float*)malloc(size *4*sizeof(float));
	randData = (float*)malloc(size *4*sizeof(float));

	// generates a datavectors with zeros
	for(int i = 0; i < size *4 ; i++)
	{
		zeroData[i] =  0.0f;
		data[i] =  0.00;
		randData[i] = i*0.00001;
	}

	velocityTexture = new DualTexture(textureSizeX, textureSizeY, zeroData);
	pressureTexture = new DualTexture(textureSizeX, textureSizeY, zeroData);
	densityTexture = new DualTexture(textureSizeX, textureSizeY, zeroData);
	divergenceTexture = new Texture(textureSizeX, textureSizeY, zeroData);

	// vorticity confinment
	vorticityTexture = new Texture(textureSizeX, textureSizeY, zeroData);
	NTexture = new Texture(textureSizeX,  textureSizeY, zeroData);

	 // set texenv mode from modulate (the default) to replace
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 

	InitateShaders();	
	InitateFBO();	

	CheckGLErrors ("FluidSolver::FluidSolver()");	
}

FluidSolver::~FluidSolver()
{
	//std::cout << "FluidSolver::~FluidSolver()" << std::endl;	

	delete divergenceTexture;
	delete velocityTexture;
	delete pressureTexture;
	delete densityTexture;

	delete drawDensityShader;
	delete drawVelocityShader;
	delete addSourceShader;
	delete addForceShader;
	delete advectShader;
	delete divergenceShader;
	delete jacobiShader;
	delete gradientShader;

	// vorticity confinment 
	delete vorticityShader;
	delete computeNShader;
	delete addVorticityShader;

	delete densityFbo;	
	delete velocityFbo;
	delete projectionFbo;
}

void FluidSolver::SetComputationModeOn()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
	gluOrtho2D(0.0, textureSizeX, 0.0, textureSizeY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
	glViewport(0, 0, textureSizeX, textureSizeY);
}

void FluidSolver::SetDrawModeOn(float x, float y )
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, x, 0.0, y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, x, y);
}

GLuint FluidSolver::GetVelocityTexture()
{
	return velocityTexture->GetReadTexture();
}

void FluidSolver::DrawDensity(float x , float y)
{

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	drawDensityShader->Activate();  	
	drawDensityShader->SetTexture( "densityR", densityTexture->GetReadTexture() , 0);
	
	// nicer rendering
	if(interpolateTexture)
	{
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	DrawToScreen(x, y);
	
	/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, x, 0.0, y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, x, y);

	
	glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); 
			glVertex2f(5, 5);
			glTexCoord2f(textureSize, 0.0); 
			glVertex2f(x-5, 5);
			glTexCoord2f(textureSize, textureSize); 
			glVertex2f(x-5, y-5);
			glTexCoord2f(0.0, textureSize); 
			glVertex2f(5, y-5);
	glEnd();


	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   
	gluOrtho2D(0.0, textureSize, 0.0, textureSize);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
	glViewport(0, 0, textureSize, textureSize);
	*/
	
	if(interpolateTexture)
	{	
		// deactivate interpolation
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	
	CheckGLErrors ("drawDensity()");
}

void FluidSolver::DrawVelocity(float x , float y)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	drawVelocityShader->Activate();
	
	drawVelocityShader->SetTexture("velocityR" ,velocityTexture->GetReadTexture() , 0);

	DrawToScreen(x, y);

	/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, x, 0.0, y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, x, y);

	glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); 
			glVertex2f(5, 5);
			glTexCoord2f(textureSize, 0.0); 
			glVertex2f(x-5, 5);
			glTexCoord2f(textureSize, textureSize); 
			glVertex2f(x-5, y-5);
			glTexCoord2f(0.0, textureSize); 
			glVertex2f(5, y-5);
	glEnd();


	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
	gluOrtho2D(0.0, textureSize, 0.0, textureSize);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
	glViewport(0, 0, textureSize, textureSize);

	/**/

	
	CheckGLErrors ("drawVelocity()");
}

void FluidSolver::DrawVorticity(float x , float y)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	drawDensityShader->Activate();	
	drawDensityShader->SetTexture("densityR" ,vorticityTexture->GetReadTexture() , 0);

	DrawToScreen(x, y);
	
	CheckGLErrors ("drawVelocity()");
}

void FluidSolver::DrawToScreen(float x, float y)
{
	/*

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, x, 0.0, y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, x, y);

	*/
	//std::cout << x << " " << y << " "<< textureSizeX<< " " << textureSizeY << std::endl;

	float inset = 5.0;
	glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); 
			glVertex2f(inset, inset);
			glTexCoord2f(textureSizeX, 0.0); 
			glVertex2f(x-inset, inset);
			glTexCoord2f(textureSizeX, textureSizeY); 
			glVertex2f(x-inset, y-inset);
			glTexCoord2f(0.0, textureSizeY); 
			glVertex2f(inset, y-inset);
	glEnd();

	/*

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
	gluOrtho2D(0.0, textureSizeX, 0.0, textureSizeY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
	glViewport(0, 0, textureSizeX, textureSizeY);

	*/
}

void FluidSolver::InitateShaders()
{
	drawDensityShader = new Shader("shaders/renderDensity.frag");
	drawVelocityShader = new Shader("shaders/renderVelocity.frag");
	addSourceShader = new Shader("shaders/addSource.frag");
	addForceShader = new Shader("shaders/addForce.frag");	
	advectShader  = new Shader("shaders/advect.frag");
	divergenceShader = new Shader("shaders/divergence.frag");	
	jacobiShader = new Shader("shaders/jacobi.frag");	
	gradientShader = new Shader("shaders/gradient.frag");
	boundaryShader = new Shader("shaders/boundary.frag");

	// vorticity confinment
	vorticityShader = new Shader("shaders/vorticity.frag");
	computeNShader = new Shader("shaders/computeN.frag");
	 addVorticityShader = new Shader("shaders/addVorticity.frag");

	bouyancyShader = new Shader("shaders/buoyancy.frag");

	CheckGLErrors ("FluidSolver::InitateShaders()");	
}

// create all FBOs and attach correct textures 
void FluidSolver::InitateFBO()
{
	densityFbo = new FramebufferObject();
	densityFbo->AttachTexture(attachmentpoints[densityTexture->GetWriteID()] , densityTexture->GetWriteTexture());
	densityFbo->AttachTexture(attachmentpoints[densityTexture->GetReadID()] , densityTexture->GetReadTexture());

	velocityFbo = new FramebufferObject();
	velocityFbo->AttachTexture(attachmentpoints[velocityTexture->GetWriteID()] , velocityTexture->GetWriteTexture());
	velocityFbo->AttachTexture(attachmentpoints[velocityTexture->GetReadID()] , velocityTexture->GetReadTexture());

	projectionFbo = new FramebufferObject();
	projectionFbo->AttachTexture(attachmentpoints[pressureTexture->GetWriteID()] ,  pressureTexture->GetWriteTexture());
	projectionFbo->AttachTexture(attachmentpoints[pressureTexture->GetReadID()] ,  pressureTexture->GetReadTexture());
	projectionFbo->AttachTexture(GL_COLOR_ATTACHMENT2, divergenceTexture->GetID() );
	projectionFbo->AttachTexture(GL_COLOR_ATTACHMENT3, velocityTexture->GetWriteTexture() );

	// projectionFbo->AttachTexture(GL_COLOR_ATTACHMENT2_EXT, divergenceTexture->GetID());
	// projectionFbo->AttachTexture(GL_COLOR_ATTACHMENT3_EXT, velocityTexture->GetWriteTexture());

	// vorticity confinment
	vorticityFbo = new FramebufferObject();
	projectionFbo->AttachTexture(attachmentpoints[0] ,  vorticityTexture->GetID());
	projectionFbo->AttachTexture(attachmentpoints[1] ,  NTexture->GetID());

	CheckGLErrors ("FluidSolver::InitateFBO()");	
}

void FluidSolver::AddForce(int i, int j, float dirU, float dirV,float  dt)
{	
	velocityFbo->Activate();
	velocityFbo->SetDrawBuffer(attachmentpoints[velocityTexture->GetWriteID()]);
	
	addForceShader->Activate();

	addForceShader->SetTexture("velocitySource", velocityTexture->GetReadTexture(),0 );
	addForceShader->SetParameter("coords" , i, j);
	addForceShader->SetParameter("direction" , dirU ,dirV );

	addForceShader->SetParameter( "dt", dt);
	addForceShader->SetParameter( "rdx" , rdx);

	Calculate();

	velocityTexture->Swap();

	CheckGLErrors ("addForce");
}

void FluidSolver::AddSource(int i, int j, float dt)
{	
	// activate shader and set coorect drawbuffer
	densityFbo->Activate();
	densityFbo->SetDrawBuffer(attachmentpoints[densityTexture->GetWriteID()]);

	// activate add source shader and set shader parameters
	addSourceShader->Activate();	
	
	addSourceShader->SetTexture( "densitySource", densityTexture->GetReadTexture() ,0 );
	addSourceShader->SetParameter("coords", i,j);

	addSourceShader->SetParameter( "dt", dt);
	addSourceShader->SetParameter( "rdx" , rdx);
   
	Calculate();

	densityTexture->Swap();

	CheckGLErrors ("addSource");
}

void FluidSolver::Calculate()
{
	// render a quad the includes all pixel exept 1 pixel wide border.
	glBegin(GL_QUADS);					
			glTexCoord2f(1.0, 1.0); 
			glVertex2f(1.0, 1.0);
			glTexCoord2f(textureSizeX-1, 1.0); 
			glVertex2f(textureSizeX-1, 1.0);
			glTexCoord2f(textureSizeX-1, textureSizeY-1); 
			glVertex2f(textureSizeX-1, textureSizeY-1);
			glTexCoord2f(1.0, textureSizeY-1); 
			glVertex2f(1.0, textureSizeY-1);
	glEnd();
}

void FluidSolver::Diffuse(std::vector<float> &x, std::vector<float> &x0, float dt, int b )
{
	// Not yet implemented
}


void FluidSolver::Advect(FramebufferObject *framebuffer, DualTexture *velocityTexture , DualTexture *advectTexture , float dt)
{
	// activate framebuffer and set correct drawbuffer
	framebuffer->Activate();
	framebuffer->SetDrawBuffer(attachmentpoints[advectTexture->GetWriteID()]);	

	// activate shader and set shaderparameters 
	advectShader->Activate();		

	advectShader->SetTexture("velocitySource", velocityTexture->GetReadTexture(), 0);
	advectShader->SetTexture("quantityToAdvect", advectTexture->GetReadTexture(), 1);

	advectShader->SetParameter( "dt", dt);
	advectShader->SetParameter( "rdx" , 1);

	// do actual advection calculation
	Calculate();

	advectTexture->Swap();

	CheckGLErrors ("FluidSolver::Advect()");
}

void FluidSolver::DensityStep( float dt)
{
	
	Advect(densityFbo ,velocityTexture , densityTexture , dt);
}

void FluidSolver::VelocityStep(float dt  )
{
	if(vorticityActive)
	{
		VorticityStep(dt);
	}

	if(buoyancyActive)
	{
		Buoyancy(dt);
	}

	Advect(velocityFbo ,velocityTexture , velocityTexture , dt);
	
	Projection();	
	
	//densityTexture->ReadBackTexture(data);
}

void FluidSolver::Projection()
{
	// calc divergence
	
	// divegence texture
	projectionFbo->Activate();
	projectionFbo->SetDrawBuffer(GL_COLOR_ATTACHMENT2);
	// projectionFbo->SetDrawBuffer(GL_COLOR_ATTACHMENT2_EXT);
	
	divergenceShader->Activate();

	divergenceShader->SetTexture("velocity", velocityTexture->GetReadTexture(),0);
	divergenceShader->SetParameter("halfrdx" , 0.5/1);
	//divergenceShader->SetParameter("halfrdx" , 0.5/rdx);

	Calculate();
	
	CheckGLErrors ("divegence");

	// jacobi iteration
	jacobiShader->Activate();
	jacobiShader->SetTexture("b",divergenceTexture->GetID(),0);	
		
	jacobiShader->SetParameter( "alpha" , -(1*1));
	
	for ( int i = 0 ; i < 20; i++)
	{
		jacobiShader->Activate();
		jacobiShader->SetTexture("b",divergenceTexture->GetID(),0);	
		jacobiShader->SetParameter( "alpha" , -(1*1));
				
		projectionFbo->SetDrawBuffer(attachmentpoints[pressureTexture->GetWriteID()]);
		
		jacobiShader->SetTexture("x" ,pressureTexture->GetReadTexture(), 1);
	
		Calculate();

		pressureTexture->Swap();

		SetBounds(pressureTexture , 1, 0 );		
	}
	CheckGLErrors ("jacobi");

	//subtract gradient from veloity 
	gradientShader->Activate();
	
	gradientShader->SetTexture("velocity" , velocityTexture->GetReadTexture(), 0);
	gradientShader->SetTexture( "pressure", pressureTexture->GetReadTexture(), 1);

	gradientShader->SetParameter( "halfrdx", 0.5/1);
	//gradientShader->SetParameter( "halfrdx", 0.5/rdx);

	// connect correct drawbuffer
	projectionFbo->AttachTexture(GL_COLOR_ATTACHMENT3, velocityTexture->GetWriteTexture());
	projectionFbo->SetDrawBuffer(GL_COLOR_ATTACHMENT3);

	// projectionFbo->AttachTexture(GL_COLOR_ATTACHMENT3_EXT, velocityTexture->GetWriteTexture());
	// projectionFbo->SetDrawBuffer(GL_COLOR_ATTACHMENT3_EXT);
	
	Calculate();

	velocityTexture->Swap();

	SetBounds(velocityTexture , -1 , 1);	
	
	CheckGLErrors ("gradient");
}

// vorticity confinment
void FluidSolver::VorticityStep(float dt)
{
	CalcVorticity();
	CalcN();
	AddVorticity();
}

void FluidSolver::CalcVorticity()
{
	vorticityFbo->Activate();
	vorticityFbo->SetDrawBuffer(attachmentpoints[0]); // write to vorticityTexture

	vorticityShader->Activate();
	vorticityShader->SetTexture("velocitySource", velocityTexture->GetReadTexture(), 0 );

	Calculate();

	CheckGLErrors ("CalcVorticity");
}

void FluidSolver::CalcN()
{
	vorticityFbo->Activate();
	vorticityFbo->SetDrawBuffer(attachmentpoints[1]); // write to NTexture

	computeNShader->Activate();
	computeNShader->SetTexture("vorticitySource", vorticityTexture->GetID(), 0 );

	Calculate();

	CheckGLErrors ("CalcN");
}

void FluidSolver::AddVorticity()
{
	velocityFbo->Activate();
	velocityFbo->SetDrawBuffer(attachmentpoints[velocityTexture->GetWriteID()]);
	
	addVorticityShader->Activate();
	addVorticityShader->SetTexture("velocitySource", velocityTexture->GetReadTexture(), 0 );
	addVorticityShader->SetTexture("vorticitySource", vorticityTexture->GetID(), 1 );
	addVorticityShader->SetTexture("NSource", NTexture->GetID(), 2 );

	addVorticityShader->SetParameter("eps", 0.005);

	Calculate();

	velocityTexture->Swap();
	
	CheckGLErrors("AddVorticity");
}

void FluidSolver::Buoyancy(float dt)
{
	velocityFbo->Activate();
	velocityFbo->SetDrawBuffer(attachmentpoints[velocityTexture->GetWriteID()]);

	bouyancyShader->Activate();
	bouyancyShader->SetTexture("density", densityTexture->GetReadTexture(),0 );
	bouyancyShader->SetTexture("currentVelocity", velocityTexture->GetReadTexture(),1 );
	bouyancyShader->SetParameter("scale" , dt*0.001);

	Calculate();

	velocityTexture->Swap();
	
	CheckGLErrors("addForce");
}

// for velocity bounudary scale should be -1
// for pressure scale should be 1
void FluidSolver::SetBounds(DualTexture *texture, int scale , int marker)
{
	
	boundaryShader->Activate();

	if(marker == 1 )
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_RECTANGLE, texture->GetWriteTexture(), 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		// glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_TEXTURE_RECTANGLE_ARB, texture->GetWriteTexture(), 0);
		// glDrawBuffer(GL_COLOR_ATTACHMENT3_EXT);
	}
	else
	{
		glDrawBuffer(attachmentpoints[texture->GetWriteID()]);
	}
	
	boundaryShader->SetTexture("x" , texture->GetReadTexture() , 0);
	boundaryShader->SetParameter("scale" , scale);

	//float top = textureSize;
	float bottom  = 0.0;

	// left edge
	boundaryShader->SetParameter("offset", 1, 0);	

	glBegin(GL_LINES);		
		glTexCoord2f(0.0, 0.0); 
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0, textureSizeY); 
		glVertex2f(0, textureSizeY);
	glEnd();

	// right edge
	boundaryShader->SetParameter("offset", -1, 0);
	glBegin(GL_LINES);
		glTexCoord2f(textureSizeX, 0.0); 
		glVertex2f(textureSizeX, 0.0);
		glTexCoord2f(textureSizeX-1, textureSizeY-1); 
		glVertex2f(textureSizeX-1, textureSizeY-1);
	glEnd();

	// bottom edge
	boundaryShader->SetParameter("offset", 0, 1);
	glBegin(GL_LINES);			
		glTexCoord2f(0.0, 0.0); 
		glVertex2f(0.0, 0.0);
		glTexCoord2f(textureSizeX, 0.0); 
		glVertex2f(textureSizeX, 0.0);
	glEnd();

	// top edge		
	boundaryShader->SetParameter("offset", 0, -1);	
	glBegin(GL_LINES);			
		glTexCoord2f(0.0, textureSizeY); 
		glVertex2f(0.0, textureSizeY-1);
		glTexCoord2f(textureSizeX-1, textureSizeY-1); 
		glVertex2f(textureSizeX, textureSizeY);		
	glEnd();
}

void FluidSolver::CheckGLErrors (const char *label) {
    GLenum errCode;
    const GLubyte *errStr;
    
    if ((errCode = glGetError()) != GL_NO_ERROR) {
	errStr = gluErrorString(errCode);
	printf("OpenGL ERROR: ");
	printf((char*)errStr);
	printf("(Label: ");
	printf(label);
	printf(")\n.");
    }
}
