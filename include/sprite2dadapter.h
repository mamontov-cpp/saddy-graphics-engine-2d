/*! \file sprite2dadapter.h
	\author HiddenSeeker

	Describes a 2d sprite adapter, that creates a 2d sprite
 */
#include <scene.h>
#include <sprite.h>
#include <templates/maybe.hpp>
#pragma once

/*! \class Sprite2DAdapter
	A class of 2d adapter for 3d sprite. Supports most of sprite operations.
 */
class Sprite2DAdapter: public sad::BasicNode
{
 SAD_OBJECT
 public:
	/*! Options for creating adapter
	 */
	class Options
	{
	  public:
		  hst::string Texture;      //!<  A texture name for current sprite
		  hst::Maybe<hst::string> TextureContainer; //!< A texture container if needed
		  hRectF TextureRectangle;  //!<  A texture coordinate rectangle
		  hRectF Rectangle;         //!<  A main rectangle

		  inline Options() {}
		  inline Options(const Sprite2DAdapter::Options & o) : Texture(o.Texture),
		  TextureContainer(o.TextureContainer), TextureRectangle(o.TextureRectangle),
		  Rectangle(o.Rectangle)
		  {
		  }
	};
 private:
		  Sprite * m_sprite;  //!< Sprite, that is being rendered
		  hRectF   m_rect;    //!< A bounding rectangle of sprite (non-rotated or flipped)
		  hRectF   m_texrect; //!< A non-rotated non-flipped texture coordinates
		  bool     m_flipx;   //!< A flag, that determines, whether image is flipped on X axis
		  bool     m_flipy;   //!< A flag, that determines, whether image is flipped on Y axis
		  float    m_angle;   //!< An angle of rotation of sprite. Sprite is rotated around it's center
		  hst::acolor m_color; //!< Blending color
		  /*! Creates a new non-rotated non-swapped sprite and sets it
			  Texture coordinates are ABSOLUTE, if normalize flag is set to true
			  \param[in] tex       texture
			  \param[in] texrect   texture rectangle
			  \param[in] bbox      bounding box
			  \param[in] normalize normalization flag
		   */
		  void createSprite(sad::Texture * tex,const hRectF & texrect,const hRectF & bbox, bool normalize=true);
		  /*! Updates bounding rect, preserving all properties
		   */
		  void updateBoundingRect();
		  /*! Flips bounding rect on X axis, changing properties to correct 
		   */
		  void flipBoundingRectX();
		  /*! Flips bounding rect on Y axis, changing properties to correct 
		   */
		  void flipBoundingRectY();
		  /*! Swaps two points in inner sprite texture rectangle
			  \param[in] p1 index of first point
			  \param[in] p2 index of second point
		   */
		  void swapPoints(int p1, int p2);
 public:
	      /*! Creates a new sprite adapter with some properties, like ( Texture coordinates are ABSOLUTE)
			  \param[in] tex     texture
			  \param[in] texrect texture coordinates rectangle
			  \param[in] bbox    bounding box of sprite
		   */ 
		  Sprite2DAdapter(sad::Texture * tex,const hRectF & texrect,const hRectF & bbox);
		  /*! Creates a new sprite, as copy of other
			  \param[in] sprite sprite data
		   */
		  Sprite2DAdapter(const Sprite2DAdapter & sprite);
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
		  /*! Returns a top coordinate
			  \return top coordinate
		   */
		  inline float top()  const { return (float)(pos().y()+size().y()/2);}
		  /*! Returns a bottom coordinate
			  \return bottom coordinate
		   */
		  inline float bottom()  const { return (float)(pos().y()-size().y()/2);}
		  /*! Returns a left coordinate
			  \return left coordinate
		   */
		  inline float left()  const { return (float)(pos().x()-size().x()/2);}
		  /*! Returns a right coordinate
			  \return right coordinate
		   */
		  inline float right()  const { return (float)(pos().x()+size().x()/2);}
		  /*! Renders a sprite
		   */
		  virtual void render();
		  /*! Changes a sprite parameters, preserving data  (Texture coordinates are ABSOLUTE )
		      \param[in] tex texture
			  \param[in] texrect texture rectangle
		   */
		 void setSprite(sad::Texture * tex, const hRectF & texrect);
		 /*! Returns a rectangle
			 \return rectangle
          */
		 const hRectF & rect() const;
		 /*! Sets a rectangle
			 \param[in] rect rectangle
		  */
		 void setRect(const hRectF & rect);
		 /*! Sets a color
			 \param[in] color color data
		  */
		 void setColor(const hst::acolor & color);
		 /*! Returns a color
			 \return color data
		  */
		 const hst::acolor & color() const;
	     /*! Sets sprite parameters from options
			 \param[in] o options
		  */
		 void set(const Sprite2DAdapter::Options & o);
		 /*! Makes centers span between two points
			 \param[in] r source rectangle for adapter
			 \param[in] p1 first point
			 \param[in] p2 second point
		  */
		 void makeSpanBetweenPoints(const hRectF & r, const hPointF & p1, const hPointF & p2);
};


/*! A 2d sprite is some adapter for actual 3d sprite.
 */
typedef Sprite2DAdapter Sprite2D;
