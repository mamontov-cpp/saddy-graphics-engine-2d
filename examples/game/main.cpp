#include "renderer.h"
#include "input.h"
#include "fontmanager.h"
#include "player.h"
#include "background.h"
#include "statelabel.h"
#include "lightmodel.h"
#include "orthocamera.h"
#include "png/picopngloader.h"

#ifdef WIN32
#ifndef MINGW
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#endif
#endif


void rend_quit(const sad::Event & o)
{
	sad::Renderer::ref()->quit();
}
void rend_toggle(const sad::Event & o)
{
	if (!paused)
		sad::Renderer::ref()->toggleFullscreen();
}
void rend_pause(const sad::Event & o)
{
	paused=!paused;
}
void rend_mousemove(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		::s3d::point p=PlayerInstance::ref()->middle();
		float af=atan2(o.y-p.y(),o.x-p.x());
		PlayerInstance::ref()->setAngle(af);
	}
}
void rend_mouseclick(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		PlayerInstance::ref()->shoot();
	}
}
#define P_SPEED 1.0
#define N_SPEED -1.0
void rend_up(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityY(P_SPEED);
	}
}
void rend_down(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityY(N_SPEED);
	}
}
void rend_speed_zero(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		if (PlayerInstance::ref()->key()!=o.key) 
			return;
		PlayerInstance::ref()->toggleVelocityX(0.0f);
		PlayerInstance::ref()->toggleVelocityY(0.0f);
	}
}

void rend_left(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityX(N_SPEED);
	}
}
void rend_right(const sad::Event & o)
{
	if (PlayerInstance::ref() && !paused)
	{
		PlayerInstance::ref()->key()=o.key;
		PlayerInstance::ref()->toggleVelocityX(P_SPEED);
	}
}

static float light=0.0f;
void light_inc(const sad::Event & o)
{
  light+=2.5;
  sad::Light::AQuadratic(0,light);
}
void light_dec(const sad::Event & o)
{
  light-=2.5;
  sad::Light::AQuadratic(0,light);
}
void light_enable(const sad::Event & o)
{
 sad::ColorMaterial::disable();
 sad::ColorMaterial::set(sad::Both,sad::ColorMaterial::DiffuseAmbient);
 sad::ColorMaterial::enable();
 
 sad::LightModel::enable();
 sad::LightModel::setAmbient(0.5f,0.5f,0.5f,1.0f);


 sad::Light::enable(0);
 sad::Light::set(0,sad::Light::Ambient,1.0f,0.0f,0.0f,1.0f);
 sad::Light::locate(0,0.0f,0.0f,0.5f,1.0f); 
 sad::Light::AQuadratic(0,light);
 sad::Light::AConstant(0,1.0f);
}
void light_disable(const sad::Event & o)
{
	sad::Light::disable(0);
	sad::LightModel::disable();
}


#include<math.h>
#include<time.h>

inline bool loadTex(const char * from,const char * texname)
{
	sad::Texture * tex=new sad::Texture();
	bool res=tex->load(hst::string(from));
	sad::TextureManager::ref()->add(texname,tex);
	tex->makePOT();
	if (!res) 
		SL_FATAL(hst::string("Loading \"")+hst::string(from)+hst::string("\" failed"));
	return res;
}
inline bool loadSprite(const char * from,const char * texname)
{
	sad::Texture * tex=new sad::Texture();
	bool res=tex->load(hst::string(from));
	tex->setAlpha(255,hst::color(255,255,255),90);
	tex->makePOT();
	if (!res) 
		SL_FATAL(hst::string("Loading \"")+hst::string(from)+hst::string("\" failed"));
	
	tex->makePOT();
	
	sad::TextureManager::ref()->add(texname,tex);
	return res;
}

bool toggle_idle(int)
{
	sad::Scene * sc=sad::Renderer::ref()->getCurrentScene();

	sc->clear();
	killTestingTask();
	sc->add(new sad::Background("title"));
	sc->add(new StateLabel(HIGHSCORE,"times_large"));
	sc->add(new EnemyEmitter(IDLE_RAIN));
	
	return true;
}
bool toggle_play(int)
{
	sad::Scene * sc=sad::Renderer::ref()->getCurrentScene();
	
	current_score=0;
	player_health_point=10;

	sc->clear();
	addTestingTask();
	sc->add(new sad::Background("background"));
	sc->add(new StateLabel(PLAYERSTATE,"times_large"));
	sc->add(new EnemyEmitter(REAL_SPAWN));
    sc->add(new Player(hPointF(320.0,240.0)));

	return true;
}

void toggle_state(const sad::Event & o)
{
	if (StateMachine::state()==IDLE_STATE)
		StateMachine::pushState(PLAY_STATE);
}

void playerbullet(Collidable * bullet,Collidable * enemy)
{
	current_score+=100;
	if (current_score>high_score) { high_score=current_score; }
	enemy->die();
}
void playerbonus(Collidable * player, Collidable * bonus)
{
	current_score+=50;
	if (current_score>high_score) { high_score=current_score; }
	++player_health_point;
	bonus->die();
}
void playerenemybullet(Collidable * player, Collidable * enemybullet)
{
  enemybullet->die();
  --player_health_point;
  if (player_health_point<=0) 
  {
	  PlayerInstance::ref()->die(); 
	  StateMachine::pushState(IDLE_STATE); 
  }
}

