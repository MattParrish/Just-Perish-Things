///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
//   city.cpp file for MissileDefender   //
///////////////////////////////////////////

#include "city.h"

using namespace ci;
using namespace ci::app;

//Pre: None
//Post: Creates a City object.
City::City()
{}

//Pre: None
//Post: Draws the city on the screen.
void City::draw()
{
	gl::draw(_currentCityState, _cityLocation);
}

//Pre: placement is an int. 
//Post: Positions the cities according to the passed in value.
void City::locationSet(int placement)
{
	float halfCityWidth=63.0;
	float halfCityHeight=93.0;

	float cityPlacementWidthHelper=app::getWindowWidth()/4;
	float cityPlacementHeightHelper=app::getWindowHeight();

	float actualCitySpotWidth;
	float actualCitySpotHeight;
	switch(placement)
	{		
	case 1:	
		 _asm{
			 movss xmm0,cityPlacementWidthHelper;
			 movss xmm1,cityPlacementHeightHelper;

			 subss xmm0, halfCityWidth;
			 subss xmm1, halfCityHeight;

			 movss actualCitySpotWidth, xmm0;
			 movss actualCitySpotHeight, xmm1;

		 }
		 _cityLocation.x=actualCitySpotWidth;
		 _cityLocation.y=actualCitySpotHeight;
		break;
	case 2:
		cityPlacementWidthHelper=app::getWindowWidth()/2;

		_asm{
			 movss xmm0,cityPlacementWidthHelper;
			 movss xmm1,cityPlacementHeightHelper;

			 subss xmm0, halfCityWidth;
			 subss xmm1, halfCityHeight;

			 movss actualCitySpotWidth, xmm0;
			 movss actualCitySpotHeight, xmm1;

		 }
		 _cityLocation.x=actualCitySpotWidth;
		 _cityLocation.y=actualCitySpotHeight;
		break;
	case 3:
		cityPlacementWidthHelper*=3;

		_asm{
			 movss xmm0,cityPlacementWidthHelper;
			 movss xmm1,cityPlacementHeightHelper;

			 subss xmm0, halfCityWidth;
			 subss xmm1, halfCityHeight;

			 movss actualCitySpotWidth, xmm0;
			 movss actualCitySpotHeight, xmm1;

		 }
		 _cityLocation.x=actualCitySpotWidth;
		 _cityLocation.y=actualCitySpotHeight;
		break;
	default:
		break;
	}
}

//Pre: None
//Post: Sets _alive to true, and loads an image into _currentCityState
void City::setUp()
{
	_currentCityState = gl::Texture( loadImage( loadAsset("fine.jpg" ) ) );
	_alive=true;
}

//Pre: None
//Post: Switches the _alive state to false, and updates the picture of the city to be on fire.
void City::kill()
{
	_currentCityState =  gl::Texture( loadImage( loadAsset("burn.jpg" ) ) );
	_alive=false;
}