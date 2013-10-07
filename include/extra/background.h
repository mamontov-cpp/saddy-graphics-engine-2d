/*! \file    background.h
    \author  HiddenSeeker

	Contains background node definitions
*/
#include "../sprite.h"
#include "../texturemanager.h"

namespace sad
{
class Background: public Sprite
{
 SAD_OBJECT
 public:
	    /*! Creates background from texture with name
			\param[in] name name of texture
			\param[in] r rectangle
	    */
	    Background(const sad::String & name,const hRectF & r);	
		/*! Creates background from texture with name
			\param[in] name name of texture
	    */
		Background(const sad::String & name);
		/*! Destructor
		*/
		~Background();
};

}