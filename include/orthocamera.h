/*! \file   orthocamera.h
    \author HiddenSeeker
    \brief  Here is placed a camera, used for orthographic proection
*/
#include "renderer.h"
#include "scene.h"
#pragma once

/*! \class OrthoCamera
	A class of camera with orthographic proection
*/
class OrthoCamera: public sad::Camera
{
 private:
	     bool m_first_apply;    //!< Whether is apply is first on game
		 int  m_width;		    //!< Width of viewport
		 int  m_height;         //!< Height of viewport
		 bool m_set_renderfix;  //!< Render fixing
		 void renderFix();      //!< Applies render fixing
		 sad::Renderer * m_renderer; //!< Renderer which it works with
 public:
	     /*! Creates a camera, using windows settings from renderer.
			 Created camera maps one in one coordinates
			 \param[in] fix performs implementation specific fix. Use, if some coordinates a calculated wrongly
			 \param[in] r renderer which it works with
		 */
	     OrthoCamera(bool fix=false, sad::Renderer * r = sad::Renderer::ref());
		 /*! Creates a orthographic camera with specific viewing width and height
			 \param[in]  width of field
			 \param[in]  height of field
			 \param[in]  renderer renderer which it works with
		 */
		 OrthoCamera(int width, int height, sad::Renderer * r = sad::Renderer::ref());
		 /*! Applies a camera
		 */
		 void apply();

		 /*! Destructor
		 */
		 ~OrthoCamera();
};
