/*! \file   sprite2dtemplate.h
	\author HiddenSeeker
	
	Here is described loadable sprite template, that can be easily loaded from file
 */
#include "../primitives/hrect.h"
#include "../primitives/hcolor.h"
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
		 
		 bool         m_transparent;       //!< Whether the template should be transparent
		 hst::color   m_transparencycolor; //!< A color, which should be transparent
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
		/*! Makes a template transparent
			\param[in] color transparent color
		 */
		inline void makeTransparent(const hst::color & color)
		{
			m_transparent=true;
			m_transparencycolor=color;
		}
		/*! Makes a template nontransparent
		 */
		inline void makeNonTransparent()
		{
			m_transparent=false;
		}
		/*! Whether template is transparent
			\return if transparent returns true
		 */
		inline bool isTransparent() { return m_transparent; }
		/*! Returns color of transparemcy for the sprite
			\return color
		 */
		inline const hst::color & transparencyColor() { return m_transparencycolor; }
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

