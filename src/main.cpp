#include "renderer.h"
#include "input.h"
#include "testnode.h"
#include "fontnode.h"
#include "texturemanager.h"
#include "fontmanager.h"
#include "collisionmanager.h"
#include "movitem.h"
#include "background.h"
#include "statelabel.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

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
void rend_mouseclick(const sad::Event & o)
{
	hst::log::inst()->write(hst::string("Click parameters: \n"));
	hst::log & lg=hst::log::inst()->write(o.x);
	lg.write(' ').write(o.y).write(' ').write(o.z);
	lg.write(hst::string("\n\\Click parameters\n"));

	float fx=((float)rand())/RAND_MAX*0.04-0.02;
	float fy=((float)rand())/RAND_MAX*0.04-0.02;

	sad::Renderer::instance().getCurrentScene()->add(new PlayerBullet(
		                   Vector(fx,fy),
						   BoundingBox(hPointF(o.x,o.y),0.02,0.02),
						   0.9
						  ));
	
}

void playerbullet(Collidable * bullet,Collidable * enemy)
{
	sad::Renderer::instance().getCurrentScene()->markForDeletion(enemy);
}

#include<math.h>
#include<time.h>

inline bool loadTex(const char * from,const char * texname)
{
	sad::Texture * tex=new sad::Texture();
	bool res=tex->load(hst::string(from));
	sad::TextureManager::instance()->load(texname,tex);
	return res;
}
inline bool loadSprite(const char * from,const char * texname)
{
	sad::Texture * tex=new sad::Texture();
	bool res=tex->load(hst::string(from));
	tex->setAlpha(255,hst::color(255,255,255),90);
	sad::TextureManager::instance()->load(texname,tex);
	return res;
}

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
	bool res; //!< Is loading succeeded?
	//Loading fonts
	sad::TMFont * fnt1=new sad::TMFont;
	res=res && fnt1->load("examples/times_large.PNG","examples/times_large.CFG");
	sad::FontManager::add(fnt1,"times_large");

    sad::TMFont * fnt2=new sad::TMFont;
	res=res && fnt1->load("examples/times_lg.PNG","examples/times_lg.CFG");
	sad::FontManager::add(fnt2,"times_lg");

	//Loading sprites
	res=res && loadTex("examples/title.tga","title");
	res=res && loadTex("examples/ingame.tga","background");
    res=res && loadSprite("examples/halfsmile.png","halfsmile");
	res=res && loadSprite("examples/largesmile.png","largesmile");
    res=res && loadSprite("examples/neutral.png","neutral");
	res=res && loadSprite("examples/sad.png","sad");
	res=res && loadSprite("examples/smile.png","smile");
	res=res && loadSprite("examples/enemybullet.bmp","enemybullet");
	res=res && loadSprite("examples/playerbullet.bmp","playerbullet");
	res=res && loadSprite("examples/bonus.bmp","bonus");

    if (!res)
	{
		hst::log::inst()->write(hst::string("Resource loading failed!\n Exiting..."));
		hst::log::inst()->save("log.txt");
		return 1;
	}


	sad::Renderer::instance().init(sad::Settings(640,480,false));
	
		
	srand(time(NULL));

	sad::Scene * sc= new sad::Scene();
	
	sc->add(new CollisionTester());
	sc->add(new Background("title"));
	sc->add(new StateLabel(HIGHSCORE,"times_large"));
	sc->add(new EnemyEmitter(IDLE_RAIN));

	sad::Renderer::instance().setCurrentScene(sc);
	sad::Renderer::instance().setWindowTitle("sad::Game");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::inst()->bindKeyDown('F',rend_toggle);
	sad::Input::inst()->bindKeyDown('P',rend_pause);
	sad::Input::inst()->setMouseClickHandler(new sad::EventHandler(rend_mouseclick));
	//Here must be an initialization of engine, and running it

	CollisionManager::bind(PlayerBullet::Type,Enemy::Type,new CollisionHandler(playerbullet));
	CollisionManager::bind(PlayerBullet::Type,ShootingEnemy::Type,new CollisionHandler(playerbullet));
					 
	
	sad::TextureManager::buildAll();
	sad::Renderer::instance().run();
	hst::log::inst()->save("log.txt");

	return 0;
}

