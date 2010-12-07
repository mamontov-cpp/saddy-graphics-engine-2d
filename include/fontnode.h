/*! \file   fontnode.h
    \author HiddenSeeker

	Contains a definition of simple font test node, that can be added to scene
*/
#include "scene.h"
#include "fontmanager.h"
//#define FPS_CHECK //Uncomment this for fps checking
#pragma once

class TestFontNode: public sad::BasicNode
{
 private:
	    sad::TMFont * m_fnt;
 public:
	    static int Type; //!< Статический тип
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
