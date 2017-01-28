// cs381Homework1B.cpp
// Matthew Parrish
// 16 September 2014
//
// For CS 381 Fall 2014
// Assignment 1. 
// Make something using all of the given Primitives.

// Based on kbdtext.cpp, by Glenn G. Chappell
// Original documentation below:
	// kbdtext.cpp
	// VERSION 3
	// Glenn G. Chappell
	// 12 Sep 2014
	//
	// For CS 381 Fall 2014
	// Keyboard & Bitmap Text Demo

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
const int startwinsizeY = 600;  // Start window height (pixels)
const int startwinsizeX = 900;  // Start window width  (pixels)
 
// Objects
double savetime;               // Time of previous movement (sec)
double rainTime;               // Time used for rain animation 

// Scene
bool daytime;                  // True if day
bool stormMode;                // True if storming
bool lightning;                // True if storm has lightning
double cloudX;                 // Daytime's Cloud's X Location
double cloudY;                 // Daytime's Cloud's Y Location
double sunRotateAngle;         // The angle to rotate the sun by
double sunBeamRotateAngle;     // The angle to rotate the sun's beams by

// Size Modifiers
const double STORM_CLOUD_SIZE_MULTIPLIER = 3.0;
const double CLOUD_WIDTH = 4.0;

// drawSun
// Draws a filled octogon, using current GL states,
//  in the x,y plane, centered at the origin, aligned w/ x,y axes,
//  with sidelength of 2.
void drawSun()
{
	glColor3d(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2d(-1.0 , -2.0);
		glVertex2d(1.0 , -2.0);
		glVertex2d(2.0 , -1.0);
		glVertex2d(2.0 , 1.0);
		glVertex2d(1.0 , 2.0);
		glVertex2d(-1.0 , 2.0);
		glVertex2d(-2.0 , 1.0);
		glVertex2d(-2.0 , -1.0);
	glEnd();
}

// drawSunBeams
// Draws a collection of 8 lines, using current GL states,
//  in the x,y plane, focused at the origin, with distance
//  from the origin of 2.
void drawSunBeams()
{
	glColor3d(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex2d(2.0 , 2.0);
		glVertex2d(2.5 , 2.5);

		glVertex2d(2.0 , -2.0);
		glVertex2d(2.5 , -2.5);

		glVertex2d(-2.0 , -2.0);
		glVertex2d(-2.5 , -2.5);

		glVertex2d(-2.0 , 2.0);
		glVertex2d(-2.5 , 2.5);

		glVertex2d(2.5 , 0.0);
		glVertex2d(3.0 , 0.0);

		glVertex2d(0.0 , 2.5);
		glVertex2d(0.0 , 3.0);

		glVertex2d(-2.5 , 0.0);
		glVertex2d(-3.0 , 0.0);

		glVertex2d(0.0 , -2.5);
		glVertex2d(0.0 , -3.0);
	glEnd();
}

// drawStar
// Draws a collection of white points in the sky of the scene
void drawStar()
{
	glColor3d(1.0, 1.0, 1.0);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glVertex2d(4.0 , 12.0);
		glVertex2d(5.0 , 16.5);
		glVertex2d(8.0 , 15.0);
		glVertex2d(12.0 , 17.0);
		glVertex2d(15.0 , 14.0);
		glVertex2d(18.0 , 16.0);
		glVertex2d(21.0 , 12.0);
		glVertex2d(26.0 , 15.9);
	glEnd();
}

// drawMoon
// Draws a white filled polygon, in the sky of the scene
void drawMoon()
{		
	glBegin(GL_QUAD_STRIP);
		glVertex2d(22.0 , 15.0);
		glVertex2d(23.0 , 14.0);
		glVertex2d(23.0 , 16.0);
		glVertex2d(25.0 , 16.0);
		glVertex2d(23.0 , 17.0);
		glVertex2d(25.0 , 17.0);
		glVertex2d(22.0 , 18.0);
		glVertex2d(23.0 , 19.0);		
	glEnd();
}

// drawGrass
// Draws a green filled polygon to create the ground of the scene
void drawGrass()
{
	glColor3d(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2d(0.0 , 7.0);
		glVertex2d(5.0 , 9.0);
		glVertex2d(13.0 , 9.0);
		glVertex2d(20.0 , 7.0);
		glVertex2d(30.0 , 10.0);
		glVertex2d(30.0 , 0.0);
		glVertex2d(0.0 , 0.0);
	glEnd();
}

// drawMountain
// Draws a grey triangle-fan to create the mountain in the scene.
//   As well as lines on the mountain to give it some depth.
void drawMountain()
{
	//Mountain
	glColor3d(0.5, 0.5, 0.5);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2d(9.0 , 13.0);
		glVertex2d(5.0 , 9.0);
		glVertex2d(7.0 , 9.0);
		glVertex2d(9.0 , 9.0);
		glVertex2d(11.0 , 9.0);
		glVertex2d(13.0 , 9.0);
	glEnd();

	//Mountain Decoration
	glColor3d(0.4, 0.4, 0.4);
	glBegin(GL_LINES);
		glVertex2d(7.0 , 10.0);
		glVertex2d(9.0 , 9.5);
		glVertex2d(8.5 , 11.0);
		glVertex2d(10.0 , 10.5);
		glVertex2d(7.5 , 10.5);
		glVertex2d(11.0 , 9.9);
	glEnd();
}

// drawLake
// Draws a blue filled polygon to create the lake of the scene.
//   As well as waves on the surface of the water.
void drawLake()
{
	//Lake
	glColor3d(0.1, 0.1, 0.9);
	glBegin(GL_POLYGON);
		glVertex2d(5.0 , 6.0);
		glVertex2d(4.0 , 4.0);
		glVertex2d(7.0 , 3.0);
		glVertex2d(10.5 , 5.0);	
		glVertex2d(8.0 , 6.2);
	glEnd();

	//Waves on the lake
	glColor3d(0.4, 0.4, 0.9);
	glBegin(GL_LINE_LOOP);
		glVertex2d(5.0 , 4.0);
		glVertex2d(5.4 , 4.4);
		glVertex2d(5.8 , 4.0);
		glVertex2d(6.2 , 4.4);
		glVertex2d(6.6 , 4.0);
		glVertex2d(7.0 , 4.4);
		glVertex2d(7.4 , 4.0);
		glVertex2d(7.8 , 4.4);
		glVertex2d(8.2 , 4.0);
	glEnd();

	//Upper waves on the lake
	glBegin(GL_LINE_STRIP);
		glVertex2d(5.3 , 4.5);
		glVertex2d(5.7 , 4.9);
		glVertex2d(6.1 , 4.5);
		glVertex2d(6.5 , 4.9);
		glVertex2d(6.9 , 4.5);
		glVertex2d(7.3 , 4.9);
		glVertex2d(7.7 , 4.5);
		glVertex2d(8.1 , 4.9);
		glVertex2d(8.5 , 4.5);
	glEnd();
}

// drawHouse
// Draws a collection of filled polygons to create the house of the scene.
void drawHouse()
{
	//House
	glColor3d(0.6, 0.6, 0.1);
	glBegin(GL_POLYGON);
		glVertex2d(11.0 , 2.0);
		glVertex2d(16.0 , 2.0);
		glVertex2d(16.0 , 4.0);
		glVertex2d(11.0 , 4.0);		
	glEnd();

	//Door on the house
	glColor3d(0.2, 0.2, 0.1);
	glBegin(GL_POLYGON);
		glVertex2d(12.0 , 2.0);
		glVertex2d(13.5 , 2.0);
		glVertex2d(13.5 , 3.9);
		glVertex2d(12.0 , 3.9);		
	glEnd();

	//Roof on the House
	glColor3d(0.9, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2d(10.0 , 4.0);
		glVertex2d(11.5 , 6.0);
		glVertex2d(11.5 , 4.0);
		glVertex2d(15.5 , 6.0);	
		glVertex2d(15.5 , 4.0);
		glVertex2d(17.0 , 4.0);
	glEnd();
}

// drawTrees
// Draws the trees of the scene, including stumps.
void drawTrees()
{
	//Body of the trees
	glColor3d(0.1, 0.7, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2d(19.0 , 5.0);
		glVertex2d(20.0 , 6.0);
		glVertex2d(20.0 , 5.0);
		glVertex2d(28.0 , 6.0);
		glVertex2d(28.0 , 5.0);	
		glVertex2d(29.0 , 5.0);
	glEnd();

	//Tops of trees
	glBegin(GL_TRIANGLES);
		glVertex2d(20.0 , 6.0);
		glVertex2d(21.0 , 7.0);
		glVertex2d(22.0 , 6.0);

		glVertex2d(21.0 , 6.0);
		glVertex2d(22.0 , 7.0);
		glVertex2d(23.0 , 6.0);
		
		glVertex2d(22.0 , 6.0);
		glVertex2d(23.0 , 7.0);
		glVertex2d(24.0 , 6.0);
		
		glVertex2d(23.0 , 6.0);
		glVertex2d(24.0 , 7.0);
		glVertex2d(25.0 , 6.0);
		
		glVertex2d(24.0 , 6.0);
		glVertex2d(25.0 , 7.0);
		glVertex2d(26.0 , 6.0);
				
		glVertex2d(25.0 , 6.0);
		glVertex2d(26.0 , 7.0);
		glVertex2d(27.0 , 6.0);
		
		glVertex2d(26.0 , 6.0);
		glVertex2d(27.0 , 7.0);
		glVertex2d(28.0 , 6.0);
	glEnd();

	//Tree Stumps
	glColor3d(0.6, 0.6, 0.1);
	glBegin(GL_LINES);
		glVertex2d(21.0 , 5.0);
		glVertex2d(21.0 , 4.5);

		glVertex2d(22.0 , 5.0);
		glVertex2d(22.0 , 4.5);

		glVertex2d(23.0 , 5.0);
		glVertex2d(23.0 , 4.5);

		glVertex2d(24.0 , 5.0);
		glVertex2d(24.0 , 4.5);

		glVertex2d(25.0 , 5.0);
		glVertex2d(25.0 , 4.5);

		glVertex2d(26.0 , 5.0);
		glVertex2d(26.0 , 4.5);

		glVertex2d(27.0 , 5.0);
		glVertex2d(27.0 , 4.5);
	glEnd();
}

// drawCloud
// Draws a filled polygon, using current GL states,
//  in the x,y plane, starting at the origin.
void drawCloud()
{
	glBegin(GL_POLYGON);
		glVertex2d(0.0 , 0.0);
		glVertex2d(0.5 , -0.25);
		glVertex2d(1.0 , 0.0);
		glVertex2d(1.5 , -0.25);
		glVertex2d(2.0 , 0.0);
		glVertex2d(2.5 , -0.25);
		glVertex2d(3.0 , 0.0);
		glVertex2d(3.5 , -0.25);
		glVertex2d(4.0 , 0.0);
		glVertex2d(3.5 , 0.3);
		glVertex2d(3.0 , 0.4);
		glVertex2d(2.5 , 0.8);
		glVertex2d(2.0 , 0.6);
		glVertex2d(1.5 , 0.7);
		glVertex2d(1.0 , 0.4);
	glEnd();
}

// drawRain
// Draws a collection of vertical lines, using current GL states,
//  in the x,y plane, starting at the origin, lines at each x value,
//  length of 0.5
void drawRain()
{
	glColor3d(0.4, 0.4, 0.9);
	glBegin(GL_LINES);
		glVertex2d(0.0 , 0.0);
		glVertex2d(0.0 , 0.5);

		glVertex2d(1.0 , 0.0);
		glVertex2d(1.0 , 0.5);

		glVertex2d(2.0 , 0.0);
		glVertex2d(2.0 , 0.5);

		glVertex2d(3.0 , 0.0);
		glVertex2d(3.0 , 0.5);

		glVertex2d(4.0 , 0.0);
		glVertex2d(4.0 , 0.5);

		glVertex2d(5.0 , 0.0);
		glVertex2d(5.0 , 0.5);

		glVertex2d(6.0 , 0.0);
		glVertex2d(6.0 , 0.5);

		glVertex2d(7.0 , 0.0);
		glVertex2d(7.0 , 0.5);

		glVertex2d(8.0 , 0.0);
		glVertex2d(8.0 , 0.5);

		glVertex2d(9.0 , 0.0);
		glVertex2d(9.0 , 0.5);

		glVertex2d(10.0 , 0.0);
		glVertex2d(10.0 , 0.5);
	glEnd();
}

// drawLightning
// Draws a filled octogon, using current GL states,
//  in the x,y plane, starting at the origin
void drawLightning()
{
	glBegin(GL_POLYGON);
		glVertex2d(0.0 , 0.0);
		glVertex2d(-1.1, -2.0);
		glVertex2d(0.4 , -1.8);
		glVertex2d(0.5 , -4.0);
		glVertex2d(1.1 , -1.7);
		glVertex2d(0.6 , -1.7);
		glVertex2d(1.0 , 0.0);
	glEnd();
}

// myDisplay
// The GLUT display function
void myDisplay()
{
	//Initial Loading
	glLoadIdentity();

	//Setting Line Width to be more visible
	glLineWidth(5.0f);

	if(daytime)
	{
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw the Sun
		glPushMatrix();
		glTranslated(23.0, 16.5, 0.0);
		glRotated(sunRotateAngle,0.0,0.0,1.0);
		drawSun();
		glPopMatrix();

		//Draw the Sun Beams
		glPushMatrix();
		glTranslated(23.0, 16.5, 0.0);
		glRotated(sunBeamRotateAngle, 0.0, 0.0, 1.0);
		drawSunBeams();
		glPopMatrix();
	}
	else
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawStar();
		drawMoon();	
	}

	//Draw the scene
	drawGrass();
	drawMountain();
	drawLake();
	drawHouse();
	drawTrees();
	
	//Draw the Cloud, not-storming
	if(!stormMode)
	{
		glColor3d(1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslated(cloudX, cloudY, 0.0);
		drawCloud();
		glPopMatrix();
	} else	
	{
		//Draw the Cloud, storming
		glColor3d(0.3, 0.3, 0.3);
		glPushMatrix();
		glTranslated(cloudX, cloudY, 0.0);
		glScaled(STORM_CLOUD_SIZE_MULTIPLIER, STORM_CLOUD_SIZE_MULTIPLIER, 1.0);
		drawCloud();
		glPopMatrix();

		//If storming, and lightning toggled, draw lightning
		if(lightning)
		{
			glColor3d(1.0, 1.0, 0.0);
			glPushMatrix();
			glTranslated(cloudX + 6.0, cloudY, 0.0);
			drawLightning();
			glPopMatrix();
		}

		//Draw and animate Rain
		if(rainTime > 0.0)
		{
			glPushMatrix();
			glTranslated(cloudX + 1.0, cloudY - 1.5, 1.0);
			drawRain();
			glPopMatrix();

			glPushMatrix();
			glTranslated(cloudX + 1.0, cloudY - 3.5, 1.0);
			drawRain();	
			glPopMatrix();
			if(rainTime >= 1.0)
				rainTime -= 2.0;
		} 
		else 
		{
			glPushMatrix();
			glTranslated(cloudX + 1.5, cloudY - 2.5, 1.0);
			drawRain();	
			glPopMatrix();
		}
	}

    // Draw documentation
    glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
    glColor3d(1.0, 1.0, 1.0);        // Black text
    BitmapPrinter p(-0.9, 0.9, 0.1);
    p.print("Space:  Toggle daytime");
    p.print("Arrows: Fly the cloud");
	p.print("S key:  STORM MODE!");
	p.print("D key:  LIGHTNING BOLT! (while storming)");
    p.print("Esc:    Quit");
    glPopMatrix();                // Restore prev projection
    glMatrixMode(GL_MODELVIEW);

    glFlush();
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

	// Update the rain's time for animating
	rainTime += elapsedtime;
	if(rainTime > 1.0)
		rainTime = 1.0;

	// Rotate the sun
	sunRotateAngle += elapsedtime * 10;
	sunBeamRotateAngle -= elapsedtime * 10;
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
    case ' ':     // Space: toggle daytime
        daytime = !daytime;
        glutPostRedisplay();
        break;
	case 's':     // s: toggle Storm Mode
		stormMode = !stormMode;
		glutPostRedisplay();
		break;
	case 'd':     // d: toggles lightning bolt, only during storm mode
		if(stormMode)
			lightning = !lightning;
		glutPostRedisplay();
		break;
    }
}


// mySpecial
// The GLUT special function
void mySpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_RIGHT:  // -> move Cloud right
        cloudX += 0.1;
		if (cloudX > 30.)
		{
			if(stormMode)
			    cloudX = -CLOUD_WIDTH * STORM_CLOUD_SIZE_MULTIPLIER;
			else
				cloudX = -CLOUD_WIDTH;
		}
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:   // <- move Cloud left
        cloudX -= 0.1;
		if(stormMode)
		{
			if (cloudX < -(CLOUD_WIDTH * STORM_CLOUD_SIZE_MULTIPLIER))
				cloudX = 30.0;
		} else if(cloudX < -CLOUD_WIDTH)
			cloudX = 30.0;
        glutPostRedisplay();
        break; 

	case GLUT_KEY_UP:    // ^ move Cloud up
        cloudY += 0.1;
        if (cloudY > 20.0)
            cloudY = 20.;
        glutPostRedisplay();
        break; 

	case GLUT_KEY_DOWN:  // \/ move Cloud down
        cloudY -= 0.1;
        if (cloudY < 0.0)
            cloudY = 0.0;
        glutPostRedisplay();
        break; 
    }
}


// init
// Initialize GL states & global data
// Called by main after window creation
void init()
{
    // Scene
    daytime = true;
	stormMode = false;
	lightning = false;
    cloudX = 5.0;
	cloudY = 14.0;
	rainTime = 0.0;
	sunRotateAngle = 0.0;
	sunBeamRotateAngle = 0.0;

	// Objects
    savetime = glutGet(GLUT_ELAPSED_TIME) / 1000.;

    // OpenGL Stuff
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 30.0, 0.0, 20.0);

    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
}


int main(int argc, char ** argv)
{
    // Initialize OpenGL/GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Make a window
    glutInitWindowSize(startwinsizeX, startwinsizeY);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS 381 - Homework 1, Cloud Sim 2014");

    // Initialize GL states & register GLUT callbacks
    init();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);

    // Do something
    glutMainLoop();

    return 0;
}
