//#include "stdafx.h"
#include "renderer.h"
#include "input.h"
#include "testnode.h"
#include "fontnode.h"
#include "texturemanager.h"
#include "fontmanager.h"
#include "collision.h"
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

#include<math.h>
#include<time.h>

#ifdef WIN32
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
#define RUNTEST(R11,R12,R21,R22) hst::log::inst()->write(collides(R11,R12,R21,R22)); \
                                 hst::log::inst()->write('\n');

	
	hst::xyrect t1r11(hPointF(1,0),4,4),t1r12(hPointF(6,7),4,4);
    hst::xyrect t1r21(hPointF(0,12),4,4),t1r22(hPointF(11,5),4,4);

	RUNTEST(t1r11,t1r12,t1r21,t1r22);
	RUNTEST(t1r12,t1r11,t1r21,t1r22);
	RUNTEST(t1r12,t1r11,t1r22,t1r21);
	RUNTEST(t1r11,t1r12,t1r22,t1r21);
    
	hst::xyrect t2r11(hPointF(0,0),4,4),t2r12(hPointF(4,4),4,4);
    hst::xyrect t2r21(hPointF(0,8),4,4),t2r22(hPointF(4,4),4,4);

	RUNTEST(t2r11,t2r12,t2r21,t2r22);
	RUNTEST(t2r12,t2r11,t2r21,t2r22);
	RUNTEST(t2r12,t2r11,t2r22,t2r21);
	RUNTEST(t2r11,t2r12,t2r22,t2r21);

	hst::xyrect t3r11(hPointF(0,0),4,4),t3r12(hPointF(11,11),4,4);
    hst::xyrect t3r21(hPointF(0,11),4,4),t3r22(hPointF(11,0),4,4);

	RUNTEST(t3r11,t3r12,t3r21,t3r22);
	RUNTEST(t3r12,t3r11,t3r21,t3r22);
	RUNTEST(t3r12,t3r11,t3r22,t3r21);
	RUNTEST(t3r11,t3r12,t3r22,t3r21);

	hst::xyrect t4r11(hPointF(0,1),2,2),t4r12(hPointF(3,0),2,2);
    hst::xyrect t4r21(hPointF(0,4),2,2),t4r22(hPointF(3,4),2,2);

	RUNTEST(t4r11,t4r12,t4r21,t4r22);
	RUNTEST(t4r12,t4r11,t4r21,t4r22);
	RUNTEST(t4r12,t4r11,t4r22,t4r21);
	RUNTEST(t4r11,t4r12,t4r22,t4r21);

	hst::xyrect t5r11(hPointF(0,0),2,2),t5r12(hPointF(3,3),2,2);
    hst::xyrect t5r21(hPointF(0,6),2,2),t5r22(hPointF(1,8),2,2);

	RUNTEST(t5r11,t5r12,t5r21,t5r22);
	RUNTEST(t5r12,t5r11,t5r21,t5r22);
	RUNTEST(t5r12,t5r11,t5r22,t5r21);
	RUNTEST(t5r11,t5r12,t5r22,t5r21);

#undef RUNTEST
	hst::log::inst()->save("collisiontest.txt");
	/*
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
	sad::Texture * tex=new sad::Texture();
	hst::string  testString("examples/times_red.PNG");
	bool result=tex->loadPNG(testString);
	sad::TextureManager::instance()->load("TEST",tex);
    sad::Texture * test=sad::TextureManager::instance()->get("TEST");
	test->setAlpha(32);
	srand(time(NULL));

	sad::Scene * sc= new sad::Scene();


	
	sc->add( new TestNode(test,(float)rand()/RAND_MAX*0.2f - 0.1f,(float)rand()/RAND_MAX*0.2f - 0.1f,0.06f) );
	if (res)
        	sc->add( new TestFontNode(fnt));

	sad::Renderer::instance().setCurrentScene(sc);
	sad::Renderer::instance().setWindowTitle("My new sad window");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::inst()->bindKeyDown('F',rend_toggle);	
	//Here must be an initialization of engine, and running it

							 
	
	test->buildMipMaps();
	test->enable();
	sad::Renderer::instance().run();
   */
	return 0;
}

