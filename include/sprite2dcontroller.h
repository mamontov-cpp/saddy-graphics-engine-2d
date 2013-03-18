/*! \file sprite2dcontroller.h
	\author HiddenSeeker

	Describes a 2d sprite controller, that handles sprite size changing event, allowing to check it
 */
#include "sprite2dadapter.h"
#pragma once

/*! \class Sprite2DController
	A 2D sprite controller, that allows some handling, whether sprite size is changed
 */
class Sprite2DController
{
 private:
	     Sprite2DAdapter * m_sprite;       //!< A real sprite in scene
		 bool              m_sizechanged;  //!< Whether size of sprite, has been changed
 public:
		  /*! Creates a new sprite adapter with some properties, like ( Texture coordinates are ABSOLUTE)
			 By default object DOES NOT own sprite. That's because if we removing sprite from scene, memory
		     will be freed automatically
			   \param[in] tex     texture
			   \param[in] texrect texture coordinates rectangle
			   \param[in] bbox    bounding box of sprite
		   */ 
		  Sprite2DController(sad::Texture * tex,const hRectF & texrect,const hRectF & bbox);
		  /*! Destroys a controller
		   */
		  ~Sprite2DController();
		  /*! Determines, whether size is changed. It's not handles ::setSize calls, only changing by ::setTop
			 due to changing size from config
			 \return flag, whether size was changed.
		   */
		  bool wasSizeChanged() const;
		  /*! Sets a position of middle of sprite to specified point
			  \param[in] p point of middle
		   */
		  void setPos(const hPointF & p);
		  /*! Moves a sprite by specified vector
			  \param[in] p vector
		   */
		  void move(const hPointF & p);
		  /*! Returns a middle point of sprite
			  \return a middle point of sprite
		   */
		  hPointF pos() const;
		  /*! Rotates a sprite around it's center clockwise
			  \param[in] angle angle parameter
		   */
		  void rotate(float angle);
		  /*! Returns a rotation angle for a sprite
			  \return angle
		   */
		  float angle() const;
		  /*! Flips image on x axis
		   */
		  void flipX();
		  /*! Flips image on y axis
		   */
		  void flipY();
		  /*! Determines, whether image is flipped on X axis
		   */  
		  bool isFlippedX() const;
		  /*! Determines, whether image is flipped on Y axis
		   */  
		  bool isFlippedY() const;
		  /*! Sets a left side of sprite. May change right side, if width becomes negative
			  \param[in] x left side of sprite
		   */
		  void setLeft(float x);
		  /*! Sets a right side of sprite. May change left side, if width becomes negative
			  \param[in] x right side of sprite
		   */
		  void setRight(float x);
		  /*! Sets a top side of sprite. May change bottom side, if height becomes negative
			  \param[in] y left side of sprite
		   */
		  void setTop(float y);
		  /*! Sets a bottom side of sprite. May change top side, if height becomes negative
			  \param[in] y right side of sprite
		   */
		  void setBottom(float y);
		  /*! Sets a size of sprite, preserving center of image
			  \param[in] size size of sprite
			  \param[in] registerEvent whether we should register size changing event
		   */
		  void setSize(const hPointF & size, bool registerEvent=true);
		  /*! Returns a current size of sprite
			  \return size of sprite
		   */
		  hPointF size() const;
		  /*! Changes a sprite parameters, preserving data  (Texture coordinates are ABSOLUTE )
		      \param[in] tex texture
			  \param[in] texrect texture rectangle
		   */
		 void setSprite(sad::Texture * tex, const hRectF & texrect);
		 /*! Returns a top coordinate
			  \return top coordinate
		   */
		  inline float top()  const { return m_sprite->top();}
		  /*! Returns a bottom coordinate
			  \return bottom coordinate
		   */
		  inline float bottom()  const { return m_sprite->bottom();}
		  /*! Returns a left coordinate
			  \return left coordinate
		   */
		  inline float left()  const { return m_sprite->left();}
		  /*! Returns a right coordinate
			  \return right coordinate
		   */
		  inline float right()  const { return m_sprite->right();}
          /*! Returns adapter from sprite
              \return adapter
           */ 
          inline Sprite2DAdapter * adapter() const { return m_sprite; }
};

