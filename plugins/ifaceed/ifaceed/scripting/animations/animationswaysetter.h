/*! \file animationswaysetter.h
    \author HiddenSeeker

    A basic setter for setting way
 */
#pragma once
#include "animationssetter.h"

#include "../../history/animations/animationschangewaymovingway.h"

#include <animations/animationswaymoving.h>

namespace scripting
{

namespace animations
{

class WaySetter: public scripting::animations::Setter<sad::animations::WayMoving, unsigned long long, history::animations::ChangeWayMovingWay>
{
public:
/*! A converter for converting to way from string
 */
class WayFromString: public scripting::AbstractToValue<unsigned long long>
{
public:
	/*! Created new converter
	 */
	WayFromString();
	/*! Could be inherited
	 */
	virtual ~WayFromString();
	/*! Converts a way to string, using string data
	 */
	sad::Maybe<unsigned long long> toValue(const QScriptValue& v);
};
/*! A condition, which is tests, whether this is unsigned long long is either null or a way
 */
class IsAWay: public scripting::AbstractCondition<unsigned long long>
{
public:
	/*! Creates new condition
	 */
	IsAWay();
	/*! Could be inherited
	 */
	virtual ~IsAWay();
	/*! Checks a condition for object
		\param[in] a an object
		\return if doesn't match -  a message
	 */
	virtual sad::Maybe<QString> check(const unsigned long long& a);
};
    /*! Construct new setter for property
        \param[in] e engine
     */
    WaySetter(
        QScriptEngine* e
    );
    /*! Could be inherited
     */
    virtual ~WaySetter();
};

}

}
