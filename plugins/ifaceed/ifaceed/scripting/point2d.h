/*! \file point2d.h
	

	A sad::Point2D scriptable wrapper
 */
#pragma once
#include <QMetaType>
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
	/*! A metafunction for converting value
		\param[in] engine an engine
		\param[in] in value
	 */
	static QScriptValue toValue(QScriptEngine* engine, scripting::Point2D* const &in);
	/*! A meta function for converting value from script to object
		\param[in] object object
		\param[out] out out value
	 */
	static void fromValue(const QScriptValue& object, scripting::Point2D*& out);
public slots:
	/*! Returns sum of two scripting points
		\param[in] p point
		\return sum
	 */
	scripting::Point2D* sum(scripting::Point2D* p);
	/*! Returns substraction of two scripting points
		\param[in] p point
		\return substraction result
	 */
	scripting::Point2D* sub(scripting::Point2D* p);
	/*! Returns middle point between two points
		\param[in] p point
		\return middle point
	 */
	scripting::Point2D* mid(scripting::Point2D* p);
	/*! Returns multiplication of two scripting points
		\param[in] d number
		\return multiplication result
	 */
	scripting::Point2D* mul(double d);
	/*! Returns division for points
		\param[in] d number
		\return division result
	 */
	scripting::Point2D* div(double d);
protected:
	/*! An inner point wrapper
	 */
	sad::Point2D m_point;
};

}

Q_DECLARE_METATYPE(scripting::Point2D*)
