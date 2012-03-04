/*! \file   sprite2dtemplate.h
	\author HiddenSeeker
	
	Here is described loadable sprite template, that can be easily loaded from file
 */
#include "../primitives/hrect.h"
#include "../templates/hstring.h"
#pragma once

/*! \class Sprite2DTemplate
	
	A class of loadable 2D template
 */
class Sprite2DTemplate
{
 private:
		 hst::string  m_texturename;  //!< Name of texture
		 hRectF       m_texturerect;  //!< Texture coordinates of rectangle
		 hPointF      m_size;         //!< Size of object
 public:
	    /*! Creates empty invalid 2d template
		 */
		inline Sprite2DTemplate()
		{
		}
		/*! Destructor, currently does nothing
		 */
		inline ~Sprite2DTemplate()
		{
		}
		/*! Sets a texture name 
			\param[in] name texture name
		 */
		inline void setTextureName(const hst::string & name) { m_texturename=name; }
		/*! Sets a texture rectangle
			\param[in] rect texture rectangle
		 */
		inline void setTextureRect(const hRectF & rect)  { m_texturerect=rect; }
		/*! Sets a size of object
			\param[in] size size of object
		 */
		inline void setSize(const hPointF & size)  { m_size=size; }
		/*! Returns a texturename
			\return texture name
		 */
		inline const hst::string & textureName() const { return m_texturename; }
		/*! Returns a texture rectangle
			\return texture rectangle
		 */
		inline const hRectF & textureRect() const { return m_texturerect; }
		/*! Returns a size of sprite
			\return size of sprite
		 */
		inline const hPointF & size() const {return m_size; }
};

