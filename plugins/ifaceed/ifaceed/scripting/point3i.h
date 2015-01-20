/*! \file point3i.h
	\author HiddenSeeker

	A sad::Point3I scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadpoint.h>

namespace scripting
{

/*! A sad::Point3I scriptable wrapper
 */
class Point3I: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(int x READ x WRITE setX)
 Q_PROPERTY(int y READ y WRITE setY)
 Q_PROPERTY(int z READ z WRITE setZ)
public:
	/*! Constructs new wrapper
	 */
	Point3I();
	/*! Constructs new wrapper
		\param[in] p point
	 */
	Point3I(const sad::Point3I& p);
	/*! Could be inherited
	 */
	virtual ~Point3I();
	/*! Converts a point to original point
	 */
	const sad::Point3I& toPoint() const;
	/*! Returns x coordinate
		\param[in] x coordinate
	 */
	int x() const;
	/*! Returns y coordinate
		\param[in] y coordinate
	 */
	int y() const;
	/*! Returns z coordinate
		\param[in] z coordinate
	 */
	int z() const;
	/*! Sets x coordinate
		\param[in] x x coordinate
	 */ 
	void setX(int x);
	/*! Sets y coordinate
		\param[in] y y coordinate
	 */
	void setY(int y);
	/*! Sets z coordinate
		\param[in] z z coordinate
	 */
	void setZ(int z);
	/*! Contains to string
		\return object to string
	 */
	QString toString() const;
protected:
	/*! An inner point wrapper
	 */
	sad::Point3I m_point;
};

}
