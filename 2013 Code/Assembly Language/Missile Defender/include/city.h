///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
//   city.h file for MissileDefender     //
///////////////////////////////////////////

#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;

// class City
// Contains information for city Objects
class City {
 public:

//Constructor
	City();

//Mutators
	void kill();
	void draw();
	void setUp();
	void locationSet(int);
	bool _alive;

	gl::Texture _currentCityState;
	ci::Vec2f	_cityLocation;
};