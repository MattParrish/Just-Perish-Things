// cs381Homework2.cpp
// Matthew Parrish
// 25 September 2014
//
// For CS 381 Fall 2014
// Assignment 2. 
// Write a graphical application that does smooth animation and makes use of the mouse

// Based on clackdrag2.cpp, by Glenn G. Chappell
// Original documentation below:
	// clickdrag2.cpp
	// Glenn G. Chappell
	// 19 Sep 2014
	//
	// For CS 381 Fall 2014
	// Improved Click-and-Drag (object stays in window)
	// Based on clickdrag.cpp

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
double cam_xmin, cam_xmax, cam_ymin, cam_ymax;
                               // Viewport bdaries in world (cam coords)

// Mouse
double cam_mousex, cam_mousey; // Mouse pos in cam coords
int draggedobj;                // 0: not currently dragging
                               // 1: dragging the plane
							   // 2: dragging the throttle
// Objects
double savetime; 

// Plane
const double planewidth = 0.1; // Plane half side len (cam coords)
double planeposx, planeposy;         // x,y of center of plane (cam coords)
double planedragposx, planedragposy; // Where plane is dragged (cam coords)
                               // Valid when draggedobj == 1
                               // May lie outside window; in this case
                               //  (planeposx, planeposy) is nearest point
                               //  with entire square in window.

// Throttle
const double throttlehalfsize = 0.1; //Throttle half side len (cam coords)
double throttleposx, throttleposy;         // x,y of the center of the throttle (cam coords)
double throttledragposx, throttledragposy; // Where Throttle is dragged (cam coords)
								// Valid when draggedobj == 2

// Clouds
double cloud1Y;
double cloud2Y;
double cloud3Y;
double cloud4Y;
double cloud5Y;
double cloud6Y;
double cloud7Y;

double cloud1Size;
double cloud2Size;
double cloud3Size;
double cloud4Size;
double cloud567Size;

