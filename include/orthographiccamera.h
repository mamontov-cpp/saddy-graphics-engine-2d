/*! \file   orthographiccamera.h
    \author HiddenSeeker
    
	An orthographic camera, which sets of orthographic projection, so all viewed objects will be withing match
	a rectangle with specified width and height
*/
#include <scene.h">
#pragma once

namespace sad
{
/*! \class OrthographicCamera

	An orthographic camera, which sets of orthographic projection, so all viewed objects will be withing match
	a rectangle with specified width and height
 */
class OrthographicCamera: public sad::Camera
{
public:
	     /*! Creates a camera, with rectangle width and height, taken from window settings of renderer.
			 \param[in] r renderer which it works with
		  */
	     OrthoCamera(sad::Renderer * r = sad::Renderer::ref());
		 /*! Creates a camera, width and height of orthographic projections, so all viwed objects will be within
			 width and specified height
			 \param[in]  width of bounding rectangle
			 \param[in]  height of bounding rectangle
		  */
		 OrthoCamera(int width, int height);
		 /*! Applies an orthographic projection matrices, using GLU functions
		  */
		 virtual void apply();
		 /*! Kept for purpose of inheritance
		  */
		 virtual ~OrthoCamera();
  private:
		 int  m_width;		    //!< Width of viewport
		 int  m_height;         //!< Height of viewport
};

}
