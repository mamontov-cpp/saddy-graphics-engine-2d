/*! \file    background.h
    \author  HiddenSeeker

	Contains background node definitions
*/
#include "../sprite.h"
#include "../texturemanager.h"

class Background: public Sprite
{
 SAD_NODE
 public:
	    /*! Creates background from texture with name
	    */
	    Background(const hst::string & name);	
		/*! Destructor
		*/
		~Background();
};
