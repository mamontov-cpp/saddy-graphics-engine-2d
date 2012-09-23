/* \file spritedatabase.h
   \author HiddenSeeker

   A database for holding all of the sprites
*/
#include "xmlconfigloader.h"
#include <QImage>
#pragma once
/** Database of sprites and data
 */
class AbstractSpriteDatabase
{

};

/** Extract image from big texture
	\param[in] im image data
	\param[in] t  template 
	\return image data
 */
QImage extractImage(const QImage & im, Sprite2DTemplate & t);
