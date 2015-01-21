/*! \file point2d.h
	\author HiddenSeeker

	A sad::Point2D scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadpoint.h>

namespace scripting
{

/*! A sad::Point2D scriptable wrapper
 */
class Point2D: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(double x READ x WRITE setX)
 Q_PROPERTY(double y READ y WRITE setY)
public:
	/*! Constructs new wrapper
	 */
	Point2D();
	/*! Constructs new wrapper
		\param[in] p point
	 */
	Point2D(const sad::Point2D& p);
	/*! Could be inherited
	 */
	virtual ~Point2D();
	/*! Converts a point to original point
	 */
	const sad::Point2D& toPoint() const;
	/*! Returns x coordinate
		\return x coordinate
	 */
	double x() const;
	/*! Returns y coordinate
		\return y coordinate
	 */
	double y() const;
	/*! Sets x coordinate
		\param[in] x x coordinate
	 */ 
	void setX(double x);
	/*! Sets y coordinate
		\param[in] y y coordinate
	 */
	void setY(double y);
	/*! Contains to string
		\return object to string
	 */
	QString toString() const;
protected:
	/*! An inner point wrapper
	 */
	sad::Point2D m_point;
};

}
