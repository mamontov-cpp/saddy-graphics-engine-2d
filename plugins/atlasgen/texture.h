/*! \file texture.h
	\author HiddenSeeker

	Describes a in-app texture
 */
#pragma once
#include <QtGui/QImage>
#include <QtCore/QRectF>
#include <QtCore/QSizeF>
#include <QtCore/QString>


/*! An in-app texture
 */
class Texture
{
public: 
	/*! An image for texture
	 */
	QImage Image;

	/*! A texture rectangle
	 */
	QRectF TextureRectangle;
	/*! A name for texture
	 */
	QString Name;

	/*! A new texture
		\param[in] name of a texture
	 */ 
	Texture(const QString& name);
	/*! Loads a texture by it's name
		\return whether it was successfull
	 */
	bool load();
	/*! Returns size of a texture
		\return size of texture
	 */
	QSizeF size() const;
};