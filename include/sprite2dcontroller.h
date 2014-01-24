/*! \file sprite2dcontroller.h
	\author HiddenSeeker

	Describes a 2d sprite controller, that handles sprite size changing event, allowing to check it
 */
#include "sprite2d.h"
#pragma once

/*! \class Sprite2DController
	A 2D sprite controller, that allows some handling, whether sprite size is changed
 */
class Sprite2DController
{
 private:
		 sad::Sprite2D * m_sprite;       //!< A real sprite in scene
		 bool              m_sizechanged;  //!< Whether size of sprite, has been changed
 public:
		  /*! Creates a new sprite adapter with some properties, like ( Texture coordinates are ABSOLUTE)
			 By default object DOES NOT own sprite. That's because if we removing sprite from scene, memory
		     will be freed automatically
			   \param[in] tex     texture
			   \param[in] texrect texture coordinates rectangle
			   \param[in] bbox    bounding box of sprite
		   */ 
		  Sprite2DController(sad::Texture * tex,const sad::Rect2D & texrect,const sad::Rect2D & bbox);
		  /*! Destroys a controller
		   */
		  ~Sprite2DController();
		  /*! Determines, whether size is changed. It's not handles Sprite2DController::setSize calls, only changing by 
		      Sprite2DController::setTop
			  due to changing size from config
			 \return flag, whether size was changed.
		   */
		  bool wasSizeChanged() const;
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
			  \param[in] registerEvent whether we should register size changing event
		   */
		  void setSize(const sad::Point2D & size, bool registerEvent=true);
		  /*! Returns a current size of sprite
			  \return size of sprite
		   */
		  sad::Point2D size() const;
		  /*! Changes a sprite parameters, preserving data  (Texture coordinates are ABSOLUTE )
		      \param[in] tex texture
			  \param[in] texrect texture rectangle
		   */
		 void setSprite(sad::Texture * tex, const sad::Rect2D & texrect);
          /*! Returns adapter from sprite
              \return adapter
           */ 
		  inline sad::Sprite2D * adapter() const { return m_sprite; }
		  /*! Sets a color for sprite
		      \param[in] clr color
		   */
		  void setColor(const sad::AColor & clr);
		  /*! Returns a color of sprite
		   */
		  sad::AColor color() const; 
};

