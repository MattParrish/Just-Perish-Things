// cs381Homework3B.cpp
// Matthew Parrish
// October 7, 2014
//
// For CS 381, Fall 2014
// Homework Assignment 3B
//   - Make something interactive, animated, 
//       having at least two indipendant objects,
//       and with hidden surface removal in 3D.


// Based on manip2.cpp, by Glenn G. Chappell
// Original documentation below:
	// manip2.cpp
	// Glenn G. Chappell
	// 29 Sep 2014
	//
	// For CS 381 Fall 2014
	// Improved 3-D Object Manipulation
	// Based on manip.cpp

// OpenGL/GLUT includes - DO THESE FIRST
#include <cstdlib>       // Do this before GL/GLUT includes
using std::exit;
#ifndef __APPLE__
# include <GL/glut.h>    // Includes OpenGL headers as well
#else
# include <GLUT/glut.h>  // Apple puts glut.h in a different place
#endif

// Other includes
#include "lib381/bitmapprinter.h"
                         // For class BitmapPrinter
#include <iostream>
using std::cerr;
using std::endl;


// Global variables
// Keyboard
const int ESCKEY = 27;         // ASCII value of Escape

// Window/viewport
const int startwinsize = 600;  // Start window width & height (pixels)
int winw = 1, winh = 1;        // Window width, height (pixels)
                               //  (Initialize to avoid spurious errors)

// Objects
const double angstep = 10.0;   // Amt to rotate on single keypress (deg)
GLdouble savedrots[16];        // Saved rotation matrix for object
                               //  (OpenGL format)

double savetime;               // Time of previous movement (sec)

const double polyHalfSide = 0.5; // 23gon's half width

double cageSize;               // Sidelength of the bounding cage.
double polyVelocX;             // X velocity of the 23gon
double polyVelocY;             // Y velocity of the 23gon
double polyVelocZ;             // Z velocity of the 23gon
double polyPosX;               // X position of the 23gon
double polyPosY;               // Y position of the 23gon
double polyPosZ;               // Z position of the 23gon

// drawSquare
// Draws a filled square, using current GL states,
//  in the x,y plane, centered at the origin, aligned w/ x,y axes,
//  with side 2.
void drawSquare()
{
    	glBegin(GL_POLYGON);
        	glVertex2d(-1., -1.);
        	glVertex2d( 1., -1.);
        	glVertex2d( 1.,  1.);
        	glVertex2d(-1.,  1.);
    	glEnd();
}

// drawTriangle
// Draws a filled triangle, using current GL states,
//  in the x,y plane, centered at the origin, aligned w/ x,y axes,
//  with width and height of 2.
void drawTriangle()
{
	glBegin(GL_TRIANGLES);
        	glVertex2d( 0.,  1.);
        	glVertex2d(-1., -1.);
        	glVertex2d( 1., -1.);
    	glEnd();
}

