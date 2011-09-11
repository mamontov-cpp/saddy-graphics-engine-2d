/*! \file   hcolor.h
    \author HiddenSeeker

    \brief  Color class definition.

	This file contains a color type definitions.
*/
#pragma once
/*! \fn typedef unsigned char Uint8
 *  This typedef is important for SDL applications. It doesn't contains any other sense
 */
typedef unsigned char Uint8;

namespace hst
{
/*! \class color
    \brief color class

	This class describes a class, which represents color
*/
class color
{
private:
     Uint8 m_r;   						//!< Red component of color. The bigger it is, the bigger brightness.
	 Uint8 m_g;   						//!< Green component of color. The bigger it is, the bigger brightness.
	 Uint8 m_b;   						//!< Blue component of color. The bigger it is, the bigger brightness.
public:
	color();  						//!< Default constructor of color (0,0,0)
	 /*! Parametric constuctor of following color
	     \param[in] r red component
		 \param[in] g green component
		 \param[in] b blue component
	 */
	 color(Uint8 r,Uint8 g,Uint8 b);
	 /*! Copy constructor
	     \param[in] o other color
	 */
	 color(const color & o);
	 /*! Assignment operator
	     \param[in] o other color
	 */
	 color & operator=(const color & o);
	 /*! Destructor operator
	 */
	 ~color();
	 /*! Red component
	     \return red component
     */
     inline Uint8 r() const;
     /*! Green component
	     \return green component
     */
     inline Uint8 g() const;
     /*! Blue component
	     \return blue component
     */
     inline Uint8 b() const;
     /*! Sets the red component
         \param[in] r red component
         \return self-reference
     */
     inline color & setR(Uint8 r);
     /*! Sets the green component
         \param[in] g green component
         \return self-reference
     */
     inline color & setG(Uint8 g);
     /*! Sets the blue component
         \param[in] b blue component
         \return self-reference
     */
     inline color & setB(Uint8 b);
	 /*! Compares two colors
	    \param[in] o color
		\return  true, if equal
	*/
	inline  bool operator==(const hst::color o); 
};
/*! \class acolor
    \brief Determines a color with alpha

	This class determines a color with alpha
*/

class acolor:public color
{
 private:
	     Uint8 m_alpha; //!< Determines an alpha parameter. The bigger it is, the more color transparent
 public:
	     /*! Default constructor of non-transparent color
		 */
	     acolor();
	     /*! Constructor
		     \param[in] r red component
			 \param[in] g green component
			 \param[in] b blue component
			 \param[in] a alpha component
		 */
		 acolor(Uint8 r,Uint8 g,Uint8 b,Uint8 a);
		 /*! Default destructor
		 */
		 ~acolor();
         /*! Copy constructor
		     \param[in] o other color
		 */
		 acolor(const acolor & o);
         /*! Assignment overload
		     \param[in] o other color
		 */
		 acolor & operator=(const acolor & o);
		 /*! Sets a transparency parameter
		     \param[in] a new value
		 */
         inline void setA(Uint8 a);
		 /*! Simple comparator
		    \param[in] o other color
		 */
		 bool operator==(const acolor & o) const;
		 /*! Returns a transparency parameter
		     \return transparency
		 */
		 inline Uint8 a() const;
};


//Here comes a inlined functions source code
//===========================================
inline Uint8 color::r() const {return m_r;}
inline Uint8 color::g() const {return m_g;}
inline Uint8 color::b() const {return m_b;}
bool   color::operator==(const hst::color o) { return m_r==o.m_r && m_g==o.m_g && m_b==o.m_b; }
inline color & color::setR(Uint8 r) {m_r=r; return *this;}
inline color & color::setG(Uint8 g) {m_g=g; return *this;}
inline color & color::setB(Uint8 b) {m_b=b; return *this;}

inline void acolor::setA(Uint8 a) {m_alpha=a;}
inline Uint8 acolor::a() const {return m_alpha;}
//===========================================

}
/*! /fn typedef hst::acolor hAColor
    Convenience typedef
*/
typedef hst::acolor hAColor;
/*! /fn typedef hst::color hColor
    Convenience typedef
*/
typedef hst::color hColor;
