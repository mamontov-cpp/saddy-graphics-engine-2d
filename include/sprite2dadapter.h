/*! \file sprite2dadapter.h
	\author HiddenSeeker

	Describes a 2d sprite adapter, that creates a 2d sprite
 */
#include <scene.h>
#include <sprite3d.h>
#include <maybe.h>
#pragma once

/*! \class Sprite2DAdapter
	A class of 2d adapter for 3d sprite. Supports most of sprite operations.
 */
class Sprite2DAdapter: public sad::SceneNode
{
 SAD_OBJECT
 public:
	/*! Options for creating adapter
	 */
	class Options
	{
	  public:
		  sad::String Texture;      //!<  A texture name for current sprite
		  sad::Maybe<sad::String> TextureContainer; //!< A texture container if needed
		  sad::Rect2D TextureRectangle;  //!<  A texture coordinate rectangle
		  sad::Rect2D Rectangle;         //!<  A main rectangle

		  inline Options() {}
		  inline Options(const Sprite2DAdapter::Options & o) : Texture(o.Texture),
		  TextureContainer(o.TextureContainer), TextureRectangle(o.TextureRectangle),
		  Rectangle(o.Rectangle)
		  {
		  }
	};
private:
		  sad::Sprite3D * m_sprite;  //!< Sprite, that is being rendered
		  sad::Rect2D     m_buffer;
public:
	      /*! Creates a new sprite adapter with some properties, like ( Texture coordinates are ABSOLUTE)
			  \param[in] tex     texture
			  \param[in] texrect texture coordinates rectangle
			  \param[in] bbox    bounding box of sprite
		   */ 
		  Sprite2DAdapter(sad::Texture * tex,const sad::Rect2D & texrect,const sad::Rect2D & bbox);
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
		  void setPos(const sad::Point2D & p);
		  /*! Moves a sprite by specified vector
			  \param[in] p vector
		   */
		  void move(const sad::Point2D & p);
		  /*! Returns a middle point of sprite
			  \return a middle point of sprite
		   */
		  sad::Point2D pos() const;
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
		  /*! Sets a size of sprite, preserving center of image
			  \param[in] size size of sprite
		   */
		  void setSize(const sad::Point2D & size);
		  /*! Returns a current size of sprite
			  \return size of sprite
		   */
		  sad::Point2D size() const;
		  /*! Renders a sprite
		   */
		  virtual void render();
		  /*! Changes a sprite parameters, preserving data  (Texture coordinates are ABSOLUTE )
		      \param[in] tex texture
			  \param[in] texrect texture rectangle
		   */
		 void setSprite(sad::Texture * tex, const sad::Rect2D & texrect);
		 /*! Returns a rectangle
			 \return rectangle
          */
		 const sad::Rect2D & rect() const;
		 /*! Sets a rectangle
			 \param[in] rect rectangle
		  */
		 void setRect(const sad::Rect2D & rect);
		 /*! Sets a color
			 \param[in] color color data
		  */
		 void setColor(const sad::AColor & color);
		 /*! Returns a color
			 \return color data
		  */
		 const sad::AColor & color() const;
	     /*! Sets sprite parameters from options
			 \param[in] o options
		  */
		 void set(const Sprite2DAdapter::Options & o);
		 /*! Makes centers span between two points
			 \param[in] r source rectangle for adapter
			 \param[in] p1 first point
			 \param[in] p2 second point
		  */
		 void makeSpanBetweenPoints(const sad::Rect2D & r, const sad::Point2D & p1, const sad::Point2D & p2);
};


/*! A 2d sprite is some adapter for actual 3d sprite.
 */
typedef Sprite2DAdapter Sprite2D;