// drawPlane
// Draws a colored plane, using current GL states,
//  in the x,y plane, centered at the origin, aligned w/ x,y axes,
//  with overall width and height of two.
void drawPlane()
{
	// glPushMatrixed added because color is changed, and functions should 'fix'
	//    anything changed.
	glPushMatrix();
	glColor3d(0.7,0.0,0.0);
    glBegin(GL_POLYGON);
        glVertex2d(0.375, 0.875);
		glVertex2d(-0.375, 0.875);
		glVertex2d(-0.375, -0.375);
		glVertex2d(0.375, -0.375);
    glEnd();

	glBegin(GL_TRIANGLES);
		glVertex2d(-0.375, -0.375);
		glVertex2d(0.375, -0.375);
		glVertex2d(0., -1.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2d(-0.675, -1.0);
		glVertex2d(0.675, -1.0);
		glVertex2d(0.675, -0.7);
		glVertex2d(-0.675, -0.7);
	glEnd();

	glColor3d(1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glVertex2d(-1.0, 0.125);
		glVertex2d(1.0, 0.125);
		glVertex2d(1.0, 0.75);
		glVertex2d(-1.0, 0.75);
	glEnd();

	glColor3d(1.0,0.0,0.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);
		glVertex2d(0.0, -1.0);
		glVertex2d(0.0, -0.75);
	glEnd();

	glColor3d(0.6,0.6,0.1);
	glBegin(GL_LINES);
		glVertex2d(0.0, 0.875);
		glVertex2d(0.0, 1.0);
		glVertex2d(-0.34, 1.0);
		glVertex2d(0.34, 1.0);
	glEnd();

	glColor3d(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glVertex2d(-0.2, 0.125);
		glVertex2d(0.2, 0.125);
		glVertex2d(0.2, -0.125);
		glVertex2d(-0.2, -0.125);
	glEnd();
	glPopMatrix();
}

// drawCloud
// Draws a colored Cloud, using current GL states,
//  in the x,y plane, centered at the origin, aligned w/ x,y axes,
//  with width to the right of 1, to the left of 0.75, and height of 1
void drawCloud()
{
	// glPushMatrixed added because color is changed, and functions should 'fix'
	//    anything changed.
	glPushMatrix();
	glColor3d(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
		glVertex2d(1.0, 0.0);
		glVertex2d(0.75, -0.25);
		glVertex2d(0.25, -0.25);
		glVertex2d(-0.30, -0.5);
		glVertex2d(-0.60, -0.5);
		glVertex2d(-0.75, -0.2);
		glVertex2d(-0.75, 0.0);
		glVertex2d(-0.25, 0.5);
		glVertex2d(0.25, 0.5);
		glVertex2d(0.5, 0.25);
		glVertex2d(0.75, 0.25);
	glEnd();
	glPopMatrix();
}

// drawThrottle
// Draws a box, using current GL states,
//  in the x,y plane, centered at the origin, aligned w/ x,y axes,
//  with width of 2
void drawThrottle()
{
	glBegin(GL_POLYGON);
		glVertex2d(-1.0,-1.0);
		glVertex2d(-1.0, 1.0);
		glVertex2d(1.0, 1.0);
		glVertex2d(1.0, -1.0);
	glEnd();
}

// drawThrottleBox
// Draws an outline of a box, using current GL states,
//  in the x,y plane, centered at the origin, aligned w/ x,y axes,
//  with overall width and height of two.
void drawThrottleBox()
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-1.0,-1.0);
		glVertex2d(-1.0, 1.0);
		glVertex2d(1.0, 1.0);
		glVertex2d(1.0, -1.0);
	glEnd();
}

// myDisplay
// The GLUT display function
void myDisplay()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw objects

    // Draw Plane
    glLoadIdentity();
	glPushMatrix();
    glTranslated(planeposx, planeposy, 0.);
    glScaled(planewidth, planewidth, planewidth);
    drawPlane();
	glPopMatrix();
	
	// ****** CLOUDS ****** \\

	glPushMatrix();
	glTranslated(0.0, cloud1Y, 0.);
	glScaled(cloud1Size, cloud1Size, cloud1Size);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5, cloud2Y, 0.);
	glScaled(cloud2Size, cloud2Size, cloud2Size);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, cloud3Y, 0.);
	glScaled(cloud3Size, cloud3Size, cloud3Size);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.75, cloud4Y, 0.);
	glScaled(cloud4Size, cloud4Size, cloud4Size);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, cloud5Y, 0.);
	glScaled(cloud567Size, cloud567Size, cloud567Size);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6, cloud6Y, 0.);
	glScaled(cloud567Size, cloud567Size, cloud567Size);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, cloud7Y, 0.);
	glScaled(cloud567Size, cloud567Size, cloud567Size);
	drawCloud();
	glPopMatrix();

	// ****** END CLOUDS ****** \\

	// Draw Throttle's Bounding Box
	glPushMatrix();
	glTranslated(throttleposx, -0.5, 0.);
	glScaled(throttlehalfsize, 0.5, throttlehalfsize);
	glColor3d(1.0,1.0,0.0);
	drawThrottleBox();
	glPopMatrix();

	// Draw Throttle
	glPushMatrix();
	glTranslated(throttleposx, throttleposy, 0.);
	glScaled(throttlehalfsize, throttlehalfsize, throttlehalfsize);
	glColor3d(0.0,1.0,0.0);
	drawThrottle();
	glPopMatrix();

    // Draw documentation
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0., double(winw), 0., double(winh));
    glColor3d(0., 0., 0.);        // Black text
    BitmapPrinter p(20., winh-20., 20.);
    p.print("Click and Drag to Fly The Airplane");
    p.print("Click and Drag the Throttle to increase");
    p.print("   or decrease speed.");
    p.print("Esc - Quit");
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

	// Cloud Movement Logic
	elapsedtime *= throttleposy+2.0;

	cloud1Y -= elapsedtime;
	cloud2Y -= elapsedtime;
	cloud3Y -= elapsedtime;
	cloud4Y -= elapsedtime;
	cloud5Y -= elapsedtime;
	cloud6Y -= elapsedtime;
	cloud7Y -= elapsedtime;
	if(cloud1Y < (-1.0 - cloud1Size))
		cloud1Y = 1.0 + cloud1Size;
	if(cloud2Y < (-1.0 - cloud2Size))
		cloud2Y = 1.0 + cloud2Size;
	if(cloud3Y < (-1.0 - cloud3Size))
		cloud3Y = 1.0 + cloud3Size;
	if(cloud4Y < (-1.0 - cloud4Size))
		cloud4Y = 1.0 + cloud4Size;
	if(cloud5Y < (-1.0 - cloud567Size))
		cloud5Y = 1.0 + cloud567Size;
	if(cloud6Y < (-1.0 - cloud567Size))
		cloud6Y = 1.0 + cloud567Size;
	if(cloud7Y < (-1.0 - cloud567Size))
		cloud7Y = 1.0 + cloud567Size;
	glutPostRedisplay();

    // Print OpenGL errors, if there are any (for debugging)
    static int error_count = 0;
    if (GLenum err = glGetError())
    {
        ++error_count;
        cerr << "OpenGL ERROR " << error_count << ": "
             << gluErrorString(err) << endl;
    }
}


