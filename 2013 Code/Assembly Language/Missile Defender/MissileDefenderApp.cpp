///////////////////////////////////////////
//    Matthew Parrish and Tom Cerny      //
//          CS301 Project 2              //
// Main control file for MissileDefender //
///////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "MissileLauncher.h"
#include "Missiles.h"
#include "Resources.h"
#include "city.h"
#include "boss.h"

using namespace ci;
using namespace ci::app;

class MissileDefenderApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void update();
	void draw();
	void launchMissile();
	void prepareSettings( Settings *settings );
	void updateCityLocation();
	void cityDraw();

	MissileLauncher FIRETHECANNONZ;

	City _left;
	City _middle;
	City _right;

	Boss _boss;

	
	gl::Texture _winScreen;
	gl::Texture _loozeScreen;

	float _mouseX; 
	float _mouseY; 
	bool _mIsPressed;
	float _radius;
};


//Pre: Settings be Settingtastic.
//Post: Sets the window size to 800x600
void MissileDefenderApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 600 );
}

//Pre: None
//Post: Launches missiles. Launches more as time goes on.
void MissileDefenderApp::launchMissile()
{
	if(getElapsedFrames()<(60*10)) //FRAME RATE * TIME IN SECONDS
	{
		if(getElapsedFrames()%60 == 0)
			FIRETHECANNONZ.addMissile();
	}
	else if(getElapsedFrames()<(60*20))
	{
		if(getElapsedFrames()%60 == 0)
		{
			FIRETHECANNONZ.addMissile();
			FIRETHECANNONZ.addMissile();
		}
	}
	else if(getElapsedFrames()<(60*33))
	{
		if(getElapsedFrames()%60 == 0)
		{
			FIRETHECANNONZ.addMissile();
			FIRETHECANNONZ.addMissile();
			FIRETHECANNONZ.addMissile();
			FIRETHECANNONZ.addMissile();
		}
	}
}

//Pre: None
//Post: Updates the city's location to account for resizing the screen.
void MissileDefenderApp::updateCityLocation()
{
	_left.locationSet(1);
	_middle.locationSet(2);
	_right.locationSet(3);
}

//Pre: None
//Post: Draws the city.
void MissileDefenderApp::cityDraw()
{
	_left.draw();
	_middle.draw();
	_right.draw();
}

//Pre: None
//Post: Initalizes the cities' locations, sets up all other objects in the DefenderApp, and loads _winScreen and 
//_loozeScreen's images.
void MissileDefenderApp::setup()
{
	updateCityLocation();
	_boss.setUp();
	_left.setUp();
	_middle.setUp();
	_right.setUp();
	_radius=0.0f;	
	_winScreen = gl::Texture( loadImage( loadAsset("dogeWin.jpg" ) ) );
	_loozeScreen = gl::Texture( loadImage( loadAsset("dogeLose.jpg" ) ) );
}

//Pre: event be a Mousetastic event.
//Post: sets _mouseX and _mouseY to the location of the click, as well as sets _mIsPressed to true.
void MissileDefenderApp::mouseDown( MouseEvent event )
{
	_mIsPressed=true; 
	_mouseX=event.getX(); 
	_mouseY=event.getY(); 
} 


//Pre: event be a Mousetastic event.
//Post: Resets _mIsPressed to false, and sets _radius to 0
void MissileDefenderApp::mouseUp( MouseEvent event ) 
{ 
	_mIsPressed = false; 
	_radius =0.0f;
}

//Pre: None
//Post: Launches new missiles, and updates all missiles on screen.
void MissileDefenderApp::update()
{
	launchMissile();
	FIRETHECANNONZ.update();
}

//Pre: None
//Post: Draws everything onto the screen, and controls the mouse click-logic. 
void MissileDefenderApp::draw()
{
	// clear out the window with black
	glDisable( GL_TEXTURE_2D );
	glColor3f( 1, 1, 1 );
	gl::clear( Color( 0, 0, 0 ) , true ); 

	updateCityLocation();
	cityDraw();

	if(!FIRETHECANNONZ.midLifeCheck())
		_middle.kill();

	if(!FIRETHECANNONZ.leftLifeCheck())
		_left.kill();

	if(!FIRETHECANNONZ.rightLifeCheck())
		_right.kill();

	if(app::getElapsedFrames() > (60*40) && _boss._bossHP >= 0)
	{
		_boss.draw();
		_boss.update();
		if(_boss._yLocation == app::getWindowHeight() - 250) // 250 == height of boss picture
		{
			_middle.kill();
			_left.kill();
			_right.kill();
		}
	}

	FIRETHECANNONZ.draw();

		if( _mIsPressed ){ 
			float r=_radius; 
			float increment=3.0f; 
			float maxSize=75.0f; 
			_asm{ 
				movss xmm0, r; 
				ucomiss xmm0,maxSize; 
				je _maxSize; 
				addss xmm0,increment;
					
			_maxSize: 
				movss r,xmm0; 
			}; 
			_radius=r;
			gl::color(Color(1.0,0.2,0.0)); 
			gl::drawSolidCircle( Vec2f( _mouseX, _mouseY ), _radius ); 
		
			glColor3f( 1, 1, 1 ); 

		_boss.hitCheck(Vec2f( _mouseX, _mouseY ));
		FIRETHECANNONZ.deadCheck(_mouseX, _mouseY, _radius);
		}

		if(_left._alive == false && _right._alive == false && _middle._alive == false)
			gl::draw(_loozeScreen, app::getWindowBounds() );

		if(_boss._bossHP <= 0)
			gl::draw(_winScreen, app::getWindowBounds() );


}

CINDER_APP_NATIVE( MissileDefenderApp, RendererGl )
