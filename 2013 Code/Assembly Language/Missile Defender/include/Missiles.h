///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
//  missiles.h file for MissileDefender  //
///////////////////////////////////////////

#pragma once
#include "cinder/Channel.h"

// class Missile
// Missile object
class Missile {
 public:

//constructor
	Missile( ci::Vec2f );

//Mutators
	void update();
	void draw();

//Accessor
	int getMissileX();

	ci::Vec2f	_missileLocation;
	ci::Vec2f	_missileDirection;
	float		_missileVelocity;	
	float		_missileRadius;
};