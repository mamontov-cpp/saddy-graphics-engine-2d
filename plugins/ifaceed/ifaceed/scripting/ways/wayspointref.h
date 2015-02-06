/*! \file wayspointref.h
	\author HiddenSeeker

	A wrapper for reference to way point
 */
#pragma once
#include "../classwrapper.h"

#include <sadpoint.h>

#include <p2d/app/way.h>

namespace scripting
{

namespace ways
{

/*! A wrapper to point reference
 */
class PointRef: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(double x READ x WRITE setX)
 Q_PROPERTY(double y READ y WRITE setY)
public:
	/*! Constructs new wrapper
	 */
	PointRef();
	/*! Constructs new wrapper
		\param[in] way a way, whose point is referenced
		\param[in] pos a position
	 */
	PointRef(sad::p2d::app::Way* way, unsigned int pos);
	/*! Could be inherited
	 */
	virtual ~PointRef();
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
	sad::p2d::app::Way* m_way;
	/*! A position of reference in list
	 */
	unsigned int m_pos;
};

}

}
