/*! \mainpage Multithreading Example
	A simple example of two OpenGL windows running simultaneously
 */
/*! \file main.cpp
	\author HiddenSeeker
	
	A multithreading example is an example of two OpenGL windows in one process
	running simultaneously.
 */
#include "renderer.h"
#include "input.h"
#include "fontmanager.h"
#include "orthocamera.h"
#include "png/picopngloader.h"
#include "sadthread.h"
#include "input.h"
#include "sprite2dadapter.h"
#include "label.h"
#include "ftfont.h"


/*! \class EventHandler
    A simple handler which, depending on settings cand quit renderer's main loop
	or just move sprite adapter to a new point, where user clicked. It's used 
	mainly to implement two behaviours
	
	1) When user presses Escape key, then window closes and the game ends

	2) When users clicks on some space on screen, the center of sprite (m_ad field)
	   moves here.

    Current behaviour depends on m_quit variable, which is if set to true quits it
 */
class EventHandler: public sad::EventHandler
{
 private:
	 sad::Renderer * m_renderer; //!< A current renderer, which working controls belong to
	 Sprite2DAdapter * m_ad;     //!< A sprite, which should be moved when user clicks. Could be NULL if m_quit is true
	 bool m_quit;                //!< Whether we should quit renderer on event call
 public:
	 /*! A new handler just consists from these three fields
		 \param[in] r renderer
		 \param[in] a sprite
		 \param[in] quit quit
	  */
	 EventHandler(sad::Renderer * r, Sprite2DAdapter * a, bool quit)
	 {
		 m_renderer = r;
		 m_ad = a;
		 m_quit = quit;
	 }
	 /*! This method is called, when  event, specified when passing handler to 
	     sad::Input is occured in window. Currently it implements behaviour specified
		 in header of class
	  */
	 virtual void operator()(const sad::Event & o)
	 {
		if (m_quit)
		{
			m_renderer->quit();
		}
		else 
		{
			// This point is center of sprite
			sad::Point2D center = m_ad->pos();
			// Since Sprite2DAdapter::move uses relative coordinates to move center of sprite
			// we must compute distance between point, where user clicked ands center of sprite
			// and call it.
			sad::Point2D v = sad::Point2D(o.x,o.y) - center;
			m_ad->move(v);
		}
	 }
	 /*! This is convenient function for  implementation of sad::Input, which avoids calling
		 handlers with no valid pointer to functions inside of these. So we must tell it, that
		 our handler is safe and it can call it.
		 \return bool false
	  */
	 virtual bool empty() 
	 {
		return false;
	 }
};	

// On some Intel GMA modules,
// building a few mipmaps at the same time or rendering scenes
// causes segault and texture corruption. 
// So we create a lock, which disables it.
// If you don't have these kind of cards feel free to comment,
// It has been tested on NVidia card without locks ans seems to work.
sad::Mutex mipmap_part_mutex;
// We override default scene to avoid bugs with IntelGMA modules
// All we need to do is to override ::render()
class InterlockedScene:public sad::Scene
{
 public:
	 /*! So, all this scene is doing is
		 1) Locking on mutex, so other scene won't be rendered
		 
		 2) Render scene as normal

		 3) Unlocking on mutex
	  */
	 virtual void render() {
		mipmap_part_mutex.lock();
		this->sad::Scene::render();
		mipmap_part_mutex.unlock();
	 }
};


/*! This is simple thread function, which inits a renderer, with simple scene of
    two kind of fonts and sprite. Also it creates separate log for work, and sets 
	it's separate callbacks to exit on Escape and move sprite on user click.

	This demo does not use own assets, instead it loads in from "game" example and
	IFaceEditor plugin.

	\param[in] p log name as pointer to characters.
	\return 0 in most cases
 */
