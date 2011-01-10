/*! \file   testnode.h
    \author HiddenSeeker

	Contains a definition of simple test node, that can be added to scene
*/
#include "scene.h"
#include "texture.h"
//#define FPS_CHECK //Uncomment this for fps checking
#pragma once

/*! Class of simple quad that rotates constantly on one of the axis
*/
class TestNode: public sad::BasicNode
{
 private:
	     float m_x;             //!< X
		 float m_y;             //!< Y
		 float m_r;             //!< Radius
	     float m_angle;         //!< Angle
		 sad::Texture * m_tex;  //!< Associated texture
 public:
	     static int Type;       //!< Static type
          /*! Creates a simple node
		      \param[in] tex texture
			  \param[in] x  X position
			  \param[in] y  Y position
			  \param[in] r  radius
		  */
		 TestNode(sad::Texture * tex,float x, float y, float r);
          /*! Renders it
		  */
		  void render();
		  /*! Destructs it
		  */
		 ~TestNode();
};
