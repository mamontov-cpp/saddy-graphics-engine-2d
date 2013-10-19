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
	     /*! Creates a camera, with width and height of window, taken from renderer.
			 Created camera maps one in one coordinates
			 \param[in] fix performs implementation specific fix. Use, if some coordinates a calculated wrongly
			 \param[in] r renderer which it works with
		 */
	     OrthoCamera(sad::Renderer * r = sad::Renderer::ref());
		 /*! Creates a orthographic camera with specific viewing width and height
			 \param[in]  width of field
			 \param[in]  height of field
			 \param[in]  r renderer which it works with
		 */
		 OrthoCamera(int width, int height, sad::Renderer * r = sad::Renderer::ref());
		 /*! Applies a camera
		 */
		 void apply();

		 /*! Destructor
		 */
		 ~OrthoCamera();
  private:
		 int  m_width;		    //!< Width of viewport
		 int  m_height;         //!< Height of viewport
};

}
