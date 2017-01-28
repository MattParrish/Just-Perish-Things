///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
//    boss.cp file for MissileDefender   //
///////////////////////////////////////////

#include "boss.h"

//Pre: None
//Post: Creates a Boss object.
Boss::Boss()
{
	_yLocation = -250;
	_bossHP = 300;
}

//Pre: None
//Post: Initilizes the image for the boss.
void Boss::setUp()
{
	_boss = gl::Texture( loadImage( loadAsset("doge.jpg" ) ) );
}

//Pre: None
//Post: Updates the Boss's location.
void Boss::update() { 
	int elapsedFrames=app::getElapsedFrames() % 3; 

	int newyLocation=_yLocation; 

	_asm{ 
		mov eax,elapsedFrames; 
		mov edi, newyLocation;
		cmp eax, 0;
		jne done; 
		inc edi;
	done:
		mov newyLocation,edi; 
	} 
	_yLocation=newyLocation;
}

//Pre: None
//Post: Checks if the boss is hit, if so, decrements its health.
void Boss::hitCheck(Vec2f explosion)
{
	if(explosion.x > app::getWindowWidth()/2 - 125 && explosion.x < app::getWindowWidth()/2 + 125)
		if(explosion.y > _yLocation  && explosion.y < _yLocation + 250)
			_bossHP--;
}

//Pre: None
//Post: Draws the boss on the screen.
void Boss::draw()
{
	gl::draw(_boss, Vec2f(app::getWindowWidth()/2 - 128, _yLocation) );
}

