/*! \file acolor.h
	\author HiddenSeeker

	A sad::AColor scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadcolor.h>

namespace scripting
{

/*! A sad::Color scriptable wrapper
 */
class AColor: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(unsigned int r READ r WRITE setR)
 Q_PROPERTY(unsigned int g READ g WRITE setG)
 Q_PROPERTY(unsigned int b READ b WRITE setB)
 Q_PROPERTY(unsigned int a READ a WRITE setA)
public:
	/*! Constructs new wrapper
	 */
	AColor();
	/*! Constructs new wrapper
        \param[in] p color
	 */
	AColor(const sad::AColor& p);
	/*! Could be inherited
	 */
	virtual ~AColor();
	/*! Converts a color to original color
		\return color
	 */
	const sad::AColor& toColor() const;
	/*! Returns red component
		\return red component
	 */
	unsigned int r() const;
	/*! Returns green component
		\return green component
	 */
	unsigned int g() const;
	/*! Returns blue component
		\return blue component
	 */
	unsigned int b() const;
	/*! Returns alpha component
		\return alpha component
	 */
	unsigned int a() const;
	/*! Sets red component
		\param[in] r red component
	 */ 
	void setR(unsigned int r);
	/*! Sets green component
		\param[in] g green component
	 */ 
	void setG(unsigned int g);
	/*! Sets blue component
		\param[in] b blue component
	 */ 
	void setB(unsigned int b);
	/*! Sets alpha component
		\param[in] a alpha component
	 */ 
	void setA(unsigned int a);
	/*! Contains to string
		\return object to string
	 */
	QString toString() const;
protected:
	/*! An inner color
	 */
	sad::AColor m_color;
};

}
