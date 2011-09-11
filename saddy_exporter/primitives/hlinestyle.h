/*! \file hlinestyle.h
    \brief defines a style of line/polyline
	
	Defines a polyline style
*/
#include "hcolor.h"
#pragma once

namespace hst
{

/*! \enum linetype
    Type of connecting line
*/
enum linetype
{
	Simple,  //!< Simple line
	Arrow,   //!< Arrow line
	Anchor,  //!< Line with two big ellipses at ends
	Dashed   //!< Dashed line
};

/*! \class linestyle
    \brief a style of line or polyline

	This class defines a 
	style of line, polyline, defined by width, type and color
*/
class  linestyle
{
 private:
	     acolor   m_color;   //!< Defines a color of line
		 float    m_width;   //!< Defines a width of line
		 linetype m_type;    //!< Defines a type of line
 public:
	    /*! Default constructor
		*/
	    linestyle();
		/*! Copies linestyle
		    \param[in] o other linestyle
		*/
		linestyle(const linestyle & o);
		/*! Constructs linestyle
		    \param[in] type type of line
			\param[in] width width of line
			\param[in] color color of line
		*/
		linestyle(linetype type,float width, const acolor & color);
		/*! Default destructor
		*/
		~linestyle();
		/*! Copies linestyle
		    \param[in] o other linestyle
			\return self-reference
		*/
		linestyle & operator=(const linestyle & o);
		
		/*! Returns width
		    \return width
		*/
		inline float width() const;
		/*! Returns type
		    \return type
		*/
		inline linetype type() const;
		/*! Returns color
		    \return color
		*/
		inline const acolor & color() const;

		/*! Sets a width
		    \param[in] w width
		*/
		inline void setWidth(float w);
		/*! Sets a type
		    \param[in] t type
		*/
		inline void setType( linetype t);
		/*! Sets a color
		    \param[in] c color
		*/
		inline void setColor( const acolor & c);
		/*! Simple comparator
		    \param[in] o other style
			\return true if equal
		*/
		bool operator==(const linestyle & o) const;
};

//Here comes source code for inlined functions
//==============================================
inline float linestyle::width() const {return m_width;}
inline linetype linestyle::type() const { return m_type;}
inline const acolor & linestyle::color() const {return m_color;}

inline void linestyle::setWidth(float w) {m_width=w;}
inline void linestyle::setType(linetype t) {m_type=t;}
inline void linestyle::setColor(const acolor & c) {m_color=c;}
//==============================================

}

/*! \fn typedef hst::linetype hLineType;
    Convenience typedef
*/
typedef hst::linetype hLineType;
/*! \fn typedef hst::linestyle hLineStyle;
    Conveniencte typedef
*/
typedef hst::linestyle hLineStyle;