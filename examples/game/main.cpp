/*! \mainpage Game Example
	
	Describes a simple shooting game example,
	demonstrating engine basics.
 */
/*! \file main.cpp
	

	Describes a main file for game
 */
#include "game.h"

#include <renderer.h>
#include <texturemappedfont.h>
#include <mousecursor.h>
#include <sprite2d.h>
#include <objectdependentfpsinterpolation.h>

#include <math.h>
#include <time.h>

/*! A main function 
	\param[in] argc count of arguments
	\param[in] argv arguments
 */
int main(int argc, char** argv)
{
	// Init logs with target streams to file  and console
	sad::log::FileTarget * filetarget = new sad::log::FileTarget();
	filetarget->open("log.txt");

	// A format for console is described as {date}: [file and line] [subsystem] message
	sad::log::ConsoleTarget * consoletarget = new sad::log::ConsoleTarget(
		"{0}: [{1}] {3}{2}{4}", 0, true
	);
	sad::log::Log::ref()->addTarget(filetarget).addTarget(consoletarget);
	
	// Output executable path
	SL_DEBUG   ("Executable path is ");
	SL_DEBUG   (sad::Renderer::ref()->executablePath());

	// Test console log images
	SL_FATAL   ("This is sad::Game");
	SL_CRITICAL("It\'s a game about a very sad");
	SL_WARNING ("smiley face");
	SL_MESSAGE ("which wants to survive");
	SL_DEBUG   ("against other smiley faces");
	SL_USER    ("it\'s sure will be hard for him...", "END");

	// Inits a renderer as non-fullscreen 640x480 window

	sad::ObjectDependentFPSInterpolation * fps = new sad::ObjectDependentFPSInterpolation();
	fps->setRenderer(sad::Renderer::ref());
	sad::Renderer::ref()->setFPSInterpolation(fps);

	sad::Renderer::ref()->init(sad::Settings(640,480,false));

	SL_MESSAGE("Renderer successfully initialized!");	
	// Inits generator for spawns and random raings
	srand((unsigned int)time(NULL));

	//Loading resources
	bool res=true; 
	sad::Vector<sad::resource::Error *> errors = sad::Renderer::ref()->loadResources("examples/game/resources.json");
	sad::String errortext;
	if (errors.size() != 0)
	{
		res = false;
		SL_FATAL(sad::resource::format(errors));
	} 
	sad::util::free(errors);

	if (!res)
	{
		return 1;
	}
	SL_MESSAGE("Resources successfully loaded");
	
	// Set cursor
	sad::Sprite2D * a = new sad::Sprite2D(
		sad::Renderer::ref()->texture("objects"),
		sad::Rect2D(sad::Point2D(441,32),sad::Point2D(457,48)),
		sad::Rect2D(sad::Point2D(-8, -8), sad::Point2D(8, 8))
	);
	sad::Renderer::ref()->cursor()->setImage(a);

	// Create and run game	
	Game * game = new Game();
	game->run();
	delete game;


	// If we are here, this means, that window is closed and we can exit the main loop
	return 0;
}

