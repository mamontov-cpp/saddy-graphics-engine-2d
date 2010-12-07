//#include "stdafx.h"
#include "renderer.h"
#include "input.h"
#include "testnode.h"
#include "fontnode.h"
#include "texturemanager.h"
#include "fontmanager.h"
#include "collisionmanager.h"
#include "movitem.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

void rend_quit(const sad::Event & o)
{
	sad::Renderer::instance().quit();
}
void rend_toggle(const sad::Event & o)
{
	sad::Renderer::instance().toggleFullscreen();
}
sad::Texture * tex;
void rend_mouseclick(const sad::Event & o)
{
	hst::log::inst()->write(hst::string("Click parameters: \n"));
	hst::log & lg=hst::log::inst()->write(o.x);
	lg.write(' ').write(o.y).write(' ').write(o.z);
	lg.write(hst::string("\n\\Click parameters\n"));

	sad::Renderer::instance().getCurrentScene()->add(new TestNode(tex,o.x,o.y,0.01));
}

void testCollision(Collidable * o1,Collidable * o2)
{

}

#include<math.h>
#include<time.h>

#ifdef WIN32
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
	CollisionManager::bind(0,0,new CollisionHandler(testCollision));

	sad::Renderer::instance().init(sad::Settings(640,480,false));
	
	
	sad::TMFont * fnt=new sad::TMFont;
	bool res=fnt->load("examples/times_red.PNG","examples/times_red.CFG");
    if (res)
	{
		sad::FontManager::add(fnt,"times_red");
	}
	else
	{
		delete fnt;
	}
	tex=new sad::Texture();
	hst::string  testString("examples/times_red.PNG");
	bool result=tex->loadPNG(testString);
	sad::TextureManager::instance()->load("TEST",tex);
    sad::Texture * test=sad::TextureManager::instance()->get("TEST");
	test->setAlpha(32);
	srand(time(NULL));

	sad::Scene * sc= new sad::Scene();


	
	sc->add( new TestNode(test,0.000203f,0.0051091f,0.01f) );
	sc->add( new PlayerBullet(217.0f, 0.2f, 0.15f, 0.5f, test, 0.0f, 0.0f, 0.5f, 0.5f) );
	if (res)
        	sc->add( new TestFontNode(fnt));
	sc->add(new CollisionTester());
	sad::Renderer::instance().setCurrentScene(sc);
	sad::Renderer::instance().setWindowTitle("My new sad window");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::inst()->bindKeyDown('F',rend_toggle);
	sad::Input::inst()->setMouseClickHandler(new sad::EventHandler(rend_mouseclick));
	//Here must be an initialization of engine, and running it

							 
	
	test->buildMipMaps();
	test->enable();
	sad::Renderer::instance().run();
	hst::log::inst()->save("log.txt");

	return 0;
}

