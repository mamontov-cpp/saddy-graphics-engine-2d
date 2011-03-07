#ifndef LIB

#include "../include/renderer.h"
#include "../include/input.h"
#include "ftfont.h"

FTFont *  testfont=NULL;
FTFont *  testfont2=NULL;

class FTNode: public sad::BasicNode
{
 private:
 public:
	    static int Type;        //!< Type
        /*! Creates a simple node
		*/
		FTNode() {}
        /*! Renders it
		*/
		void render()  {  testfont->render("Test fucker!\n Yeah!",120,240); testfont2->render("Test2!",60,120); }
		/*! Destructs it
		*/
		~FTNode() {}
};

int FTNode::Type=505;

void rend_quit(const sad::Event & o)
{
    delete testfont;
	delete testfont2;
	sad::Renderer::instance().quit();
}

int main(int argc, char** argv)
{
  sad::Renderer::instance().init(sad::Settings(640,480,false));
  printf("Initted renderer!\n");	
  
  testfont=new FTFont();
  testfont2=new FTFont();
  
  if (!testfont->load("test.ttf",32,hst::acolor(255,255,255,0)))
  {
    delete testfont;
	printf("Error: can't load font!\n");
	return 0;
  }
  if (!testfont2->load("test2.ttf",32,hst::acolor(255,255,255,0)))
  {
    delete testfont;
	printf("Error: can't load test2.ttf!\n");
	return 0;
  }
    
  sad::Scene * sc= new sad::Scene();
  sad::Renderer::instance().setCurrentScene(sc);
  sad::Renderer::instance().setWindowTitle("sad::Game");
  sc->add(new FTNode());	
  sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit); 
  
  
  printf("Engine started!\n");
  sad::Renderer::instance().run();
  return 0;
}

#endif
