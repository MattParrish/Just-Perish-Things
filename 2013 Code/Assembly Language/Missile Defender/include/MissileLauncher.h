///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
//MissileLauncher.h file for MissileDefender
///////////////////////////////////////////

#pragma once
#include "Missiles.h"

// class MissileLauncher
// Contains a list of Missile objects
class MissileLauncher {
 	public:

	//Constructor
	MissileLauncher();

	//Mutators
	void update();
	void draw();
	void addMissile();
	bool leftLifeCheck();
	bool midLifeCheck();
	bool rightLifeCheck();
	void deadCheck(float, float, float);
	
	std::list<Missile> _Missiles;
};