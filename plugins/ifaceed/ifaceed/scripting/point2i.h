/*! \file point2i.h
	

	A sad::Point2I scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadpoint.h>

namespace scripting
{

/*! A sad::Point2I scriptable wrapper
 */
class Point2I: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(int x READ x WRITE setX)
 Q_PROPERTY(int y READ y WRITE setY)
public:
	/*! Constructs new wrapper
	 */
	Point2I();
	/*! Constructs new wrapper
		\param[in] p point
	 */
	Point2I(const sad::Point2I& p);
	/*! Could be inherited
	 */
	virtual ~Point2I();
	/*! Converts a point to original point
	 */
	const sad::Point2I& toPoint() const;
	/*! Returns x coordinate
        \return x coordinate
	 */
	int x() const;
	/*! Returns y coordinate
        \return y coordinate
	 */
	int y() const;
	/*! Sets x coordinate
		\param[in] x x coordinate
	 */ 
	void setX(int x);
	/*! Sets y coordinate
		\param[in] y y coordinate
	 */
	void setY(int y);
	/*! Contains to string
		\return object to string
	 */
	QString toString() const;
protected:
	/*! An inner point wrapper
	 */
	sad::Point2I m_point;
};

}
