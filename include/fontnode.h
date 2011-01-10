/*! \file   fontnode.h
    \author HiddenSeeker

	Contains a definition of simple font test node, that can be added to scene
*/
#include "scene.h"
#include "fontmanager.h"
//#define FPS_CHECK //Uncomment this for fps checking
#pragma once

/*! \class TestFontNode
    Class of simple texture mapped fonts demo node.
*/
class TestFontNode: public sad::BasicNode
{
 private:
	    sad::TMFont * m_fnt;    //!<  Associated font
 public:
	    static int Type;        //!< Type
        /*! Creates a simple node
		*/
		TestFontNode(sad::TMFont * fnt);
        /*! Renders it
		*/
		void render();
		/*! Destructs it
		*/
		~TestFontNode();
};
