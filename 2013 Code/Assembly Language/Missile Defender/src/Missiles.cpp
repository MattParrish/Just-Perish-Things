///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
// missiles.cpp file for MissileDefender //
///////////////////////////////////////////

#include "Missiles.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include <random>

using namespace ci;

//Missile's Constructor
//Pre: None
//Post: Creates a missile at the location specified by loc, aimed at random city.
Missile::Missile( Vec2f loc )
{
	int i = rand() % 3;
	int cityDirection;
	_missileLocation = loc;
	switch(i)
	{
	case 0:
		cityDirection = app::getWindowWidth()/4;
		break;
	case 1:
		cityDirection = app::getWindowWidth()/2;
		break;
	case 2: 
		cityDirection = (3*app::getWindowWidth())/4;
		break;
	default:
		cityDirection = app::getWindowWidth()/4;
		break;
	}

	float movX = ((cityDirection - _missileLocation.x)/100.0f);
	float movY = (app::getWindowHeight()/100.0f);
	_missileDirection = Vec2f(movX, movY);
	_missileVelocity = Rand::randFloat( 0.4f ) + 0.2f;
	_missileRadius = 3.0f;
}

//Pre: None
//Post: Returns the X location of a missile.
int Missile::getMissileX()
{
	return _missileLocation.x;
}

//Pre: None
//Post: Moves the missile.
void Missile::update() { 
	float missileDX=_missileDirection.x; 
	float missileDY=_missileDirection.y; 
	float missileLX=_missileLocation.x; 
	float missileLY=_missileLocation.y; 
	float missileV=_missileVelocity;

	_asm{ 
		movss xmm0, missileDX;
		movss xmm1, missileDY;
		mulss xmm0,missileV; 
		mulss xmm1,missileV;
		addss xmm0, missileLX; 
		addss xmm1, missileLY; 
		movss missileLX,xmm0;
		movss missileLY,xmm1;
	};

	_missileLocation.x=missileLX; 
	_missileLocation.y=missileLY; 
}

//Pre: None
//Post: Draws the missile.
void Missile::draw()
{
	gl::drawSolidCircle( _missileLocation, _missileRadius );
}

