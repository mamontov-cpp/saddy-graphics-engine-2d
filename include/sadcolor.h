/*! \file   hcolor.h
    \author HiddenSeeker

    \brief  Color class definition.

	This file contains a color type definitions.
*/
#pragma once

namespace sad
{

typedef unsigned char uchar;
/*! \class Color
    \brief color class

	This class describes a class, which represents color
*/
class Color
{
protected:
	 sad::uchar m_r;   					//!< Red component of color. The bigger it is, the bigger brightness.
	 sad::uchar m_g;   					//!< Green component of color. The bigger it is, the bigger brightness.
	 sad::uchar m_b;   					//!< Blue component of color. The bigger it is, the bigger brightness.
public:
	/* Default constructor of color (0,0,0)
	 */
	inline Color(): m_r(0), m_g(0), m_b(0)
	{
	}
	 /*! Parametric constuctor of following color
	     \param[in] r red component
		 \param[in] g green component
		 \param[in] b blue component
	 */
	inline Color(sad::uchar r,sad::uchar g,sad::uchar b) : m_r(r), m_g(g), m_b(b)
	{

	}
	/*! Red component
	    \return red component
    */
	inline sad::uchar r() const 
	{ 
		return m_r; 
	}
    /*! Green component
	    \return green component
    */
	inline sad::uchar g() const 
	{ 
		return m_g; 
	}
    /*! Blue component
	    \return blue component
    */
	inline sad::uchar b() const 
	{ 
		return m_b; 
	}
    /*! Sets the red component
        \param[in] r red component
        \return self-reference
    */
	inline void setR(sad::uchar r) 
	{ 
		m_r = r; 
	}
    /*! Sets the green component
        \param[in] g green component
        \return self-reference
    */
	inline void setG(sad::uchar g)
	{
		m_g = g;
    }
    /*! Sets the blue component
        \param[in] b blue component
        \return self-reference
    */
	inline void setB(sad::uchar b)
	{
		m_b = b;
	}
	/*! Compares two colors
	    \param[in] o color
		\return  true, if equal
	 */
	inline  bool operator==(const sad::Color & o) const
	{
		return m_r == o.m_r && m_g == o.m_g && m_b == o.m_b;
	}
};
/*! \class AColor
    \brief Determines a color with alpha channel

	This class determines a color with alpha channel
*/
class AColor: public Color
{
 private:
	/*! Determines an alpha channel parameter. 
	    The bigger it is, the more color transparent
	 */
	sad::uchar m_a;  
 public:
	/*! Default constructor of non-transparent color with (0,0,0) rgb parameters
	 */
	inline AColor() : sad::Color(), m_a(0)
	{
	}
	/*! Constructs new color with alphachannel with following parameters
		\param[in] r red component
		\param[in] g green component
		\param[in] b blue component
		\param[in] a alpha component
	 */
	inline AColor(sad::uchar r, sad::uchar g, sad::uchar b, sad::uchar a)
	: sad::Color(r, g, b), m_a(a)
	{
	}
	/*! Sets an alpha-channel parameter, which describes transparency
		\param[in] a new value
     */
	inline void setA(sad::uchar a)
	{
		m_a = a;
	}
	/*! Returns an alpha-channel parameter, which describes transparency
		\return transparency
	 */
	inline sad::uchar a() const
	{
		return m_a;
	}
    /*! Compares two colors for equality
		\param[in] o other color
	  */
	bool operator==(const sad::AColor & o) const
	{
		return this->sad::Color::operator==(o) && m_a == o.m_a;
	}
};

}
