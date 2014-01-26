/*! \mainpage Multithreading Example
	A simple example of two OpenGL windows running simultaneously
 */
/*! \file main.cpp
	\author HiddenSeeker
	
	A multithreading example is an example of two OpenGL windows in one process
	running simultaneously.
 */
#include <renderer.h>
#include <input/controls.h>
#include <keymouseconditions.h>
#include <input/handlers.h>
#include <fontmanager.h>
#include <orthographiccamera.h>
#include <sadthread.h>
#include <sprite2d.h>
#include <label.h>
#include <texturemappedfont.h>
#include <texturemanager.h>
#include <freetype/font.h>

#ifdef WIN32
#include <windows.h>
#endif

/*! \class EventHandler
    A simple handler which, depending on settings cand quit renderer's main loop
	or just move sprite adapter to a new point, where user clicked. It's used 
	mainly to implement two behaviours
	
	1) When user presses Escape key, then window closes and the game ends

	2) When users clicks on some space on screen, the center of sprite (m_ad field)
	   moves here.

    Current behaviour depends on m_quit variable, which is if set to true quits it
 */
class EventHandler: public sad::input::AbstractHandler
{
 private:
	 sad::Renderer * m_renderer; //!< A current renderer, which working controls belong to
	 sad::Sprite2D * m_ad;     //!< A sprite, which should be moved when user clicks. Could be NULL if m_quit is true
	 bool m_quit;                //!< Whether we should quit renderer on event call
 public:
	 /*! A new handler just consists from these three fields
		 \param[in] r renderer
		 \param[in] a sprite
		 \param[in] quit quit
	  */
	 EventHandler(sad::Renderer * r, sad::Sprite2D * a, bool quit)
	 {
		 m_renderer = r;
		 m_ad = a;
		 m_quit = quit;
	 }
	 /*! This method is called, when  event, specified when passing handler to 
	     sad::Input is occured in window. Currently it implements behaviour specified
		 in header of class
	  */
	 virtual void invoke(const sad::input::AbstractEvent & e)
	 {
		if (m_quit)
		{
			m_renderer->quit();
		}
		else 
		{
			const sad::input::MouseMoveEvent& ev = static_cast<const sad::input::MouseMoveEvent&>(e);
			// Move sprite center to a position
			m_ad->moveTo(ev.pos2D());
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
	/*! Create and set scene for renderer;
	 */
	sad::Scene * scene = new sad::Scene();
	r.setScene(scene);
	
	/* Setup the logging. We redirect all messages to a file, passed as parameter to thread
	   variable
	 */
	sad::log::FileTarget * fl = new sad::log::FileTarget();
	bool b = fl->open((const char *)p);
	r.log()->addTarget( fl);
	
	/* Create 800x600 window in windowed mode and toggle a fixed size of window
	 */
	r.init(sad::Settings(800,600, false));
	r.makeFixedSize();
	
	

	/* Bind built-ing scene to renderer 
	 */
	scene->setRenderer(&r);

	/* Load texture mapped font. 
	   We add it to font manager to be sure, that memory will be freed at exit.
	 */
	sad::TextureMappedFont * fnt2=new sad::TextureMappedFont();
	bool res2= fnt2->load("examples/game/font",  &r);
	if (!res2) {
		SL_LOCAL_FATAL("Failed to load texture-mapped font", r);
		return NULL;
	}
	r.fonts()->add(fnt2,"times_lg");

	/*! Load freetype test font. We set it's rendering color to red, so label will be
		shown as red on screen
	 */
	sad::freetype::Font * fnt1=new sad::freetype::Font();
    bool res1= fnt1->load("ifaceed/EMPORIUM.TTF");
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
	sad::Sprite2D * a = new sad::Sprite2D(tex, sad::Rect2D(sad::Point2D(0,0), sad::Point2D(512,512)), sad::Rect2D(sad::Point2D(0,0), sad::Point2D(512,512)));
	scene->add(a);

	/* Add two labels with different fonts
	 */
	scene->add(
		new sad::Label(fnt1, sad::Point2D(300,200), "FTFont")
	);
	scene->add(
		new sad::Label(fnt2, sad::Point2D(400,400), "TMFont")
	);
	
	/* Here we bind two different handlers with keydown
	 */
	sad::input::Controls * c = r.controls();
	c->add(*sad::input::ET_KeyPress & sad::Esc, new EventHandler(&r, NULL, true));
	c->add(*sad::input::ET_MousePress         , new EventHandler(&r, a, false));
	
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

