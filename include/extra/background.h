/*! \file    background.h
    \author  HiddenSeeker

	Contains background node definitions
*/
#include "scene.h"
#include "texturemanager.h"

class Background: public sad::BasicNode
{
 private:
	      sad::Texture * m_tex;
 public:
	    static int Type;
	    /*! Creates background from texture with name
	    */
	    Background(const hst::string & name);	
		/*! Renders
		*/
		void render();
		/*! Destructor
		*/
		~Background();
};