/* \file spritedatabase.h
   \author HiddenSeeker

   A database for holding all of the sprites
*/
#include "xmlconfigloader.h"
#include <QImage>
#include <QHash>
#pragma once
/** Database of sprites and data
 */
class AbstractSpriteDatabase
{

};


/** Extracts texture paths
	\param[in] c container
 */
QVector<QString> extractTexturePaths(const Sprite2DTemplateContainer & c);

/** Tries to load all of images to data
	\param[in] loads a texture paths
	\param[out] images images hash
	\return whether loading was successfull
 */
bool tryLoadImages(const QVector<QString> & loads, QHash<QString, QImage> & images);
/** Extract image from big texture
	\param[in] im image data
	\param[in] t  template 
	\return image data
 */
QImage extractImage(const QImage & im, Sprite2DTemplate & t);
