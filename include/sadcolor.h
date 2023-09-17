/*! \file   sadcolor.h
    

    \brief  Color class definition.

    This file contains a color type definitions.
*/
#pragma once

namespace sad
{

typedef unsigned char uchar;

/*! Defines a class of difference between two colors
 */
struct ColorDiff
{
    double R; //!< A red component
    double G; //!< A green component
    double B; //!< A blue component

    /*! Multiplies a difference by value
        \param[in] a argument
     */
    inline ColorDiff operator*(double a) const
    {
        ColorDiff result;
        result.R = this->R * a;
        result.G = this->G * a;
        result.B = this->B * a;
        return result;
    }
};
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
     /*! Parametric constructor of following color
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
    */
    inline void setR(sad::uchar r) 
    { 
        m_r = r; 
    }
    /*! Sets the green component
        \param[in] g green component
    */
    inline void setG(sad::uchar g)
    {
        m_g = g;
    }
    /*! Sets the blue component
        \param[in] b blue component
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
    /*! Returns difference between two colors
        \param[in] o other color
     */
    inline sad::ColorDiff operator-(const sad::Color & o) const
    {
        sad::ColorDiff result;
        result.R =  static_cast<double>(this->r()) - static_cast<double>(o.r());
        result.G =  static_cast<double>(this->g()) - static_cast<double>(o.g());
        result.B =  static_cast<double>(this->b()) - static_cast<double>(o.b());
        
        return result;
    }
    /*! Returns sum of color and difference
        \param[in] o difference
        \return color
     */
    inline sad::Color operator+(const sad::ColorDiff& o) const
    {
        sad::Color result;
        result.m_r = static_cast<unsigned char>(static_cast<double>(this->m_r) + o.R);
        result.m_g = static_cast<unsigned char>(static_cast<double>(this->m_g) + o.G);
        result.m_b = static_cast<unsigned char>(static_cast<double>(this->m_b) + o.B);
        return result;
    }
};

/*! Defines a class of difference between two alpha colors
 */
 struct AColorDiff: public sad::ColorDiff
{
    double A; //!< An  alpha componenet

    /*! Multiplies a difference by value
        \param[in] a argument
     */
    inline AColorDiff operator*(double a) const
    {
        AColorDiff result;
        result.R = this->R * a;
        result.G = this->G * a;
        result.B = this->B * a;
        result.A = this->A * a;
        return result;
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
    /*! Constructs new color with alpha channel with following parameters
        \param[in] r red component
        \param[in] g green component
        \param[in] b blue component
        \param[in] a alpha component
     */
    inline AColor(sad::uchar r, sad::uchar g, sad::uchar b, sad::uchar a)
    : sad::Color(r, g, b), m_a(a)
    {
    }
    /*! Constructs new color with alpha channel with following parameters
        \param[in] r red component
        \param[in] g green component
        \param[in] b blue component
     */
    inline AColor(sad::uchar r, sad::uchar g, sad::uchar b)
    : sad::Color(r, g, b), m_a(0)
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
    /*! Returns difference between two colors
        \param[in] o other color
     */
    inline sad::AColorDiff operator-(const sad::AColor & o) const
    {
        sad::AColorDiff result;
        result.R =  static_cast<double>(this->r()) - static_cast<double>(o.r());
        result.G =  static_cast<double>(this->g()) - static_cast<double>(o.g());
        result.B =  static_cast<double>(this->b()) - static_cast<double>(o.b());
        result.A =  static_cast<double>(this->a()) - static_cast<double>(o.a());
        
        return result;
    }
    /*! Returns sum of color and difference
        \param[in] o difference
        \return color
     */
    inline sad::AColor operator+(const sad::AColorDiff& o) const
    {
        sad::AColor result;
        result.m_r = static_cast<unsigned char>(static_cast<double>(this->m_r) + o.R);
        result.m_g = static_cast<unsigned char>(static_cast<double>(this->m_g) + o.G);
        result.m_b = static_cast<unsigned char>(static_cast<double>(this->m_b) + o.B);
        result.m_a = static_cast<unsigned char>(static_cast<double>(this->m_a) + o.A);
        return result;
    }
};

}