// myKeyboard
// The GLUT keyboard function
void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ESCKEY:  // Esc: quit
        exit(0);
        break;
    }
}


// fixPlaneLoc
// If the plane [center (planeposx, planeposy), half width = planewidth]
//  is not entirely within window [boundaries cam_xmin, ...], then
//  place the plane into window, at nearest possible location.
void fixPlaneLoc()
{
    if (planeposx < cam_xmin+planewidth)
        planeposx = cam_xmin+planewidth;
    if (planeposx > cam_xmax-planewidth)
        planeposx = cam_xmax-planewidth;
    if (planeposy < cam_ymin+planewidth)
        planeposy = cam_ymin+planewidth;
    if (planeposy > cam_ymax-planewidth)
        planeposy = cam_ymax-planewidth;
}

// fixThrottleLoc
// If throttle [center (throttleposx, throttleposy), half width = throttlehalfsize]
//  is not entirely within slider [boundaries 0.8 +/- throttlehalfsize, -1.0 -> -throttlehalfsize], then
//  place square into slider, at nearest possible location.
void fixThrottleLoc()
{
	if (throttleposy < cam_ymin + throttlehalfsize)
        throttleposy = cam_ymin + throttlehalfsize;
    if (throttleposy > -throttlehalfsize)
        throttleposy = -throttlehalfsize;
    throttleposx = 0.8;
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
    // Save max/min x/y coords in globals
    // Projection is orthographic. Aspect ratio is correct,
    // and region -1..1 in x, y always just fits in viewport.
    if (w > h)
    {
        cam_xmin = -double(w)/h;
        cam_xmax = double(w)/h;
        cam_ymin = -1.;
        cam_ymax = 1.;
    }
    else
    {
        cam_xmin = -1.;
        cam_xmax = 1.;
        cam_ymin = -double(h)/w;
        cam_ymax = double(h)/w;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(cam_xmin, cam_xmax, cam_ymin, cam_ymax);

    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode

    // Move our square into the window, if it is outside
    fixPlaneLoc();
}


// saveMouse
// Given mouse pos in GLUT format; computes mouse pos in cam coords,
//  saves it in globals cam_mousex, cam_mousey.
// Uses globals winw, winh, cam_xmin, cam_xmax, cam_ymin, cam_ymax.
void saveMouse(int x, int y)
{
    double t;  // Intermediate value of lirping

    t = double(x) / winw;
    cam_mousex = cam_xmin + t * (cam_xmax - cam_xmin);

    t = double(y) / winh;
    cam_mousey = cam_ymax + t * (cam_ymin - cam_ymax);
}


// myMouse
// The GLUT mouse function
void myMouse(int button, int state, int x, int y)
{
    // Save old mouse pos, for relative mouse-movement computation
    double old_cam_mousex = cam_mousex;
    double old_cam_mousey = cam_mousey;

    // Find mouse pos in cam coords (saved in cam_mousex, cam_mousey)
    saveMouse(x, y);

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)  // Left mouse down
        {
            // Extent test for plane
            if (cam_mousex >= planeposx-planewidth
             && cam_mousex <= planeposx+planewidth
             && cam_mousey >= planeposy-planewidth
             && cam_mousey <= planeposy+planewidth)
            {
                // We have a mouse-down on the plane
                draggedobj = 1;
                planedragposx = planeposx;
                planedragposy = planeposy;
                glutPostRedisplay();
            } 
			// Extent test for throttle
            if (cam_mousex >= throttleposx-throttlehalfsize
             && cam_mousex <= throttleposx+throttlehalfsize
             && cam_mousey >= throttleposy-throttlehalfsize
             && cam_mousey <= throttleposy+throttlehalfsize)
            {
                // We have a mouse-down on throttle
                draggedobj = 2;
				throttledragposx = throttleposx;
                throttledragposy = throttleposy;
                glutPostRedisplay();
            }
        }
        else // Left mouse up
        {
            if (draggedobj == 1)  // We were dragging the plane
            {
                // Move the plane by relative mouse movement
                planedragposx += (cam_mousex - old_cam_mousex);
                planedragposy += (cam_mousey - old_cam_mousey);
                planeposx = planedragposx;
                planeposy = planedragposy;
                fixPlaneLoc();
                glutPostRedisplay();
            }
			if (draggedobj == 2)  // We were dragging Throttle
            {
                // Move the throttle by relative mouse movement
                throttledragposx += (cam_mousex - old_cam_mousex);
                throttledragposy += (cam_mousey - old_cam_mousey);
                throttleposx = throttledragposx;
                throttleposy = throttledragposy;
				fixThrottleLoc();
                glutPostRedisplay();
            }
            draggedobj = 0;
        }
    }
}


