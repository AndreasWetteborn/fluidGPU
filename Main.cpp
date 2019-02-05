#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>

#include <time.h>

#include <iostream>

#include "FluidSolver.h"
#include "ParticleSolver.h"
#include "textfile.h"

// // Timing Variables , FPS
clock_t start, finish ;
double timeSinceLastUpdate = 0.0;
double updateInterval = 1.0;
int framecount = 0;
double fps = 0.0 ;


GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat xRotOld = 0;
GLfloat yRotOld = 0;
int mouseState = 0;
int xCenter = 0;
int yCenter = 0;


static int win_id;
static int win_x = 1024	, win_y = 1024;
// static int win_x = 960, win_y = 540;
// static int win_x = 256	, win_y = 256;

static int fluidResX = 2048, fluidResY = 2048;
// static int fluidResX = 256, fluidResY = 256;
// static int fluidResX = 960, fluidResY = 540;
// static int fluidResX = 480, fluidResY = 270;

static int particleX = 128, particleY = 128;
// static int particleX = 256, particleY = 256;
static int mouse_down[3];
static int omx, omy, mx, my;


float dt;


bool drawVelocity = false;
bool drawDensity = true;
bool drawVorticity = false;

FluidSolver *fluid;
ParticleSolver *particle;
// TuioInterface *tuioInterface;


void InitGLEW (void) 
{
    int err = glewInit();
    if (GLEW_OK != err) 
	{
		printf((char*)glewGetErrorString(err));
		exit(ERROR_GLEW);
    }  
}

void Init() {

	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();

	start = clock();	

	
	 dt = 0.1;

}

// calulate forces from mouse motion and add new source
void UpdateFromUI()
{	
	
	int Nx = fluid->GetNx();
	int Ny = fluid->GetNy();
	int  size = (Nx+2)*(Ny+2);


	if ( !mouse_down[0] && !mouse_down[2] ) 
	{
		return;
	}

	int	i = (int)((       mx /(float)win_x)*Nx+1);
	int j = (int)(((win_y-my)/(float)win_y)*Ny+1);

	if ( i<1 || i>Nx || j<1 || j>Ny ) 
	{
		return;
	}

	if ( mouse_down[0] ) 
	{
		fluid->AddForce(i, j, (mx-omx), (omy-my), dt );		

		//std::cout << "i " << i << " j " <<  j << " xCoord  " << xCoord << " yCoord  " << yCoord << " win_x  " << win_x << " win_y  " << win_y << std::endl;
		//std::cout << "(mx-omx) " << (mx-omx) <<" (omy-my) " << (omy-my) << std::endl;
	}


	if ( mouse_down[2] ) 
	{
		fluid->AddSource(i, j , dt);
	}
		
	omx = mx;
	omy = my;
	

	return;
}

void PrintFPS() 
{
	char buffer[15] ;
	int Nb = sprintf(buffer, " FPS : %3.1f", fps) ;
	glutSetWindowTitle(buffer);
	//glRasterPos2i(0, 50) ;
	//for (int i = 0; i < Nb; i++) 
	//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,(int) buffer[i]) ;
}

void Idle()
{    

	fluid->SetComputationModeOn();

	UpdateFromUI();

	fluid->VelocityStep(dt);	
	fluid->DensityStep(dt);

	//particle->SetComputationModeOn();
	//particle->Update(fluid->GetVelocityTexture(), dt);

    glutPostRedisplay();
}

void DrawDensity ( void )
{
	fluid->DrawDensity(	win_x ,win_y );	
}

void DrawVelocity()
{
	fluid->DrawVelocity(win_x ,win_y );
}

void display(void) 
{      

	fluid->SetDrawModeOn(win_x ,win_y);

	if(drawDensity)
	{
		fluid->DrawDensity(	win_x ,win_y );	
	} 
	else if(drawVelocity)
	{
		fluid->DrawVelocity(win_x ,win_y );	
	}
	else if(drawVorticity)
	{
		fluid->DrawVorticity(win_x ,win_y );	
	}

	//particle->SetDrawModeOn(win_x ,win_y);
	//particle->Draw();
	//particle->Update(fluid->GetVelocityTexture(), dt);

	// fps calculation
	finish = clock() ;
	double deltaTime = (double)(finish - start) / CLOCKS_PER_SEC ;
	timeSinceLastUpdate += deltaTime;
	framecount ++ ;

	if( timeSinceLastUpdate > updateInterval)
	{
		fps = framecount / timeSinceLastUpdate ;

		framecount = 0;
		timeSinceLastUpdate -= updateInterval;
	}


	start = clock() ;

	PrintFPS();

	glutSwapBuffers();
}

void reshape(int width, int height ) 
{

	glutReshapeWindow( width, height );
	
	win_x = width;
	win_y = height;
}

void keyboard(unsigned char key, int x, int y)
{

     switch (key) {
     case 3:             /* Ctrl-C */
     case 27:            /* ESC */
	 case 'q':
		 
         exit(0);
	 break;

	 case '1':
		 drawDensity = true;
		 drawVelocity = false;		
		 drawVorticity = false;
		 break;

	case '2':
		drawDensity = false;
		drawVelocity = true;
		drawVorticity = false;
		break;
	case '3':
		drawDensity = false;
		drawVelocity = false;		
		drawVorticity = true;
		break;
		
	 case 'b':
	 case 'B':
		 fluid->ToggleBouayncy();
		  break;

	 case 'i':
	 case 'I':
		 fluid->ToggleInterpolation();
		  break;

	 case 'v':
	 case 'V':
		 fluid->ToggleVorticity();
		  break;
     	 
     }
}

void mouse(int button, int state, int x, int y) 
{
	omx = mx = x;
	omx = my = y;

	mouse_down[button] = state == GLUT_DOWN;
}

void motion(int x, int y) 
{
	mx = x;
	my = y;
}
void ExitFunction()
{	
	delete fluid;
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );

    glutInitWindowPosition(100, 100);
    glutInitWindowSize( win_x, win_y);
    glutCreateWindow("Fluid");
    Init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
	InitGLEW();
	
	fluid = new FluidSolver(fluidResX, fluidResY, 2,  1.f, 0.099, 10.0f);
	// particle = new ParticleSolver(particleX, particleY);
    
	glutIdleFunc(Idle);
    glutReshapeFunc(reshape);
	
	//glutFullScreen();
	atexit(ExitFunction);
    glutMainLoop();
	
}



	
