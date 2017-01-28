// cs381Homework2.cpp
// Matthew Parrish
// 20 October 2014
//
// For CS 381 Fall 2014
// Assignment 4A. 
// Write a graphical application that allows interaction with an animated 3-D hierarchical object

// Based on heir.cpp, by Glenn G. Chappell
// Original documentation below:
	// hier.cpp
	// Glenn G. Chappell
	// 3 Oct 2014
	//
	// For CS 381 Fall 2014
	// Hierarchical Object

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
#include <string>
using std::string;
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
double savetime;               // Time of previous movement (sec)

// Object as a whole
const double angstep = 10.0;   // Amt to rotate on single keypress (deg)
GLdouble savedrots[16];        // Saved rotation matrix for object
                               //  (OpenGL format)

int howManyCrosses = 1;        // How many 3D crosses to draw
bool autoRotate;   

// 3D cross data
double rotateAngleY = 0.0;     
double rotateAngleX = 0.0;     // rotation angle for the 3D crosses.
const double halfCrossBeamWidth = 0.1;
double crossScaleSize = 0.5;
double normalizerScaler = (crossScaleSize + halfCrossBeamWidth)*10.;

const int NUMANGLES = 7;

// Arrays for the fractal rotations
double angle[NUMANGLES];       // Current movement angle
const double astart[NUMANGLES] = { 0., 0., 0., 0., 10., 0., 0. };
                               // Initial value of movement angle
const double astep[NUMANGLES] = { 2., 2., 2., 5., 2., 2., 2. };
                               // Step for movement angle

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


// drawCube()
// Draws a cube, centered at origin, side length 2, axis aligned,
// each face a different color.
void drawCube()
{
    // +x face
    glPushMatrix();
    glTranslated(1., 0., 0.);
    glRotated(90., 0.,1.,0.);
    glColor3d(1., 0., 0.);
    drawSquare();
    glPopMatrix();

    // -x face
    glPushMatrix();
    glTranslated(-1., 0., 0.);
    glRotated(90., 0.,1.,0.);
    glColor3d(0., 1., 1.);
    drawSquare();
    glPopMatrix();

    // +y face
    glPushMatrix();
    glTranslated(0., 1., 0.);
    glRotated(90., 1.,0.,0.);
    glColor3d(0., 1., 0.);
    drawSquare();
    glPopMatrix();

    // -y face
    glPushMatrix();
    glTranslated(0., -1., 0.);
    glRotated(90., 1.,0.,0.);
    glColor3d(1., 0., 1.);
    drawSquare();
    glPopMatrix();

    // +z face
    glPushMatrix();
    glTranslated(0., 0., 1.);
    glColor3d(0., 0., 1.);
    drawSquare();
    glPopMatrix();

    // -z face
    glPushMatrix();
    glTranslated(0., 0., -1.);
    glColor3d(1., 1., 0.);
    drawSquare();
    glPopMatrix();
}


// drawCrosses
// Recursively draws a set of 3D axis-like-crosses, placing one at each of the 6 ends of the
//   previous crosses. Centered at the origin, aligned with the x/y axis. 
int drawCrosses(int recCounter)
{
	// Base Case
	if(recCounter <= 1)
	{
		glPushMatrix();
		glScaled(crossScaleSize+halfCrossBeamWidth, halfCrossBeamWidth, halfCrossBeamWidth);
		drawCube();
		glPopMatrix();

		glPushMatrix();
		glScaled(halfCrossBeamWidth, crossScaleSize+halfCrossBeamWidth, halfCrossBeamWidth);
		drawCube();
		glPopMatrix();

		glPushMatrix();
		glScaled(halfCrossBeamWidth, halfCrossBeamWidth, crossScaleSize+halfCrossBeamWidth);
		drawCube();
		glPopMatrix();

		return 0;
	}

    glPushMatrix();
    glScaled(crossScaleSize+halfCrossBeamWidth, halfCrossBeamWidth, halfCrossBeamWidth);
    drawCube();

		glPushMatrix();
		glTranslated(1.,0.,0.);
		glScaled(1., normalizerScaler, normalizerScaler); 
		glRotated(rotateAngleY, 0.,1.,1.);
		glRotated(rotateAngleX, 1.,0.,1.);
		drawCrosses(recCounter-1);          // Recursive call
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1.,0.,0.);
		glScaled(1., normalizerScaler, normalizerScaler);
		glRotated(rotateAngleY, 0.,1.,1.);
		glRotated(rotateAngleX, 1.,0.,1.);
		drawCrosses(recCounter-1);          // Recursive call
		glPopMatrix();

    glPopMatrix();

	glPushMatrix();
    glScaled(halfCrossBeamWidth, crossScaleSize+halfCrossBeamWidth, halfCrossBeamWidth);
    drawCube();

		glPushMatrix();
		glTranslated(0.,1.,0.);
		glScaled(normalizerScaler, 1., normalizerScaler);
		glRotated(rotateAngleY, 0.,1.,1.);
		glRotated(rotateAngleX, 1.,0.,1.);
		drawCrosses(recCounter-1);          // Recursive call
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.,-1.,0.);
		glScaled(normalizerScaler, 1., normalizerScaler);
		glRotated(rotateAngleY, 0.,1.,1.);
		glRotated(rotateAngleX, 1.,0.,1.);
		drawCrosses(recCounter-1);          // Recursive call
		glPopMatrix();

    glPopMatrix();

	glPushMatrix();
    glScaled(halfCrossBeamWidth, halfCrossBeamWidth, crossScaleSize+halfCrossBeamWidth);
    drawCube();

		glPushMatrix();
		glTranslated(0.,0.,1.);
		glScaled(normalizerScaler, normalizerScaler, 1.);
		glRotated(rotateAngleY, 0.,1.,1.);
		glRotated(rotateAngleX, 1.,0.,1.);
		drawCrosses(recCounter-1);          // Recursive call
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.,0.,-1.);
		glScaled(normalizerScaler, normalizerScaler, 1.);
		glRotated(rotateAngleY, 0.,1.,1.);
		glRotated(rotateAngleX, 1.,0.,1.);
		drawCrosses(recCounter-1);          // Recursive call
		glPopMatrix();

    glPopMatrix();

	return 0;
}


