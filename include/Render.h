/*! \file   render.h
    \author FreakyBlast

	\brief  Declaration of Render.
 
*/

#ifndef Render_h__
#define Render_h__

#include "settings.h"

/*!\class The main render class 
*/
class SadRender
{
public:
	SadRender(void);
	~SadRender(void);
	BOOL SetupOpenGL(GL_Window *window);
	void Reshape(int width, int height);
};


/*!\var Global variable of our render*/
extern SadRender MainRender;


#endif // Render_h__