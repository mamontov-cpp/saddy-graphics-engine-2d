/*! \file   render.h
\author FreakyBlast

\brief  Declaration of Renderer.

*/

#ifndef Render_h__
#define Render_h__

#include "settings.h"
#include "scene.h"
#include "window.h"


namespace Sad{
	/*!\class The main render class 
	*/
	class Renderer
	{
		Renderer(void);
		Renderer(const Renderer& other);
		Renderer& operator=(const Renderer&);
		unsigned long m_starttimer;
		Scene* m_currentscene;
		sadWindow m_currentwindow;
	public:

		~Renderer(void);

		/*! Fills a structure SadWindow
		\param[in] _setting Settings of GLContext
		\param[out] window Structure to filling
		\return Success of operation
		*/
		bool init(Sad::Settings* _settings, sadWindow *window);

		/*! Reshape view
		\param[in] width Needed width
		\param[in] height Needed height
		\param[in] sets Settings of OpenGL context
		*/
		void reshape(int width, int height, Sad::Settings* sets);

		/* HS, FIX IT. I dont know what to do with it*/
		void run();

		/* HS, mainLoop - As I realized you*/
		void mainLoop();

		/*! Gets a current window in the form sadWindow
		\return Current window
		*/
		inline sadWindow* getCurrentWindow(void){return &m_currentwindow;}

		/*! Sets a current window
		\param[in] Structure of window
		*/
		inline void setCurrentWindow(sadWindow* window){m_currentwindow=*window;}

		/*! Quit
		*/
		void quit(void);

		/*Timer Part*/

		/*! Setting of timer
		*/
		inline void setTimer(void)
		{ m_starttimer = clock();}

		/*! Elapsed time in milliseconds
		\return Time in milliseconds
		*/
		inline long elapsedInMSeconds()
		{ return (clock()/(CLOCKS_PER_SEC*1000)) - (m_starttimer/(CLOCKS_PER_SEC*1000));}

		/*! Elapsed time in seconds
		\return Time in seconds
		*/
		inline int elapsedInSeconds()
		{return (clock()/CLOCKS_PER_SEC) - (m_starttimer/CLOCKS_PER_SEC);}

		/*End of Timer Part*/
		/*! Returns instance - Renderer is a classic singleton
		\return Reference to the alone Renderer object
		*/
		static Renderer& instance();

		/*! Returns a current scene, stored in m_currentscene
		\return Current scene
		*/
		inline Scene* getCurrentScene()const      {return m_currentscene;}

		/*! Sets a current scene
		\param[in] Scene to be set as current
		*/
		inline void setCurrentScene(Scene* scene) {m_currentscene=scene; }

	};
}




#endif // Render_h__