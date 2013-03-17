#include "renderer.h"
#include "input.h"
#include "fontmanager.h"
#include "player.h"
#include "background.h"
#include "statelabel.h"
#include "lightmodel.h"
#include "orthocamera.h"

#ifdef WIN32
#ifndef MINGW
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#endif
#endif


void rend_quit(const sad::Event & o)
{
	sad::Renderer::instance().quit();
}
void rend_toggle(const sad::Event & o)
{
	if (!paused)
		sad::Renderer::instance().toggleFullscreen();
}
void rend_pause(const sad::Event & o)
{
	paused=!paused;
}
void rend_mousemove(const sad::Event & o)
{
	if (PlayerInstance::i() && !paused)
	{
		::s3d::point p=PlayerInstance::i()->middle();
		float af=atan2(o.y-p.y(),o.x-p.x());
		PlayerInstance::i()->setAngle(af);
	}
}
void rend_mouseclick(const sad::Event & o)
{
	if (PlayerInstance::i() && !paused)
	{
		PlayerInstance::i()->shoot();
		hst::log::inst()->write(o.x).write(hst::string(" ")).write(o.y).write(hst::string(" ")).write(o.z).write(hst::string("\n"));
	}
}
#define P_SPEED 1.0
#define N_SPEED -1.0
void rend_up(const sad::Event & o)
{
	if (PlayerInstance::i() && !paused)
	{
		PlayerInstance::i()->key()=o.key;
		PlayerInstance::i()->toggleVelocityY(P_SPEED);
	}
}
void rend_down(const sad::Event & o)
{
	if (PlayerInstance::i() && !paused)
	{
		PlayerInstance::i()->key()=o.key;
		PlayerInstance::i()->toggleVelocityY(N_SPEED);
	}
}
void rend_speed_zero(const sad::Event & o)
{
	if (PlayerInstance::i() && !paused)
	{
		if (PlayerInstance::i()->key()!=o.key) 
			return;
		PlayerInstance::i()->toggleVelocityX(0.0f);
		PlayerInstance::i()->toggleVelocityY(0.0f);
	}
}

