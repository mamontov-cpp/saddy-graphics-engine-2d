/*! \file   window.h
    \author FreakyBlast
 
*/

#ifndef GL_FRAMEWORK__INCLUDED
#define GL_FRAMEWORK__INCLUDED


#ifndef Window_h__
#define Window_h__

#include <windows.h>
#include "settings.h"

  /*! \class SadWindow
      This class represents working window of Saddy
  */
class SadWindow										
{
public:
	SadWindow(void);
	~SadWindow(void);
};


  /*! Terminating program
      \param[in] window Window to close
	  \sa Deinitialization(void)
  */
void TerminateApplication (GL_Window* window);	

  /*! Enable/disable fullscreen
      \param[in] window Window for fullscreen
  */
void ToggleFullscreen (GL_Window* window);		

/*! Procedure of initialization
  \param[out] window Window parameters
  \param[out] keys Array for storing pressed buttons
  \sa Deinitialize(void), InitSettings(void)
  \return Whether initialization has passed normal or not
*/
BOOL Initialize(GL_Window* window, Keys* keys);

/*! Deinitialization
*/
void Deinitialize(void);

/*! Update scene
  \param[in] Time elapsed since previous call of Update(long long)
  
  Performs update of scene according with time in milliseconds,
  elapsed since last call of this function.
*/
void Update (long long DeltaTime);			

/*! Draw all scene
*/
void Draw (void);									// Отрисовка всей сцены

/*! Initialization of all settings
*/
void InitSettings(void);								// Инициализация настроек

#endif												// GL_FRAMEWORK__INCLUDED

#endif // Window_h__