typedef CMHandler<Collidable,Collidable> CCHandler;
void pbssenemy(Collidable * bullet, SuperShootingEnemy * p)
{
	p->hit();
	bullet->die();
}

void testplayer(const sad::Event & ev);
#ifdef WIN32
#ifndef MSVC_RELEASE
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
#else
int main(int argc, char** argv)
#endif
#else
int main(int argc, char** argv)
#endif
{
	bool res=true; //!< Is loading succeeded?
	
	sad::log::FileTarget * fl = new sad::log::FileTarget();
	fl->open("log.txt");
	sad::log::ConsoleTarget * tg = new sad::log::ConsoleTarget(
		"{0}: [{1}] {3}{2}{4}", 6, true,
#ifdef _MSC_VER
		true
#else
		false
#endif
	);
	sad::Log::ref()->addTarget(fl).addTarget(tg);

	SL_FATAL   ("This is sad::Game");
	SL_CRITICAL("It\'s a game about a very sad");
	SL_WARNING ("smiley face");
	SL_MESSAGE ("which wants to survive");
	SL_DEBUG   ("against other smiley faces");
	SL_USER    ("it\'s sure hard for him...", "END");

	sad::Renderer::ref()->init(sad::Settings(640,480,false));
	
	sad::Renderer::ref()->textures()->setLoader("PNG", new PicoPNGTextureLoader());
	
	
	SL_MESSAGE("Renderer successfully initialized!");	
	srand((unsigned int)time(NULL));


	//Loading fonts
	sad::TMFont * fnt1=new sad::TMFont;
	bool res1=true;
	res1=fnt1->load("examples/game/georgia2.PNG","examples/game/georgia2.CFG",hst::color(255,255,255),false);
	res=res && res1;
	if (!res1)
		SL_FATAL(hst::string("Loading \"times_large\" failed"));
	sad::Renderer::ref()->fonts()->add(fnt1,"times_large");

    sad::TMFont * fnt2=new sad::TMFont;
	bool res2= fnt1->load("examples/game/times_lg.PNG","examples/game/times_lg.CFG");
	res=res && res2;
	if (!res2)
		SL_FATAL(hst::string("Loading \"times_lg\" failed"));
	sad::Renderer::ref()->fonts()->add(fnt2,"times_lg");

	//Loading sprites
	res=res && loadTex("examples/game/title.tga","title");
	res=res && loadTex("examples/game/ingame.tga","background");
    res=res && loadSprite("examples/game/objects.bmp","objects"); 
    if (!res)
	{
		SL_FATAL(hst::string("Resource loading failed!"));
		return 1;
	}
	SL_MESSAGE(hst::string("Resources successfully loaded"));
	
	sad::Scene * sc= new sad::Scene();
	sad::Renderer::ref()->setCurrentScene(sc);
	sad::Renderer::ref()->setWindowTitle("sad::Game");
	
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
    
	sad::Input::ref()->bindKeyDown('O',light_enable);
	sad::Input::ref()->bindKeyDown('P',light_disable);
	sad::Input::ref()->bindKeyDown('K',light_inc);
	sad::Input::ref()->bindKeyDown('L',light_dec);

	//sad::Input::inst()->bindKeyDown('J',testplayer);

	sad::Input::ref()->setMouseClickHandler(new sad::EventHandler(rend_mouseclick));
	sad::Input::ref()->setMouseMoveHandler(new sad::EventHandler(rend_mousemove));
	
	SL_MESSAGE("Input handlers bound succesfully");

	StateMachine::bindState(IDLE_STATE,new StateHandler(toggle_idle));
	StateMachine::bindState(PLAY_STATE,new StateHandler(toggle_play));
	//Here must be an initialization of engine, and running it
	SL_MESSAGE("States bound successtully");


	CollisionManager::bind(PlayerBullet::ID,Enemy::ID,testCollidables,new CCHandler(playerbullet));
	CollisionManager::bind(PlayerBullet::ID,ShootingEnemy::ID,testCollidables,new CCHandler(playerbullet));
	CollisionManager::bind(PlayerBullet::ID,SuperShootingEnemy::ID,testCollidables,new CMHandler<Collidable,SuperShootingEnemy>(pbssenemy));				 
	CollisionManager::bind(Player::ID,Bonus::ID,testCollidables,new CCHandler(playerbonus));				 
	CollisionManager::bind(Player::ID,EnemyBullet::ID,testCollidables,new CCHandler(playerenemybullet));
	CollisionManager::bind(Player::ID,Enemy::ID,testCollidables,new CCHandler(playerenemybullet));
	CollisionManager::bind(Player::ID,ShootingEnemy::ID,testCollidables,new CCHandler(playerenemybullet));
	CollisionManager::bind(Player::ID,SuperShootingEnemy::ID,testCollidables,new CCHandler(playerenemybullet));

	SL_MESSAGE("Collisions bound successtully");
	
	SL_MESSAGE("Mips built successtully");	
	sad::Renderer::ref()->textures()->buildAll();

	StateMachine::pushState(IDLE_STATE);
	sad::Renderer::ref()->getCurrentScene()->setCamera(new OrthoCamera());
	
	//Set light
	sad::ColorMaterial::disable();
	sad::ColorMaterial::set(sad::Both,sad::ColorMaterial::DiffuseAmbient);
	sad::ColorMaterial::enable();

	sad::Renderer::ref()->toggleFixedOn();
	
	SL_MESSAGE("Will start now");	
	sad::Renderer::ref()->run();
	return 0;
}

