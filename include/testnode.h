/*! \file   testnode.h
    \author HiddenSeeker

	Contains a definition of simple test node, that can be added to scene
*/
#include "scene.h"
//#define FPS_CHECK //Uncomment this for fps checking
#pragma once

/*! Class of simple quad that rotates constantly on one of the axis
*/
class TestNode: public sad::BasicNode
{
 private:
	     float m_x;   //!< X
		 float m_y;   //!< Y
		 float m_r;   //!< Radius
	     float m_angle;  //!< Angle
 public:
	     static int Type; //!< Статический тип
          /*! Creates a simple node
		  */
	      TestNode(float x, float y, float r);
          /*! Renders it
		  */
		  void render();
		  /*! Destructs it
		  */
		 ~TestNode();
};
