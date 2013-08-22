/*! \mainpage Game Example
	
	Describes a simple shooting game example,
	demonstrating engine basics.
 */
/*! \file main.cpp
	\author HiddenSeeker

	Describes a main file for game
 */
#include <renderer.h>
#include <input.h>
#include <fontmanager.h>
#include <png/picopngloader.h>
#include "game.h"
#include "player.h"
#include "movitem.h"

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
	bool result=texture->load(hst::string(filename));	
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
		sad::TextureManager::ref()->get(texturename)->setAlpha(255,hst::color(255,255,255));
	}
	return result;
} 
/*! Returns a font from file and registers it in Font Manager 
 */
bool load_font(const hst::string & fontfolder, const hst::string & fontname)
{
	hst::string png = fontfolder + fontname + ".PNG";
	hst::string cfg = fontfolder + fontname + ".CFG";
	sad::TMFont * fnt=new sad::TMFont;
	bool result = fnt->load(png, cfg, hst::color(255,255,255),true);
	if (result) { 
		sad::FontManager::ref()->add(fnt, fontname);
	} else	{
		SL_FATAL( fmt::Format("Loading \"{0}\" failed") << fontname);
		delete fnt;
	}
	return result;
}


/*! A game object for interacting with enemies
 */
Game * PlayingGame;

/*! A callback, called when bullet, owned by player, hits an enemy
	An enemy dies and score rises by 100
	\param[in] bullet object 1
	\param[in] enemy  object 2
 */
void playerbullet_collided_with_enemy(Collidable * bullet,Collidable * enemy)
{
	PlayingGame->increasePlayerScore(100);
	enemy->die();
}
/*! A callback, called when player's bullet hits an enemy, which throws other enemies
	An enemy decreases life by one and bullet is removed from scene
	\param[in] playerbullet object 1
	\param[in] enemy  object 2
 */
void playerbullet_collided_with_supershootingenemy(Collidable * playerbullet, Collidable * enemy)
{
	SuperShootingEnemy * tenemy = (SuperShootingEnemy *) enemy;
	tenemy->hit();
	playerbullet->die();
}
/*! A callback, called when player hits a bonus
	A score rises by 50, player's health rises by 1 and bonus removed from scene
	\param[in] player object 1
	\param[in] bonus  object 2
 */
void player_collided_with_bonus(Collidable * player, Collidable * bonus)
{
	PlayingGame->increasePlayerScore(50);
	PlayingGame->increasePlayerHealth(1);
	bonus->die();
}
/*! A callback, called when player hits a bullet
	A life decreases, and if becomes less than zero, player dies and start screen is toggled
	\param[in] player object 1
	\param[in] enemyorbullet  object 2
 */
void player_collided_with_enemy_or_enemybullet(Collidable * player, Collidable * enemyorbullet)
{
 if (enemyorbullet->type() == SuperShootingEnemy::ID)
 {
	SL_DEBUG("Dying today");
 }
  enemyorbullet->die();
  PlayingGame->decreasePlayerHealth(1);
}


/*! A handler which takes two collidable objects
 */ 
typedef CMHandler<Collidable,Collidable> CCHandler;


int main(int argc, char** argv)
{
	// Init logs with target streams to file  and console
	sad::log::FileTarget * filetarget = new sad::log::FileTarget();
	filetarget->open("log.txt");

	// A format for console is described as {date}: [file and line] [subsystem] message
	sad::log::ConsoleTarget * consoletarget = new sad::log::ConsoleTarget(
		"{0}: [{1}] {3}{2}{4}", 6, true
	);
	sad::Log::ref()->addTarget(filetarget).addTarget(consoletarget);

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
	sad::Renderer::ref()->textures()->setLoader("PNG", new PicoPNGTextureLoader());
	SL_MESSAGE("Renderer successfully initialized!");	
	// Inits generator for EnemyEmitter
	srand((unsigned int)time(NULL));

	//Loading resources
	bool res=true; 
	hst::string fontfolder = "examples/game/";
	res=res && load_font(fontfolder, "times_lg"); 
	res=res && load_texture("examples/game/title.tga","title");
	res=res && load_texture("examples/game/ingame.tga","background");
    res=res && load_texture_with_alphachannel("examples/game/objects.bmp","objects"); 
    if (!res)
	{
		SL_FATAL(hst::string("Resource loading failed!"));
		return 1;
	}
	SL_MESSAGE(hst::string("Resources successfully loaded"));
		

	// A bind macro  is defined between two types and callbacks
	// two add a collision behavour between two collidable objects
#define BIND(TYPE1, TYPE2, CALLBACK)                    \
	CollisionManager::bind( TYPE1 ::ID,                 \
							TYPE2 ::ID,                 \
							testCollidables,            \
							new CCHandler( CALLBACK ));
	// Here we bind all of callbacks, declared before
	/*
	BIND(PlayerBullet, Enemy,  playerbullet_collided_with_enemy);
	BIND(PlayerBullet, ShootingEnemy, playerbullet_collided_with_enemy);
	BIND(PlayerBullet, SuperShootingEnemy, playerbullet_collided_with_supershootingenemy);
	BIND(Player, Bonus, player_collided_with_bonus);
	BIND(Player, EnemyBullet, player_collided_with_enemy_or_enemybullet);
	BIND(Player, Enemy, player_collided_with_enemy_or_enemybullet);
	BIND(Player, ShootingEnemy, player_collided_with_enemy_or_enemybullet);
	BIND(Player, SuperShootingEnemy, player_collided_with_enemy_or_enemybullet);
	*/
#undef BIND
	
	// Create and run game
	PlayingGame = new Game();
	PlayingGame->run();
	delete PlayingGame;


	// If we are here, this means, that window is closed and we can exit the main loop
	return 0;
}

