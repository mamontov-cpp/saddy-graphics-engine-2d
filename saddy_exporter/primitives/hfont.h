/*! \file    hfont.h
    \author  HiddenSeeker
	
	\brief   Declares a font setting.
	
	This file contains a definition of font.
*/
#include "../templates/hstring.h"
#include "../templates/hpair.hpp"
#include "../templates/hlist.hpp"
#include "hcolor.h"
#pragma once

namespace hst
{
/*! \enum fontdesc
    
	Declares an enum of style of font
*/
enum fontdesc
{
	Normal=1,         //!< Normal
	Italic=2,         //!< Italic
	Oblique=4,        //!< Oblique
	Bold=32,          //!< Bold weight
};

/*! \class font
    \brief style of font

	Declares a font style
*/
class font
{
 private: 
	    unsigned int       m_size;          //!< Size of font
		hString            m_fontname;      //!< Name of font
		unsigned int       m_style;         //!< Style of font
		acolor             m_color;         //!< Color of font
 public:
	    /*! Default constructor
		*/
	    font();
		/*! Parametric constructor
		    \param[in] size size of font
			\param[in] font name of font
			\param[in] style style of font
			\param[in] color  color of font
		*/
	    font(int size,const hString & font=hString("serif"),unsigned int style=Normal,const hAColor & color=hAColor(0,0,0,255));
		/*! Copy constructor
		    \param[in] o other object
		*/
		font(const font & o);
		/*! Assignment overload
		    \param[in] o other object
			\return self-reference
		*/
		font & operator=(const font & o);
		/*! Default destructor
		*/
		~font();
		
		/*! Returns a size
		    \return size
	    */
		inline unsigned int size() const;
		/*! Returns a name of font
		    \return name of font
		*/
		inline const hString & fontFamily() const;
		/*! Returns a style of font
		    \return style
		*/
		inline unsigned int style() const;
		/*! Returns a color of font
		    \return color of font
		*/
		inline const acolor & color() const;

		/*! Sets a size of font
		    \param[in] size new size of font
			\return self-reference
		*/
		inline font & setSize( unsigned int size );
		/*! Sets a name for font
		    \param[in] font new font-family
			\return self-reference
		*/
		inline font & setFontFamily( const hString & font );
		/*! Sets a style of font
		    \param[in] style new style of font
			\return self-reference
		*/
		inline font & setStyle( unsigned int style );
		/*! Sets a color of font
		    \param[in] color new color of font
			\return self-reference
		*/
		inline font & setColor( const acolor & color );
};

//Here comes source  code for inlined functions
//===========================================================
inline unsigned int    font::size()        const { return m_size;  }
inline const hString & font::fontFamily()  const { return m_fontname;  }
inline unsigned int    font::style()       const { return m_style; }
inline const hAColor & font::color()       const { return m_color; }

inline font & font::setSize( unsigned int size         )   { m_size=size;  return *this;}
inline font & font::setFontFamily( const hString & font  )   { m_fontname=font;  return *this;}
inline font & font::setStyle( unsigned int  style     )  { m_style=style;return *this;}
inline font & font::setColor( const acolor & color )  { m_color=color;return *this;}
//===========================================================
}

/*! \fn typedef  hst::fontdesc hFontDesc;
    Convenience typedef;
*/
typedef hst::fontdesc hFontDesc;
/*! \fn typedef hst::font hFont;
    Convenience typedef;
*/
typedef hst::font hFont;
/*! typedef hlist<hpair<hString,hString> > MarkedText;
    Type defininition of marked up text, using styles
	In first property is style placed and second is a string
*/
typedef hlist<hpair<hString,hString> > MarkedText;
/*! typedef hlist<hpair<hst::font,hString> > MarkedupText;
    Type defininition of marked up text, without using styles
*/
typedef hlist<hpair<hst::font,hString> > MarkedupText;
/*! typedef hpair<hst::font,hString>  AttrText;
    Type defininition of marked up text, without using styles
*/
typedef hpair<hst::font,hString> AttrText;
/*! typedef hpair<hString,hString> StyledText;
    Type definition for applied style to text
*/
typedef hpair<hString,hString> StyledText;
