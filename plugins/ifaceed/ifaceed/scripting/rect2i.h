/*! \file rect2i.h
	\author HiddenSeeker

	A sad::Rect2I scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadrect.h>

#include "point2i.h"

#include <QScriptValue>

namespace scripting
{

/*! A sad::Point2I scriptable wrapper
 */
class Rect2I: public scripting::ClassWrapper
{
 Q_OBJECT
public:
	/*! Constructs new wrapper
	 */
	Rect2I();
	/*! Constructs new wrapper
		\param[in] p point
	 */
	Rect2I(const sad::Rect2I& p);
	/*! Could be inherited
	 */
	virtual ~Rect2I();
	/*! Converts a point to original point
	 */
	const sad::Rect2I& toRect() const;
	/*! Contains to string
		\return object to string
	 */
	QString toString() const;
public slots:  
	/*! Sets point for rectangle
		\param[in] i index
		\param[in] p point
	 */
	void setPoint(int i, scripting::Point2I* p);
	/*! Returns point from rectangle
		\param[in] i index
	 */
	QScriptValue point(int i) const;
protected:
	/*! An inner rect wrapper
	 */
	sad::Rect2I m_rect;
};

}
