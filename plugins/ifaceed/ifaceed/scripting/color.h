/*! \file color.h
	\author HiddenSeeker

	A sad::Color scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadcolor.h>

namespace scripting
{

/*! A sad::Color scriptable wrapper
 */
class Color: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(unsigned int r READ r WRITE setR)
 Q_PROPERTY(unsigned int g READ g WRITE setG)
 Q_PROPERTY(unsigned int b READ b WRITE setB)
public:
	/*! Constructs new wrapper
	 */
	Color();
	/*! Constructs new wrapper
        \param[in] p color
	 */
	Color(const sad::Color& p);
	/*! Could be inherited
	 */
	virtual ~Color();
	/*! Converts a color to original color
		\return color
	 */
	const sad::Color& toColor() const;
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
	/*! Contains to string
		\return object to string
	 */
	QString toString() const;
protected:
	/*! An inner color
	 */
	sad::Color m_color;
};

}
