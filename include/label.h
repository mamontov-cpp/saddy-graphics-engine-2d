/*! \file   label.h
    \author HiddenSeeker

	Contains a definition of simple label node, that can be added to scene
*/
#include "scene.h"
#include "fontmanager.h"
#pragma once

/*! \class Label
    Class of simple label on scene
*/
class Label: public sad::BasicNode
{
 SAD_NODE
 private:
	    sad::BasicFont * m_fnt;    //!<  Associated font
		hst::string      m_str;    //!<  String
		hRectF           m_rect;   //!<  Bounding rectangle
		float            m_z;      //!<  Z component
 public:
		/*! Returns a font reference
		    \return font 
		*/
		inline sad::BasicFont *& font();
		/*! Returns a string
		    \return string
		*/
		inline hst::string &  string();
		/*! Returns a bounding box position
		    \return m_rect
		*/
		inline hRectF & box();
		/*! Returns a  Z component
		    \return m_z
		*/
		inline float z();
		/*! Creates a simple node
		    \param[in] fnt  font object
			\param[in] str  rendered string
			\param[in] rect bounding rectangle
			\param[in] z    Z component
		*/
		Label(
		      sad::BasicFont * fnt,
		      const hst::string & str,
			  const hRectF      & rect,
			  float                z
			 );
        /*! Renders it
		*/
		virtual void render();
		/*! Destructs it
		*/
		virtual ~Label();
};

//======================================Source code=====================================
sad::BasicFont *& Label::font()   { return m_fnt;  }
hst::string &     Label::string() { return m_str;  }
hRectF &          Label::box()    { return m_rect; }
float             Label::z()      { return m_z;    }