int thread(void * p)
{
	/* Firstly, we create our own renderer, which will do all kind of tasks
	 */ 
	sad::Renderer r;
	
	/* Setup the logging. We redirect all messages to a file, passed as parameter to thread
	   variable
	 */
	sad::log::FileTarget * fl = new sad::log::FileTarget();
	bool b = fl->open((const char *)p);
	r.log()->addTarget( fl);
	
	/* Create 800x600 window in windowed mode and toggle a fixed size of window
	 */
	r.init(sad::Settings(800,600, false));
	r.toggleFixedOn();

	/* Create new scene and toggle orthographic projection.
	   Note, that we pass our renderer to camera - that's how it will know size of window
	 */
	r.setCurrentScene(new InterlockedScene());
	r.getCurrentScene()->setCamera(new OrthoCamera(false,&r));

	/* Load texture mapped font. We don't make background transparent, because  font
	   has black letters, which cannot be seen on black background (4th parameter). 

	   In that case 3rd paramter is ignored. 

	   Note, that we pass renderer, because we need to set explicitly, which container
	   we must build.

	   We add it to font manager to be sure, that memory will be freed at exit.
	 */
	sad::TMFont * fnt2=new sad::TMFont;
	bool res2= fnt2->load("examples/game/times_lg.PNG","examples/game/times_lg.CFG",
		                  sad::Color(0,255,0), false, &r);
	if (!res2) {
		SL_LOCAL_FATAL("Failed to load texture-mapped font", r);
		return NULL;
	}
	r.fonts()->add(fnt2,"times_lg");

	/*! Load freetype test font. We set it's rendering color to red, so label will be
		shown as red on screen
	 */
	FTFont * fnt1=new FTFont();
    bool res1= fnt1->load("ifaceed/EMPORIUM.TTF", 22);
	if (res1 == false) {
		SL_LOCAL_FATAL("Failed to load font...", r);
		return NULL;
  	}
	fnt1->setColor(sad::AColor(255,0,0,0));
	r.fonts()->add(fnt1, "font");
    
	/*! Load simple texture.
		
		We add it to texture manager to be sure, that memory will be freed at exit.
	 */
	sad::Texture * tex = new sad::Texture();
	if (tex->load("examples/game/ingame.tga",&r) == false)
	{
		SL_LOCAL_FATAL("Failed to load texture...", r);
		return NULL;
	}
	r.textures()->add("tex1", tex);

	/* Create simple sprite. 512x512 is a size of texture and it's passed as second parameter
	 */
	Sprite2DAdapter * a = new Sprite2DAdapter(tex, sad::Rect2D(sad::Point2D(0,0), sad::Point2D(512,512)), sad::Rect2D(sad::Point2D(0,0), sad::Point2D(512,512)));
	r.getCurrentScene()->add(a);

	/* Add two labels with different fonts
	 */
	r.getCurrentScene()->add(
		new Label(fnt1, "FTFont", sad::Point2D(300,200), &r)
	);
	r.getCurrentScene()->add(
		new Label(fnt2, "TMFont", sad::Point2D(400,400), &r)
	);
	
	/* Here we bind two different handlers with keydown
	 */
	r.controls()->bindKeyDown(KEY_ESC,  new EventHandler(&r, NULL, true));
	r.controls()->setMouseDownHandler(new EventHandler(&r, a, false));

	/* Because of bugs in Intel GMA drivers we must lock on thread, before building mipmaps
	 */
	mipmap_part_mutex.lock();
	r.textures()->buildAll();
	mipmap_part_mutex.unlock();
	
	/* Start main rendering loop. Execution will not progress further, until user closes window
	   or press Escape
	 */
	r.run();

	// Return default value
	return 0;
}



/* This macro manupilation is to make console window hidden in MSVC window
 */
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
	// Here we create two waitable threads
	sad::Thread a(thread,const_cast<void *>((void*)"thread1.txt"));
	sad::Thread b(thread,const_cast<void *>((void*)"thread2.txt"));
	// Run them
	a.run();
	b.run();
	// And wait
	a.wait();
	b.wait();
	return 0;
}

