/*! \file   label.h
    \author HiddenSeeker

	Contains a definition of simple label node, that can be added to scene
*/
#include "scene.h"
#include "fontmanager.h"
#include "input.h"
#include "renderer.h"
#pragma once

typedef sad::BasicFont *  LabelFont;
/*! \class Label
    Class of simple label on scene
*/
class Label: public sad::BasicNode, public sad::ResizeEventHandler
{
 SAD_NODE
 private:
	    LabelFont  m_fnt;        //!<  Associated font
		hst::string      m_str;        //!<  String
		pointf           m_p;          //!<  point
		pointf           m_rend_point; //!<  Current rendering point
		sad::Renderer *  m_renderer;  //!< Input data
 public:
	    void operator()(const sad::ResizeEvent & o);
		/*! Returns a font reference
		    \return font 
		*/
		inline LabelFont & font();
		/*! Returns a string
		    \return string
		*/
		inline hst::string &  string();
		/*! Returns a point position
		    \return m_p
		*/
		inline pointf &  point();
		/*! Returns a rendering point position
		    \return m_rend_point
		*/
		inline pointf &  render_point();
		/*! Creates a label
		*/
		Label(sad::Renderer * renderer = sad::Renderer::ref());
		/*! Creates a simple node
		    \param[in] fnt  font object
			\param[in] str  rendered string
			\param[in] p    point
		*/
		Label(
		      LabelFont  fnt,
		      const hst::string & str,
			  const pointf      & p,
			  sad::Renderer * renderer = sad::Renderer::ref()
			 );
        /*! Renders it
		*/
		virtual void render();
		/*! Destructs it
		*/
		virtual ~Label();
};

//======================================Source code=====================================
LabelFont & Label::font()   { return m_fnt;  }
hst::string &     Label::string() { return m_str;  }
pointf &          Label::point()    { return m_p; }
pointf &		  Label::render_point() { return m_rend_point;}