// drawFractal
// Sets up for the calling of the recursive cross-drawing function, then calls it
void drawFractal()
{
	glPushMatrix();
	glScaled(1.5,1.5,1.5);
	glRotated(rotateAngleY, 0.,1.,0.);
	glRotated(rotateAngleX, 1.,0.,1.);
    drawCrosses(howManyCrosses);
    glPopMatrix();
}


// myDisplay
// The GLUT display function
void myDisplay()
{
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw objects

    // Set up 3-D
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glTranslated(0., 0., -6.);  // Camera transformation

    // Draw the fractal Cross thing, rotated as user directs
    glMultMatrixd(savedrots);
	drawFractal();

    // Draw documentation
    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0., double(winw), 0., double(winh));
    glColor3d(0., 0., 0.);        // Black text
    BitmapPrinter p(20., winh-20., 20.);
    p.print("Arrows   Rotate object");
    p.print("Space    Reset object rotation");
    p.print("X        Reset rotation and cross-count");
    p.print("W/S      Rotate Up/Down");
	p.print("A/D      Rotate Left/Right");
	p.print("R        Auto-Rotate");
	p.print("+/-      Add/Remove crosses (Limited up to 4)");
	p.print("~        Adds more crosses, no limits.");
	p.print("           Will destroy everything you love.");
    p.print("");
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
	
    // Auto-Rotate?
    if (autoRotate)
    {
		rotateAngleY += 20.0 * elapsedtime;
		rotateAngleX += 20.0 * elapsedtime;
		glutPostRedisplay();
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
void resetObjRotation()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(90., 0.,0.,1.);
    glGetDoublev(GL_MODELVIEW_MATRIX, savedrots);
	rotateAngleY = 0.;
	rotateAngleX = 0.;
}


// resetAll
// Set all rotations to initial values.
void resetAll()
{
    resetObjRotation();
    for (int i = 0; i < NUMANGLES; ++i)
    {
        angle[i] = astart[i];
    }
    autoRotate = false;
	howManyCrosses = 1;
}


// myKeyboard
// The GLUT keyboard function
void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':     // Space: reset object rotation
        resetObjRotation();
        glutPostRedisplay();
        break;
    case 'x':     // X: reset all
    case 'X':
        resetAll();
        glutPostRedisplay();
        break;
    case '+':     // +: Increases how many layers of crosses are drawn
	case '=':     //      Limited to 4
		howManyCrosses += 1;
		if(howManyCrosses >= 4)
			howManyCrosses = 4;
		glutPostRedisplay();
        break;
    case '-':     // -: Decreases how many layers are drawn
	case '_':
		howManyCrosses -= 1;
		if(howManyCrosses <= 1)
			howManyCrosses = 1;
		glutPostRedisplay();
        break;
    case '~':     // ~: Increases how many layers of crosses are drawn
	case '`':     //      No limits
		howManyCrosses += 1;
		glutPostRedisplay();
        break;
	case 's':     // S: decreases the Y-ish angle to rotate by
    case 'S': 
		rotateAngleY -= 1.;
		glutPostRedisplay();
        break;
	case 'w':     // W: increases the Y-ish angle to rotate by
    case 'W':
		rotateAngleY += 1.; 
		glutPostRedisplay();
        break;
	case 'a':     // A: decreases the X-ish angle to rotate by
    case 'A':
		rotateAngleX -= 1.;
		glutPostRedisplay();
        break;
	case 'd':     // D: increases the X-ish angle to rotate by
    case 'D':
		rotateAngleX += 1.;
		glutPostRedisplay();
        break;
	case 'r':     // R: toggles the auto-rotate
	case 'R':
		autoRotate = !autoRotate;
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
    gluPerspective(60., double(w)/h, 0.1, 10.0);

    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
}


// init
// Initialize GL states & global data
// Called by main after window creation
void init()
{
    // Objects
    savetime = glutGet(GLUT_ELAPSED_TIME) / 1000.;
    resetAll();

    // OpenGL Stuff
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char ** argv)
{
    // Initialize OpenGL/GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Make a window
    glutInitWindowSize(startwinsize, startwinsize);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Matthew Parrish - Boorish Banjo");

    // Initialize GL states & register GLUT callbacks
    init();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);
    glutReshapeFunc(myReshape);

    // Do something
    glutMainLoop();

    return 9001;
}
