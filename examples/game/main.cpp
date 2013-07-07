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
#include "player.h"
#include <background.h>
#include "statelabel.h"
#include <lightmodel.h>
#include <orthocamera.h>
#include <png/picopngloader.h>

#include <math.h>
#include <time.h>


/*! Defines a game high score and current player score
 */
int high_score=0, current_score=0;
/*! Defines a health of player
 */
int player_health_point=10;
/*! Defines a state of game, whether it's paused or not
 */
bool paused=false;


/*! A callback, which shuts down a renderer, ending a main loop for
	it. This is how exit is performed
	\param[in] o unused
 */
void rend_quit(const sad::Event & o)
{
	sad::Renderer::ref()->quit();
}

/*! A callback that is enters a fullscreen.
	
	We disable entering fullscreen, when paused
	\param[in] o unused
 */
void rend_toggle(const sad::Event & o)
{
	if (!paused)
		sad::Renderer::ref()->toggleFullscreen();
}
/*! A callback to toggle pausing
	\param[in] o unused
 */
void rend_pause(const sad::Event & o)
{
	paused=!paused;
}
/*! A callback, that when game is not paused, sets am facing angle
	for player to a computed angle
	\param[in] o event with coordinates
 */
void rend_mousemove(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		::s3d::point p=PlayerInstance::ref()->middle();
		float af=atan2(o.y-p.y(),o.x-p.x());
		PlayerInstance::ref()->setAngle(af);
	}
}
/*! A callback to make player's alter-ego shoot when player clicks on window
	\param[in] o unused
 */
void rend_mouseclick(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		PlayerInstance::ref()->shoot();
	}
}
/*! A positive speed as passed distance in second
 */
#define P_SPEED 1.0
/*! A negative speed as passed distance in second
 */
#define N_SPEED -1.0
/*! A callback, which sets vertical speed, heading to top
	\param[in] o event, that holds a key
 */
void rend_up(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		// Set lastspressed key, to cancel moving on key release
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityY(P_SPEED);
	}
}
/*! A callback, which sets vertical speed, heading to bottom
	\param[in] o event, that holds a key
 */
void rend_down(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		// Set last pressed key, to cancel moving on key release
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityY(N_SPEED);
	}
}
/*! A callback, which sets vertical speed, heading to bottom.
	This action is performed on key release
	
	\param[in] o event key
 */
void rend_speed_zero(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		// If last released key doesn't match with other key, we don't reset speed
		if (PlayerInstance::ref()->key()!=o.key) 
			return;
		PlayerInstance::ref()->toggleVelocityX(0.0f);
		PlayerInstance::ref()->toggleVelocityY(0.0f);
	}
}
/*! A callback, which sets horizontal speed, heading to left
	\param[in] o event, that holds a key
 */
void rend_left(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		// Set last pressed key, to cancel moving on key release
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityX(N_SPEED);
	}
}
/*! A callback, which sets horizontal speed, heading to right
	\param[in] o event, that holds a key
 */
void rend_right(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		// Set last pressed key, to cancel moving on key release
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityX(P_SPEED);
	}
}

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

/*! A state transition callback function, which is called, when we are entering
	a title screen.
	\return true to ensure, that transition was successfull
 */
bool toggle_idle(int)
{
	sad::Scene * sc=sad::Renderer::ref()->getCurrentScene();

	sc->clear();
	// We remove collision testing task, so items can freely move through each other.
	killTestingTask();
	// Fill screne with background, label and rain of element (the last object does that).
	sc->add(new sad::Background("title"));
	sc->add(new StateLabel(HIGHSCORE,"times_lg"));
	sc->add(new EnemyEmitter(IDLE_RAIN));
	
	return true;
}
/*! A state transition callback function, which is called, when we are entering
	an in-game screen screen.
	\return true to ensure, that transition was successfull
 */
bool toggle_play(int)
{
	sad::Scene * sc=sad::Renderer::ref()->getCurrentScene();
	
	current_score=0;
	player_health_point=10;

	sc->clear();
	// We enable collision testing task, adding some item for computing collisions
	addTestingTask();
	// We add background, emitter and new player's alter-ego at 320,240 - center of screen
	sc->add(new sad::Background("background"));
	sc->add(new StateLabel(PLAYERSTATE,"times_lg"));
	sc->add(new EnemyEmitter(REAL_SPAWN));
    sc->add(new Player(hPointF(320.0,240.0)));

	return true;
}

/*! A callback on ENTER. Toggles in-game state if currently we are in idle state
	\param[in] o unused
 */
void toggle_state(const sad::Event & o)
{
	if (StateMachine::state()==IDLE_STATE)
		StateMachine::pushState(PLAY_STATE);
}

