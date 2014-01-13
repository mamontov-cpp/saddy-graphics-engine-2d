/*! \file   sprite.h
    \author HiddenSeeker

	Contains a definition of simple 3D Sprite
*/
#pragma once
#include "scene.h"
#include "texture.h"
#include "sadrect.h"
 
/*! Class of simple quad that rotates
*/
class Sprite: public sad::SceneNode
{
 SAD_OBJECT
 private:
	     float        m_tex_coord[8];    //!< Texture coordinates
		 sad::Point3D   m_rect[4];         //!< Rectangle
		 sad::AColor  m_color;           //!< Color of sprite
		 sad::Texture * m_tex;           //!< Associated texture
		 /*! Rotates one point around other point
			 \param[in]   p       rotating point
			 \param[in]   pivot   pivot point
			 \param[in]   alpha   alpha angle of rotation on XY axis
			 \param[in]   theta   theta angle of rotation on YZ axis
		 */
		 void rotate(
		             sad::Point3D & p, 
					 const sad::Point3D & pivot, 
					 double alpha, 
					 double theta 
					);
 public:
	      /*! Creates a sprite with following parameters
			  \param[in] tex   texture
			  \param[in] rect  rectangle for sprite
			  \param[in] texrect rectangle for texture coordinates(in coordinate range, NOT in 0..1)
		  */
		  Sprite(
			     sad::Texture * tex, 
				 const sad::Rect< sad::Point3D> & rect,
				 const sad::Rect2D  & texrect
			    );
          /*! Creates a simple node
		      \param[in] tex        texture
			  \param[in] rect       rectangle, with point directed clockwise
			  \param[in] tex_coord  texture coordinates
		  */
		 Sprite(
		        sad::Texture * tex,
		        sad::Point3D rect[4], 
				float * tex_coord=NULL
			   );
		  /*! Creates a simple node
		      \param[in] tex        texture
			  \param[in] rect       rectangle, with point directed clockwise
			  \param[in] tex_coord  texture coordinates
		  */
		 Sprite(
		        sad::Texture * tex,
		        sad::Point3D rect[4], 
				int * tex_coord
			   );
		 /*! Copies a sprite information
			 \param[in] sprite sprite data
		  */
		 Sprite(const Sprite & sprite);
		  /*! Sets a new texture coordinate
		      \param[in] ind index
			  \param[in] x   x coord
			  \param[in] y   y coord.
		  */
		  void setTexCoords(int ind, float x ,float y);
		  /*! Sets a new texture coordinate
		      \param[in] ind index
			  \param[in] x   x coord
			  \param[in] y   y coord.
		  */
		  void setTexCoords(int ind, int x ,int y);
		  
		  /*! Returns a texture coordinates
		  */
		  const float * texCoords() const;
		  /*! Returns a point reference
		      \param[in] n number of point
			  \return reference to point
		  */
		  sad::Point3D & point(int n);
		  /*! Calculates a middle point
		      \return middle point
		  */
		  sad::Point3D  middle() const;
		  /*! Moves a points by following vector
			  \param[in] p point
		  */
		  void moveBy(const sad::Point3D & p);
		  /*! Moves a sprite center to a point
		      \param[in] p point
		  */
		  void moveTo(const sad::Point3D & p);
		  /*! Rotates a sprite around the pivot in three dimensions
		      \param[in]  pivot pivot point
		      \param[in]  alpha angle of rotation on XY axis
			  \param[in]  theta angle of rotation on YZ axis
		  */
		  void rotate(const sad::Point3D & pivot, double alpha, double theta);
		  /*! Rotates a sprite around his middle point in three dimensions
		      \param[in]  alpha angle of rotation on XY axis
			  \param[in]  theta angle of rotation on YZ axis
		  */
		  void rotate(double alpha, double theta);
		  /*! Renders it
		  */
		  virtual void render();
		  /*! Destructs it
		  */
		  virtual ~Sprite();
		  /*! Sets a texture for sprite
			  \param[in] tex texture
		  */
		  inline void setTexture(sad::Texture * tex) { m_tex=tex; }
		  /*! Returns a bounding box for rectangle
		   */
		  sad::Rect< sad::Point3D > bbox() const;
		  /*! Sets boundary box of rect
			  \param[in] rect rectangle
		   */
		  void setBBox(const sad::Rect<sad::Point3D> & rect);
		  /*! Sets a color for sprite
			  \param[in] clr blending sprite color
		   */
		  void setColor(const sad::AColor & clr);
		  /*! Returns a color for sprite
			  \return blending color of sprite 
		   */
		  const sad::AColor & color() const;
};