void rend_left(const sad::Event & o)
{
	if (PlayerInstance::i() && !paused)
	{
		PlayerInstance::i()->key()=o.key;
		PlayerInstance::i()->toggleVelocityX(N_SPEED);
	}
}
void rend_right(const sad::Event & o)
{
	if (PlayerInstance::i() && !paused)
	{
		PlayerInstance::i()->key()=o.key;
		PlayerInstance::i()->toggleVelocityX(P_SPEED);
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
	sad::TextureManager::instance()->load(texname,tex);
	tex->makePOT();
	if (!res) 
		hst::log::inst()->owrite(hst::string("Loading \"")+hst::string(from)+hst::string("\" failed"));
	return res;
}
inline bool loadSprite(const char * from,const char * texname)
{
	sad::Texture * tex=new sad::Texture();
	bool res=tex->load(hst::string(from));
	tex->setAlpha(255,hst::color(255,255,255),90);
	tex->makePOT();
	if (!res) 
		hst::log::inst()->owrite(hst::string("Loading \"")+hst::string(from)+hst::string("\" failed"));
	
	tex->makePOT();
	
	sad::TextureManager::instance()->load(texname,tex);
	return res;
}

bool toggle_idle(int)
{
	sad::Scene * sc=sad::Renderer::instance().getCurrentScene();

	sc->performCleanup();
	killTestingTask();
	sc->markForAddition(new Background("title"));
	sc->markForAddition(new StateLabel(HIGHSCORE,"times_large"));
	sc->markForAddition(new EnemyEmitter(IDLE_RAIN));
	
	return true;
}
bool toggle_play(int)
{
	sad::Scene * sc=sad::Renderer::instance().getCurrentScene();
	
	current_score=0;
	player_health_point=10;

	sc->performCleanup();
	addTestingTask();
	sc->markForAddition(new Background("background"));
	sc->markForAddition(new StateLabel(PLAYERSTATE,"times_large"));
	sc->markForAddition(new EnemyEmitter(REAL_SPAWN));
    sc->markForAddition(new Player(hPointF(320.0,240.0)));

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
	  PlayerInstance::i()->die(); 
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
	//Loading fonts
	sad::TMFont * fnt1=new sad::TMFont;
	bool res1=true;
	res1=fnt1->load("examples/georgia2.PNG","examples/georgia2.CFG",hst::color(255,255,255),false);
	//res1=fnt1->load("examples/times_large.PNG","examples/times_large.CFG");
	res=res && res1;
	if (!res1)
		hst::log::inst()->owrite(hst::string("Loading \"times_large\" failed\n"));
       sad::FontManager::add(fnt1,"times_large");

    sad::TMFont * fnt2=new sad::TMFont;
	bool res2= fnt1->load("examples/times_lg.PNG","examples/times_lg.CFG");
	res=res && res2;
	if (!res2)
		hst::log::inst()->owrite(hst::string("Loading \"times_lg\" failed\n"));
	sad::FontManager::add(fnt2,"times_lg");

	

	hst::log::inst()->owrite(hst::string("Resources loaded...\n"));
	
	sad::Renderer::instance().init(sad::Settings(640,480,false));
	printf("Initted renderer!\n");	
	srand(time(NULL));

	//Loading sprites
	res=res && loadTex("examples/title.tga","title");
	res=res && loadTex("examples/ingame.tga","background");
    res=res && loadSprite("examples/objects.bmp","objects"); 
    if (!res)
	{
		hst::log::inst()->write(hst::string("Resource loading failed!\n Exiting...\n"));
		hst::log::inst()->save("log.txt");
		return 1;
	}

	sad::Scene * sc= new sad::Scene();
	sad::Renderer::instance().setCurrentScene(sc);
	sad::Renderer::instance().setWindowTitle("sad::Game");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::inst()->bindKeyDown('F',rend_toggle);
	sad::Input::inst()->bindKeyDown('P',rend_pause);
	sad::Input::inst()->bindKeyDown(KEY_TAB,rend_mouseclick);
	
	sad::Input::inst()->bindKeyDown(KEY_UP,rend_up);
	sad::Input::inst()->bindKeyDown(KEY_DOWN,rend_down);
	sad::Input::inst()->bindKeyDown(KEY_LEFT,rend_left);
	sad::Input::inst()->bindKeyDown(KEY_RIGHT,rend_right);
	sad::Input::inst()->bindKeyUp(KEY_UP,rend_speed_zero);
	sad::Input::inst()->bindKeyUp(KEY_DOWN,rend_speed_zero);
	sad::Input::inst()->bindKeyUp(KEY_LEFT,rend_speed_zero);
	sad::Input::inst()->bindKeyUp(KEY_RIGHT,rend_speed_zero);
	
	
	sad::Input::inst()->bindKeyDown(KEY_ENTER,toggle_state);
    
	sad::Input::inst()->bindKeyDown('O',light_enable);
	sad::Input::inst()->bindKeyDown('P',light_disable);
	sad::Input::inst()->bindKeyDown('K',light_inc);
	sad::Input::inst()->bindKeyDown('L',light_dec);

	//sad::Input::inst()->bindKeyDown('J',testplayer);

	sad::Input::inst()->setMouseClickHandler(new sad::EventHandler(rend_mouseclick));
	sad::Input::inst()->setMouseMoveHandler(new sad::EventHandler(rend_mousemove));
	
	printf("Handlers binded!\n");

	StateMachine::bindState(IDLE_STATE,new StateHandler(toggle_idle));
	StateMachine::bindState(PLAY_STATE,new StateHandler(toggle_play));
	//Here must be an initialization of engine, and running it
	printf("States binded!\nBinding collisions\n");


	CollisionManager::bind(PlayerBullet::ID,Enemy::ID,testCollidables,new CCHandler(playerbullet));
	CollisionManager::bind(PlayerBullet::ID,ShootingEnemy::ID,testCollidables,new CCHandler(playerbullet));
	CollisionManager::bind(PlayerBullet::ID,SuperShootingEnemy::ID,testCollidables,new CMHandler<Collidable,SuperShootingEnemy>(pbssenemy));				 
	CollisionManager::bind(Player::ID,Bonus::ID,testCollidables,new CCHandler(playerbonus));				 
	CollisionManager::bind(Player::ID,EnemyBullet::ID,testCollidables,new CCHandler(playerenemybullet));
	CollisionManager::bind(Player::ID,Enemy::ID,testCollidables,new CCHandler(playerenemybullet));
	CollisionManager::bind(Player::ID,ShootingEnemy::ID,testCollidables,new CCHandler(playerenemybullet));
	CollisionManager::bind(Player::ID,SuperShootingEnemy::ID,testCollidables,new CCHandler(playerenemybullet));

	printf("Building mips!\n");	
	sad::TextureManager::buildAll();

	StateMachine::pushState(IDLE_STATE);
	sad::Renderer::instance().getCurrentScene()->setCamera(new OrthoCamera());
	
	//Set light
	sad::ColorMaterial::disable();
	sad::ColorMaterial::set(sad::Both,sad::ColorMaterial::DiffuseAmbient);
	sad::ColorMaterial::enable();

	printf("Engine started!\n");
	sad::Renderer::instance().toggleFixedOn();
	sad::Renderer::instance().run();
	hst::log::inst()->save("log.txt");

	return 0;
}

