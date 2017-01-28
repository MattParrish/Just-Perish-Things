///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
//   boss.h file for MissileDefender     //
///////////////////////////////////////////

#pragma once
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Vector.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include <vector>


using namespace ci;
using namespace ci::app;
using namespace std;


//class Boss
//Contains information on a Boss Object
class Boss {
 public:

//Constructor
	Boss();

//Mutators
	void setUp();
	void update();
	void draw();
	void hitCheck(Vec2f);
	int _bossHP;

	gl::Texture _boss;
	int _yLocation;
};