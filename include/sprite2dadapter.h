/*! \file sprite2dadapter.h
	\author HiddenSeeker

	Describes a 2d sprite adapter, that creates a 2d sprite
 */
#include <scene.h>
#include <sprite.h>
#pragma once

/*! \class Sprite2DAdapter
	A class of 2d adapter for 3d sprite. Supports most of sprite operations.
 */
class Sprite2DAdapter: public sad::BasicNode
{
 SAD_NODE
 private:
		  Sprite * m_sprite;  //!< Sprite, that is being rendered
		  hRectF   m_rect;    //!< A bounding rectangle of sprite (non-rotated or flipped)
		  bool     m_flipx;   //!< A flag, that determines, whether image is flipped on X axis
		  bool     m_flipy;   //!< A flag, that determines, whether image is flipped on Y axis
		  float    m_angle;   //!< An angle of rotation of sprite. Sprite is rotated around it's center
 public:
	      /*! Creates a new sprite adapter with some properties, like
			  \param[in] tex     texture
			  \param[in] texrect texture coordinates rectangle
			  \param[in] bbox    bounding box of sprite
		   */ 
		  Sprite2DAdapter(sad::Texture * tex,const hRectF & texrect,const hRectF & bbox);
		  /*! Frees a memory from sprite
		   */
		  ~Sprite2DAdapter();
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
		  /*! Changes a sprite parameters, preserving data
		      \param[in] tex texture
			  \param[in] texrect texture rectangle
		   */
		  void setSprite(sad::Texture * tex, const hRectF & texrect);
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
		   */
		  void setSize(const hPointF & size);
		  /*! Returns a current size of sprite
			  \return size of sprite
		   */
		  hPointF size() const;
		  /*! Renders a sprite
		   */
		  virtual void render();
};


/*! A 2d sprite is some adapter for actual 3d sprite.
 */
typedef Sprite2DAdapter Sprite2D;