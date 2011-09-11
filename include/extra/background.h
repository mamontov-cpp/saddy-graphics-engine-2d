/*! \file    background.h
    \author  HiddenSeeker

	Contains background node definitions
*/
#include "../scene.h"
#include "../texturemanager.h"

class Background: public sad::BasicNode
{
 SAD_NODE
 private:
	      sad::Texture * m_tex;
 public:
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
