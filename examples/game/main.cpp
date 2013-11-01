/*! \mainpage Game Example
	
	Describes a simple shooting game example,
	demonstrating engine basics.
 */
/*! \file main.cpp
	\author HiddenSeeker

	Describes a main file for game
 */
#include "game.h"

#include <renderer.h>
#include <input.h>
#include <fontmanager.h>
#include <texturemappedfont.h>
#include <texturemanager.h>

#include <math.h>
#include <time.h>

/*! Loads a texture from file and registers it in Texture Manager
	\param[in] filename name of loaded file 
	\param[in] texturename name of registered texture
	\return whether loading of texture was successfull
 */
bool load_texture(const char * filename,const char * texturename)
{
	sad::Texture * texture=new sad::Texture();
	bool result=texture->load(sad::String(filename));	
	if (result) { 
		// Make some textures to square power of two, making them compatible to old videocards
		// which does not support GL_ARB_texture_non_power_of_two or 
		// GL_ARB_texture_rectangle extensions
		texture->makePOT();
		sad::TextureManager::ref()->add(texturename, texture);
	} else	{
		SL_FATAL( fmt::Format("Loading \"{0}\" failed") << filename);
		delete texture;
	}
	return result;
}
/*! Loads a texture from file and registers it in Texture Manager.
	
	This is a special case, when texture has a white background, which must be converted 
	to tansparent before work.
	\param[in] filename name of loaded file 
	\param[in] texturename name of registered texture
	\return whether loading of texture was successfull
 */
bool load_texture_with_alphachannel(const char * filename,const char * texturename)
{
	bool result = load_texture(filename, texturename);
	if (result)
	{
		// With first component we set alpha channel to fully transparent (255) of white color
		// (255,255,255)
		sad::TextureManager::ref()->get(texturename)->setAlpha(255,sad::Color(255,255,255));
	}
	return result;
} 
/*! Returns a font from file and registers it in Font Manager 
 */
bool load_font(const sad::String & fontfolder, const sad::String & fontname)
{
	sad::String fontextendedname = fontfolder + fontname;
	sad::TextureMappedFont * fnt =  new sad::TextureMappedFont();
	bool result = fnt->load(fontextendedname);
	if (result) { 
		fnt->setSize(30);
		sad::FontManager::ref()->add(fnt, fontname);
	} else	{
		SL_FATAL( fmt::Format("Loading \"{0}\" failed") << fontname);
		delete fnt;
	}
	return result;
}

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

	// Test console log images
	SL_FATAL   ("This is sad::Game");
	SL_CRITICAL("It\'s a game about a very sad");
	SL_WARNING ("smiley face");
	SL_MESSAGE ("which wants to survive");
	SL_DEBUG   ("against other smiley faces");
	SL_USER    ("it\'s sure will be hard for him...", "END");

	// Inits a renderer as non-fullscreen 640x480 window
	// And set PicoPNG loader for images
	sad::Renderer::ref()->init(sad::Settings(640,480,false));
	SL_MESSAGE("Renderer successfully initialized!");	
	// Inits generator for spawns and random raings
	srand((unsigned int)time(NULL));

	//Loading resources
	bool res=true; 
	sad::String fontfolder = "examples/game/";
	res=res && load_font(fontfolder, "font"); 
	res=res && load_texture("examples/game/title.tga","title");
	res=res && load_texture("examples/game/ingame.tga","background");
    res=res && load_texture_with_alphachannel("examples/game/objects.bmp","objects"); 
    if (!res)
	{
		SL_FATAL("Resource loading failed!");
		return 1;
	}
	SL_MESSAGE("Resources successfully loaded");
	
	// Create and run game	
	Game * game = new Game();
	game->run();
	delete game;


	// If we are here, this means, that window is closed and we can exit the main loop
	return 0;
}

