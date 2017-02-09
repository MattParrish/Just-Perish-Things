///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
//MissileLauncher.cpp file for MissileDefender 
///////////////////////////////////////////

#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "MissileLauncher.h"

using namespace ci;
using std::list;

//MissileLauncher's Constructor
//Pre: None
//Post: Creates a MissileLauncher object.
MissileLauncher::MissileLauncher()
{ 
}

//Pre: None
//Post: Checks if the leftmost city is alive.
bool MissileLauncher::leftLifeCheck()
{
	for( list<Missile>::iterator missileList = _Missiles.begin(); missileList != _Missiles.end(); ++missileList )
	{
		if(missileList->getMissileX() == app::getWindowWidth()/4 
		   && missileList->_missileLocation.y > app::getWindowHeight() - 10 
		   && missileList->_missileLocation.y < app::getWindowHeight() + 5)
			return false;
	}
	return true;
}


//Pre: None
//Post: Checks if the middle city is alive.
bool MissileLauncher::midLifeCheck()
{
	for( list<Missile>::iterator missileList = _Missiles.begin(); missileList != _Missiles.end(); ++missileList )
	{
		if(missileList->getMissileX() == app::getWindowWidth()/2 
		   && missileList->_missileLocation.y > app::getWindowHeight() - 10 
		   && missileList->_missileLocation.y < app::getWindowHeight() + 5)
			return false;
	}
	return true;
}

//Pre: None
//Post: Checks if the rightmost city is alive.
bool MissileLauncher::rightLifeCheck()
{
	for( list<Missile>::iterator missileList = _Missiles.begin(); missileList != _Missiles.end(); ++missileList )
	{
		if(missileList->getMissileX() == 3*(app::getWindowWidth()/4) 
		   && missileList->_missileLocation.y > app::getWindowHeight() - 5
		   && missileList->_missileLocation.y < app::getWindowHeight() + 5)
			return false;
	}
	return true;
}

//Pre: None
//Post: Updates the MissileLauncher's Missiles' locations
void MissileLauncher::update()
{
	for( list<Missile>::iterator missileList = _Missiles.begin(); missileList != _Missiles.end(); ++missileList )
	{
		missileList->update();
	}
}

//Pre: None
//Post: Draws all Missiles in the MissileLauncher list.
void MissileLauncher::draw()
{
	for( list<Missile>::iterator missileList = _Missiles.begin(); missileList != _Missiles.end(); ++missileList )
	{
		missileList->draw();
	}

}

//Pre: None
//Post: Launches a new missile.
void MissileLauncher::addMissile()
{
	float x = Rand::randFloat( app::getWindowWidth() );
	float y = 0;
	_Missiles.push_back( Missile( Vec2f( x, y ) ) );

}

//Pre: None
//Post: Checks if a missile is caught in the click-explosion, and if so, erases. 
void MissileLauncher::deadCheck(float circleX, float circleY, float radius)
{
	for( list<Missile>::iterator missileList = _Missiles.begin(); missileList != _Missiles.end();/* missileList++*/ )
	{
		if(missileList->_missileLocation.x < (circleX + radius) 
		   && missileList->_missileLocation.x > (circleX - radius) 
		   && missileList->_missileLocation.y < (circleY + radius) 
		   && missileList->_missileLocation.y > (circleY - radius))
			missileList = _Missiles.erase(missileList);
		else
			missileList++;
	}
}