// myMotion
// The GLUT motion function
void myMotion(int x, int y)
{
    // Save old mouse pos, for relative mouse-movement computation
    double old_cam_mousex = cam_mousex;
    double old_cam_mousey = cam_mousey;

    // Find mouse pos in cam coords (saved in cam_mousex, cam_mousey)
    saveMouse(x, y);

    if (draggedobj == 1)  // We are dragging the plane
    {
        // Move the plane by relative mouse movement
        planedragposx += (cam_mousex - old_cam_mousex);
        planedragposy += (cam_mousey - old_cam_mousey);
        planeposx = planedragposx;
        planeposy = planedragposy;
        fixPlaneLoc();
        glutPostRedisplay();
    }

	if (draggedobj == 2)  // We are dragging the throttle
    {
        // Move the throttle by relative mouse movement
        throttledragposx += (cam_mousex - old_cam_mousex);
        throttledragposy += (cam_mousey - old_cam_mousey);
        throttleposx = throttledragposx;
        throttleposy = throttledragposy;
        fixThrottleLoc();
        glutPostRedisplay();
    }
}


// init
// Initialize GL states & global data
// Called by main after window creation
void init()
{
    // Mouse
    draggedobj = 0;
    cam_mousex = 0.0;
    cam_mousey = 0.0;

    // Objects
    planeposx = 0.0;
    planeposy = 0.0;
	throttleposx = 0.8;
	throttleposy = -0.8;
    savetime = glutGet(GLUT_ELAPSED_TIME) / 1000.;

	// Clouds
	cloud1Y = 1.0;
	cloud2Y = -0.75; 
	cloud3Y = 0.4;
	cloud4Y = 0.0;
	cloud5Y = 0.9;
	cloud6Y = -0.8;
	cloud7Y = -0.1;

	cloud1Size = 0.2;
	cloud2Size = 0.3;
	cloud3Size = 0.25;
	cloud4Size = 0.4;
	cloud567Size = 0.35;

    // OpenGL Stuff
    // (Nothing here)
}


int main(int argc, char ** argv)
{
    // Initialize OpenGL/GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Make a window
    glutInitWindowSize(startwinsize, startwinsize);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS 381 Homework 2 - Rapid Bread");

    // Initialize GL states & register GLUT callbacks
    init();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glutKeyboardFunc(myKeyboard);
    glutReshapeFunc(myReshape);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMotion);

    // Do something
    glutMainLoop();

    return 0;
}