/*! A callback, called when bullet, owned by player, hits an enemy
	An enemy dies and score rises by 100
	\param[in] bullet object 1
	\param[in] enemy  object 2
 */
void playerbullet_collided_with_enemy(Collidable * bullet,Collidable * enemy)
{
	current_score+=100;
	if (current_score>high_score) { high_score=current_score; }
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
	current_score+=50;
	if (current_score>high_score) { high_score=current_score; }
	++player_health_point;
	bonus->die();
}
/*! A callback, called when player hits a bullet
	A life decreases, and if becomes less than zero, player dies and start screen is toggled
	\param[in] player object 1
	\param[in] enemyorbullet  object 2
 */
void player_collided_with_enemy_or_enemybullet(Collidable * player, Collidable * enemyorbullet)
{
  enemyorbullet->die();
  --player_health_point;
  if (player_health_point<=0) 
  {
	  PlayerInstance::ref()->die(); 
	  StateMachine::pushState(IDLE_STATE); 
  }
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
	

	// Init new empty scene with orthographic projection
	sad::Scene * sc= new sad::Scene();
	sc->setCamera(new OrthoCamera());
	sad::Renderer::ref()->setCurrentScene(sc);
	sad::Renderer::ref()->setWindowTitle("sad::Game");
	

	// Setup all input callbacks
	sad::Input::ref()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::ref()->bindKeyDown('F',rend_toggle);
	sad::Input::ref()->bindKeyDown('P',rend_pause);
	sad::Input::ref()->bindKeyDown(KEY_TAB,rend_mouseclick);
	
	sad::Input::ref()->bindKeyDown(KEY_UP,rend_up);
	sad::Input::ref()->bindKeyDown(KEY_DOWN,rend_down);
	sad::Input::ref()->bindKeyDown(KEY_LEFT,rend_left);
	sad::Input::ref()->bindKeyDown(KEY_RIGHT,rend_right);

	sad::Input::ref()->bindKeyUp(KEY_UP,rend_speed_zero);
	sad::Input::ref()->bindKeyUp(KEY_DOWN,rend_speed_zero);
	sad::Input::ref()->bindKeyUp(KEY_LEFT,rend_speed_zero);
	sad::Input::ref()->bindKeyUp(KEY_RIGHT,rend_speed_zero);
	
	
	sad::Input::ref()->bindKeyDown(KEY_ENTER,toggle_state);

	sad::Input::ref()->setMouseClickHandler(new sad::EventHandler(rend_mouseclick));
	sad::Input::ref()->setMouseMoveHandler(new sad::EventHandler(rend_mousemove));
	SL_MESSAGE("Input handlers bound succesfully");

	// Populate state machine with state handlers
	StateMachine::bindState(IDLE_STATE,new StateHandler(toggle_idle));
	StateMachine::bindState(PLAY_STATE,new StateHandler(toggle_play));
	SL_MESSAGE("States bound successfully");

	// A bind macro  is defined between two types and callbacks
	// two add a collision behavour between two collidable objects
#define BIND(TYPE1, TYPE2, CALLBACK)                    \
	CollisionManager::bind( TYPE1 ::ID,                 \
							TYPE2 ::ID,                 \
							testCollidables,            \
							new CCHandler( CALLBACK ));
	// Here we bind all of callbacks, declared before
	BIND(PlayerBullet, Enemy,  playerbullet_collided_with_enemy);
	BIND(PlayerBullet, ShootingEnemy, playerbullet_collided_with_enemy);
	BIND(PlayerBullet, SuperShootingEnemy, playerbullet_collided_with_supershootingenemy);
	BIND(Player, Bonus, player_collided_with_bonus);
	BIND(Player, EnemyBullet, player_collided_with_enemy_or_enemybullet);
	BIND(Player, Enemy, player_collided_with_enemy_or_enemybullet);
	BIND(Player, ShootingEnemy, player_collided_with_enemy_or_enemybullet);
	BIND(Player, SuperShootingEnemy, player_collided_with_enemy_or_enemybullet);
#undef BIND
	SL_MESSAGE("Collisions bound successfully");


	
	// Load textures to videocard memory and build mipmaps
	sad::Renderer::ref()->textures()->buildAll();
	SL_MESSAGE("Mips built successfully");	

	// Toggle starting screen
	StateMachine::pushState(IDLE_STATE);
	
	// Set window size to be fixed
	sad::Renderer::ref()->toggleFixedOn();
	
	// Run an engine, starting a main loop
	SL_MESSAGE("Will start now");	
	sad::Renderer::ref()->run();

	// If we are here, this means, that window is closed and we can exit the main loop
	return 0;
}