// drawCube()
// Draws a cube, centered at origin, side length 2, axis aligned,
//  each face black.
void drawCube()
{
	// +x face
    	glPushMatrix();
    	glTranslated(1., 0., 0.);
    	glRotated(90., 0.,1.,0.);
	glScaled(1.,1.,1.);
    	glColor3d(0., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// -x face
    	glPushMatrix();
    	glTranslated(-1., 0., 0.);
    	glRotated(90., 0.,1.,0.);
	glScaled(1.,1.,1.);
    	glColor3d(0., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// +y face
    	glPushMatrix();
    	glTranslated(0., 1., 0.);
    	glRotated(90., 1.,0.,0.);
	glScaled(1.,1.,1.);
    	glColor3d(0., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// -y face
    	glPushMatrix();
    	glTranslated(0., -1., 0.);
    	glRotated(90., 1.,0.,0.);
	glScaled(1.,1.,1.);
    	glColor3d(0., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// +z face
    	glPushMatrix();
    	glTranslated(0., 0., 1.);
	glScaled(1.,1.,1.);
    	glColor3d(0., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// -z face
    	glPushMatrix();
    	glTranslated(0., 0., -1.);
	glScaled(1.,1.,1.);
    	glColor3d(0., 0., 0.);
    	drawSquare();
    	glPopMatrix();
}

// drawCage()
// Draws a cube outline of cube, centered at origin, side length cageSize, 
//    axis aligned.
void drawCage()
{
	glPushMatrix();
	glTranslated(cageSize,0.,cageSize);
	glScaled(0.1,cageSize,0.1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(cageSize,0.,-cageSize);
	glScaled(0.1,cageSize,0.1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(cageSize,-cageSize,0.);
	glScaled(0.1,0.1,cageSize);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(cageSize,cageSize,0.);
	glScaled(0.1,0.1,cageSize);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-cageSize,0.,cageSize);
	glScaled(0.1,cageSize,0.1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-cageSize,0.,-cageSize);
	glScaled(0.1,cageSize,0.1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-cageSize,-cageSize,0.);
	glScaled(0.1,0.1,cageSize);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-cageSize,cageSize,0.);
	glScaled(0.1,0.1,cageSize);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.,-cageSize,cageSize);
	glScaled(cageSize,0.1,0.1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.,cageSize,cageSize);
	glScaled(cageSize,0.1,0.1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.,-cageSize,-cageSize);
	glScaled(cageSize,0.1,0.1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.,cageSize,-cageSize);
	glScaled(cageSize,0.1,0.1);
	drawCube();
	glPopMatrix();
}

// draw26Gon()
// Draws a 26-gon, centered at the origin, with overall width of 2, centered at the origin,
//  axis aligned, colors alternating red, blue, and green. 
void draw26Gon()
{
	// +x face
    	glPushMatrix();
    	glTranslated(1., 0., 0.);
    	glRotated(90., 0.,1.,0.);
	glScaled(polyHalfSide,polyHalfSide,polyHalfSide);
    	glColor3d(1., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// -x face
    	glPushMatrix();
    	glTranslated(-1., 0., 0.);
    	glRotated(90., 0.,1.,0.);
	glScaled(polyHalfSide,polyHalfSide,polyHalfSide);
    	glColor3d(1., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// +y face
    	glPushMatrix();
    	glTranslated(0., 1., 0.);
    	glRotated(90., 1.,0.,0.);
	glScaled(polyHalfSide,polyHalfSide,polyHalfSide);
    	glColor3d(1., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// -y face
    	glPushMatrix();
    	glTranslated(0., -1., 0.);
    	glRotated(90., 1.,0.,0.);
	glScaled(polyHalfSide,polyHalfSide,polyHalfSide);
    	glColor3d(1., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// +z face
    	glPushMatrix();
    	glTranslated(0., 0., 1.);
	glScaled(polyHalfSide,polyHalfSide,polyHalfSide);
    	glColor3d(1., 0., 0.);
    	drawSquare();
    	glPopMatrix();

    	// -z face
    	glPushMatrix();
    	glTranslated(0., 0., -1.);
	glScaled(polyHalfSide,polyHalfSide,polyHalfSide);
    	glColor3d(1., 0., 0.);
    	drawSquare();
    	glPopMatrix();

	// -z/+x face
    	glPushMatrix();
    	glTranslated(0.75, 0., -0.75);
	glScaled(0.375,polyHalfSide,0.375);
	glRotated(135., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// -z/-x face
    	glPushMatrix();
    	glTranslated(-0.75, 0., -0.75);
	glScaled(0.375,polyHalfSide,0.375);
	glRotated(45., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// +z/-x face
    	glPushMatrix();
    	glTranslated(-0.75, 0., 0.75);
	glScaled(0.375,polyHalfSide,0.375);
	glRotated(135., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// +z/+x face
    	glPushMatrix();
    	glTranslated(0.75, 0., 0.75);
	glScaled(0.375,polyHalfSide,0.375);
	glRotated(45., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// +y/+z face
    	glPushMatrix();
    	glTranslated(0., 0.75, 0.75);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(150., 1.,0.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// +y/-z face
    	glPushMatrix();
    	glTranslated(0., 0.75, -0.75);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(30., 1.,0.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();
	
	// -y/+z face
    	glPushMatrix();
    	glTranslated(0., -0.75, 0.75);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(30., 1.,0.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// -y/-z face
    	glPushMatrix();
    	glTranslated(0., -0.75, -0.75);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(150., 1.,0.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// +y/+x face
    	glPushMatrix();
    	glTranslated(0.75, 0.75, 0.);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(30., 0.,0.,1.);
	glRotated(90., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// +y/-x face
    	glPushMatrix();
    	glTranslated(-0.75, 0.75, 0.);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(150., 0.,0.,1.);
	glRotated(90., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();
	
	// -y/+x face
    	glPushMatrix();
    	glTranslated(0.75, -0.75,0.);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(150., 0.,0.,1.);
	glRotated(90., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// -y/-x face
    	glPushMatrix();
    	glTranslated(-0.75, -0.75, 0.);
	glScaled(polyHalfSide,0.3,polyHalfSide);
	glRotated(30., 0.,0.,1.);
	glRotated(90., 0.,1.,0.);
    	glColor3d(0.,0.,1.);
    	drawSquare();
    	glPopMatrix();

	// +x/+y/+z triangle
	glPushMatrix();
	glTranslated(0.62,0.75,0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(45.,0.,1.,0.);
	glRotated(35.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();

	// +x/-y/+z triangle
	glPushMatrix();
	glTranslated(0.62,-0.75,0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(45.,0.,1.,0.);
	glRotated(145.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();

	// -x/+y/+z triangle
	glPushMatrix();
	glTranslated(-0.62,0.75,0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(-45.,0.,1.,0.);
	glRotated(35.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();

	// -x/-y/+z triangle
	glPushMatrix();
	glTranslated(-0.62,-0.75,0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(-45.,0.,1.,0.);
	glRotated(145.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();

	// +x/+y/-z triangle
	glPushMatrix();
	glTranslated(0.62,0.75,-0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(-45.,0.,1.,0.);
	glRotated(-35.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();

	// +x/-y/-z triangle
	glPushMatrix();
	glTranslated(0.62,-0.75,-0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(-45.,0.,1.,0.);
	glRotated(-145.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();

	// -x/+y/-z triangle
	glPushMatrix();
	glTranslated(-0.62,0.75,-0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(45.,0.,1.,0.);
	glRotated(-35.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();

	// -x/-y/-z triangle
	glPushMatrix();
	glTranslated(-0.62,-0.75,-0.62);
	glScaled(0.37,0.33,0.37);
	glColor3d(0.,1.,0.);
	glRotated(45.,0.,1.,0.);
	glRotated(-145.,-1.,0.,0.);
	drawTriangle();
	glPopMatrix();
}


// myDisplay
// The GLUT display function
void myDisplay()
{
    	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// Draw objects
    	glEnable(GL_DEPTH_TEST);
    	glLoadIdentity();
    	glTranslated(0., 0., -10.);
	
	// Bounding Cage
	drawCage();

    	// Rotating cube
    	glMultMatrixd(savedrots);
	glTranslated(polyPosX, polyPosY, polyPosZ);
    	draw26Gon();

    	// Draw documentation
    	glDisable(GL_DEPTH_TEST);
    	glLoadIdentity();
    	glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
    	glPushMatrix();
    	glLoadIdentity();
    	gluOrtho2D(0., double(winw), 0., double(winh));
    	glColor3d(0.8, 0., 0.);        // Black text
    	BitmapPrinter p(20., winh-20., 20.);
    	p.print("26-gon In A Box");
    	p.print("Arrows   Rotate 26-gon");
	p.print("W/A/S/D  Apply momentum in the X/Y plane. Hold to increase");
	p.print("F/R      Apply momentum in the Z plane. Hold to increase");
	p.print("  (Note: These apply only if the object has not been rotated)");
	p.print("+/-      increase/decrease bounding box's size");
    	p.print("Z        Reset everything");
    	p.print("Esc      Quit");
    	glPopMatrix();                // Restore prev projection
    	glMatrixMode(GL_MODELVIEW);

    	glutSwapBuffers();
}


// myIdle
// The GLUT idle function
void myIdle()
{
	// Compute elapsed time since last movement
    	double currtime = glutGet(GLUT_ELAPSED_TIME) / 1000.;
    	double elapsedtime = currtime - savetime;
    	savetime = currtime;
    	if (elapsedtime > 0.1)
        	elapsedtime = 0.1;

	// Animate the movement of the 26-gon
	polyPosX += polyVelocX * elapsedtime;
	polyPosY += polyVelocY * elapsedtime;
	polyPosZ += polyVelocZ * elapsedtime;
	glutPostRedisplay();


	// Bounds checking logic and bouncing logic.
	if(polyPosX + 2*polyHalfSide >= cageSize)
	{
		polyPosX = cageSize - 2*polyHalfSide;
		polyVelocX *= -1.;
	}
	if(polyPosX - 2*polyHalfSide <= -cageSize)
	{
		polyPosX = -cageSize + 2*polyHalfSide;
		polyVelocX *= -1.;
	}
	if(polyPosY + 2*polyHalfSide >= cageSize)
	{
		polyPosY = cageSize - 2*polyHalfSide;
		polyVelocY *= -1.;
	}
	if(polyPosY - 2*polyHalfSide <= -cageSize)
	{
		polyPosY = -cageSize + 2*polyHalfSide;
		polyVelocY *= -1.;
	}
	if(polyPosZ + 2*polyHalfSide >= cageSize)
	{
		polyPosZ = cageSize - 2*polyHalfSide;
		polyVelocZ *= -1.;
	}
	if(polyPosZ - 2*polyHalfSide <= -cageSize)
	{
		polyPosZ = -cageSize + 2*polyHalfSide;
		polyVelocZ *= -1.;
	}

    	// Print OpenGL errors, if there are any (for debugging)
    	static int error_count = 0;
    	if (GLenum err = glGetError())
    	{
        	++error_count;
        	cerr << "OpenGL ERROR " << error_count << ": "
             	     << gluErrorString(err) << endl;
    	}
}


// resetObjRotation
// Set global savedrots to an identity matrix.
// Resets the 26-gon's position and velocity to 0,
// and the bounding cage to its initial value.
// Must be in model/view mode.
void resetObjRotation()
{
	polyPosY = 0.;
	polyVelocY = 0.;
	polyPosX = 0.;
	polyVelocX = 0.;
	polyPosZ = 0.;
	polyVelocZ = 0.;
	cageSize = 3.0;

    	glLoadIdentity();
    	glGetDoublev(GL_MODELVIEW_MATRIX, savedrots);
}


// myKeyboard
// The GLUT keyboard function
void myKeyboard(unsigned char key, int x, int y)
{
    	switch (key)
    	{
	case '=':      // + key: Increase bounding cage size.
	case '+':
		cageSize += 0.2;
		if(cageSize >= 6.0)
			cageSize = 6.0;
		glutPostRedisplay();
		break;	
	case '-':      // - key: Decrase bounding cage size.
	case '_':
		cageSize -= 0.2;
		if(cageSize <= 1.2)
			cageSize = 1.2;
		glutPostRedisplay();
		break;
	case 'w':      // w key: Increase 26-gon's Y velocity.
	case 'W':
		polyVelocY += 0.1;
		glutPostRedisplay();
		break;
	case 's':      // s key: Decrease 26-gon's Y velocity.
	case 'S':
		polyVelocY -= 0.1;
		glutPostRedisplay();
		break;
	case 'a':      // a key: Decrease 26-gon's X velocity.
	case 'A':
		polyVelocX -= 0.1;
		glutPostRedisplay();
		break;
	case 'd':      // d key: Increase 26-gon's X velocity.
	case 'D':
		polyVelocX += 0.1;
		glutPostRedisplay();
		break;
	case 'r':      // r key: Decrease 26-gon's Z velocity.
	case 'R':
		polyVelocZ -= 0.1;
		glutPostRedisplay();
		break;
	case 'f':      // r key: Increase 26-gon's Z velocity.
	case 'F':
		polyVelocZ += 0.1;
		glutPostRedisplay();
		break;
    	case 'z':     // Z: reset rotations
    	case 'Z':
        	resetObjRotation();
        	glutPostRedisplay();
        	break;
    	case ESCKEY:  // Esc: quit
        	exit(0);
        	break;
    	}
}


// mySpecial
// The GLUT special function
void mySpecial(int key, int x, int y)
{
    	switch (key)
    	{
    	case GLUT_KEY_RIGHT:  // right: rotation
        	glLoadIdentity();
        	glRotated(angstep, 0.,1.,0.);
        	glMultMatrixd(savedrots);
        	glGetDoublev(GL_MODELVIEW_MATRIX, savedrots);
        	glutPostRedisplay();
        	break;
    	case GLUT_KEY_LEFT:   // left: rotation
        	glLoadIdentity();
        	glRotated(-angstep, 0.,1.,0.);
        	glMultMatrixd(savedrots);
        	glGetDoublev(GL_MODELVIEW_MATRIX, savedrots);
        	glutPostRedisplay();
        	break;
    	case GLUT_KEY_UP:     // up: rotation
        	glLoadIdentity();
        	glRotated(-angstep, 1.,0.,0.);
        	glMultMatrixd(savedrots);
        	glGetDoublev(GL_MODELVIEW_MATRIX, savedrots);
        	glutPostRedisplay();
        	break;
    	case GLUT_KEY_DOWN:   // down: rotation
        	glLoadIdentity();
        	glRotated(angstep, 1.,0.,0.);
        	glMultMatrixd(savedrots);
        	glGetDoublev(GL_MODELVIEW_MATRIX, savedrots);
        	glutPostRedisplay();
        	break;
    	}
}


// myReshape
// The GLUT reshape function
void myReshape(int w, int h)
{
    	// Set viewport & save window dimensions in globals
    	glViewport(0, 0, w, h);
    	winw = w;
    	winh = h;

    	// Set up projection
    	// Standard perspective projection
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluPerspective(60., double(w)/h, 0.1, 20.0);

    	glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
}


// init
// Initialize GL states & global data
// Called by main after window creation
void init()
{
    	// Objects
    	glMatrixMode(GL_MODELVIEW);
    	resetObjRotation();

	polyVelocX = 0.;
	polyVelocY = 0.;
	polyVelocZ = 0.;
	polyPosX = 0.;
	polyPosY = 0.;
	polyPosZ = 0.;
	cageSize = 3.0;

    	// OpenGL Stuff
    	// (Nothing here)
}


int main(int argc, char ** argv)
{
    	// Initialize OpenGL/GLUT
    	glutInit(&argc, argv);
    	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    	// Make a window
    	glutInitWindowSize(startwinsize, startwinsize);
    	glutInitWindowPosition(50, 50);
    	glutCreateWindow("Matthew Parrish - Attractive Llama");

    	// Initialize GL states & register GLUT callbacks
    	init();
    	glutDisplayFunc(myDisplay);
    	glutIdleFunc(myIdle);
    	glutKeyboardFunc(myKeyboard);
    	glutSpecialFunc(mySpecial);
    	glutReshapeFunc(myReshape);
	
    	// Do something
    	glutMainLoop();

    	return 0;
}
