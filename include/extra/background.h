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
			\param[in] name name of texture
			\param[in] r rectangle
	    */
	    Background(const hst::string & name,const hRectF & r);	
		/*! Creates background from texture with name
			\param[in] name name of texture
	    */
		Background(const hst::string & name);
		/*! Destructor
		*/
		~Background();
};
