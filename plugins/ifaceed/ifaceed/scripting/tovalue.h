/*! \file tovalue.h
 *  
 *
 *  Contains custom conversions from QScriptValue
 */
#pragma once
#include <QString>
#include <QVector>
#include <QScriptValue>
#include <maybe.h>
#include <sadcolor.h>
#include <sadrect.h>
#include <sadsize.h>
#include <sadstring.h>

#include <p2d/app/way.h>

#include "abstracttovalue.h"

namespace sad
{
class Scene;
class SceneNode;
class Label;
class Sprite2D;


namespace db
{
class Object;

namespace custom
{
class Object;	
}

}


namespace dialogue
{
class Dialogue;
class Phrase;
}

namespace animations
{
	
class Animation;
class Composite;
class Blinking;
class CameraRotation;
class CameraShaking;
class Color;
class FontList;
class FontSize;
class OptionList;
class Parallel;
class Resize;
class Rotate;
class SimpleMovement;
class Sequential;
class TextureCoordinatesList;
class TextureCoordinatesContinuous;
class Typing;
class WayMoving;

class Instance;
class WayInstance;
class Group;

}

}

namespace scripting
{
/*! A class, for performing safe conversion of script value to type
 */
template<
    typename T
>
class ToValue: public scripting::AbstractToValue<T>
{
public:
	/*! Converts a value to value
	 */
	inline ToValue()
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ToValue()
	{
		
	}
	/*! Converts a resulting value to a value
		\param[in] v value
		\return result
	 */
	virtual sad::Maybe<T> toValue(const QScriptValue& v)
	{
		return perform(v);
	}
    /*! Tries to perform conversion to specified type
     *  \return value, if it exists
     */
    inline static sad::Maybe<T> perform(const QScriptValue&)
    {
	    return sad::Maybe<T>();
    }
};

/*! Defines specialization of scripting::ToValue for specified type
 */
#define DEFINE_TO_VALUE_FOR_TYPE(A)                            \
template<                                                        \
>                                                                \
class ToValue< A >: public scripting::AbstractToValue< A >       \
{                                                                \
public:                                                          \
	ToValue();                                                   \
	~ToValue();                                                  \
	sad::Maybe< A > toValue(const QScriptValue& v);              \
    static sad::Maybe< A > perform(const QScriptValue& v);       \
};

DEFINE_TO_VALUE_FOR_TYPE( QScriptValue )
DEFINE_TO_VALUE_FOR_TYPE( double )
DEFINE_TO_VALUE_FOR_TYPE( float )
DEFINE_TO_VALUE_FOR_TYPE( int )
DEFINE_TO_VALUE_FOR_TYPE( long )
DEFINE_TO_VALUE_FOR_TYPE( long long )
DEFINE_TO_VALUE_FOR_TYPE( sad::AColor )
DEFINE_TO_VALUE_FOR_TYPE( sad::Color )
DEFINE_TO_VALUE_FOR_TYPE( sad::Point2D )
DEFINE_TO_VALUE_FOR_TYPE( sad::Point2I )
DEFINE_TO_VALUE_FOR_TYPE( sad::Point3D )
DEFINE_TO_VALUE_FOR_TYPE( sad::Point3I )
DEFINE_TO_VALUE_FOR_TYPE( sad::Size2D )
DEFINE_TO_VALUE_FOR_TYPE( sad::Size2I )
DEFINE_TO_VALUE_FOR_TYPE( sad::Rect2D )
DEFINE_TO_VALUE_FOR_TYPE( sad::Rect2I )
DEFINE_TO_VALUE_FOR_TYPE( sad::String )
DEFINE_TO_VALUE_FOR_TYPE( std::string )
DEFINE_TO_VALUE_FOR_TYPE( QString )
DEFINE_TO_VALUE_FOR_TYPE( short )
DEFINE_TO_VALUE_FOR_TYPE( bool )
DEFINE_TO_VALUE_FOR_TYPE( char )
DEFINE_TO_VALUE_FOR_TYPE( signed char )
DEFINE_TO_VALUE_FOR_TYPE( unsigned char )
DEFINE_TO_VALUE_FOR_TYPE( unsigned int )
DEFINE_TO_VALUE_FOR_TYPE( unsigned long )
DEFINE_TO_VALUE_FOR_TYPE( unsigned long long )
DEFINE_TO_VALUE_FOR_TYPE( unsigned short )
DEFINE_TO_VALUE_FOR_TYPE( sad::db::Object* )
DEFINE_TO_VALUE_FOR_TYPE( sad::Scene* )
DEFINE_TO_VALUE_FOR_TYPE( sad::SceneNode* )
DEFINE_TO_VALUE_FOR_TYPE( sad::Label* )
DEFINE_TO_VALUE_FOR_TYPE( sad::Sprite2D* )
DEFINE_TO_VALUE_FOR_TYPE( sad::db::custom::Object* )
DEFINE_TO_VALUE_FOR_TYPE( sad::p2d::app::Way* )
DEFINE_TO_VALUE_FOR_TYPE( sad::dialogue::Dialogue* )
DEFINE_TO_VALUE_FOR_TYPE( sad::dialogue::Phrase )
DEFINE_TO_VALUE_FOR_TYPE( sad::Vector<sad::p2d::app::WayPoint> )
DEFINE_TO_VALUE_FOR_TYPE( sad::Vector<sad::dialogue::Phrase> )
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Animation*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Composite*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Blinking*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::CameraRotation*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::CameraShaking*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Color*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::FontList*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::FontSize*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::OptionList*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Parallel*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Resize*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Rotate*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::SimpleMovement*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Sequential*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::TextureCoordinatesList*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::TextureCoordinatesContinuous*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Typing*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::WayMoving*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Instance*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::WayInstance*)
DEFINE_TO_VALUE_FOR_TYPE(sad::animations::Group*)
DEFINE_TO_VALUE_FOR_TYPE(sad::Vector<sad::String>)
#undef DEFINE_TO_VALUE_FOR_TYPE

}
