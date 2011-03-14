#ifndef LIB

#include "../include/renderer.h"
#include "../include/input.h"
#include "ftfont.h"
#include <time.h>

FTFont *  testfont=NULL;
FTFont *  testfont2=NULL;

unsigned char cff[50]="Кириллица !";
class FTNode: public sad::BasicNode
{
 private:
          clock_t beg;
		  float t1;
		  float t2;
 public:
	    static int Type;        //!< Type
        /*! Creates a simple node
		*/
		FTNode() {beg=0;t1=0;t2=0;}
        /*! Renders it
		*/
		void render()  
		{ 
		  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		  glLoadIdentity(); 
		  testfont->render("Fear the moving label!\n!",120,240); 
		  if (clock()-beg>CLOCKS_PER_SEC/6)
		  {
		   beg=clock();
		   //for (int i=0;i<13;i++)
		   //	  ++(cff[i]);
			
		  }
		  //glPushMatrix();
		  //glLoadIdentity();
		  //t1+=0.05;
		  //t2+=0.04;
		  //glRotatef(t1,0,0,1);
		  //glScalef(1,0.9+.6*cos(t2/5),1);
		  testfont2->render((const char *)cff,320-160*cos(0.1*t1),120+120*sin(0.1*t2)); 
		}
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
  glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
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
