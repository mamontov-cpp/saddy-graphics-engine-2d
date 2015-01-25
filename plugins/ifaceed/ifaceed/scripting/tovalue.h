/*! \file tovalue.h
 *  \author HiddenSeeker
 *
 *  Contains custom conversions from QScriptValue
 */
#pragma once
#include <QString>
#include <QScriptValue>
#include <maybe.h>
#include <sadcolor.h>
#include <sadrect.h>
#include <sadsize.h>
#include <sadstring.h>

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

namespace p2d
{
	
namespace app
{

class Way;

}

}

namespace dialogue
{
class Dialogue;
class Phrase;
}

}

namespace scripting
{
/*! A class, for performing safe conversion of script value to type
 */
template<
    typename T
>
class ToValue
{
public:
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
class ToValue< A >                                               \
{                                                                \
public:                                                          \
    static sad::Maybe< A > perform(const QScriptValue& v);       \
};

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
#undef DEFINE_TO_VALUE_FOR_TYPE

}
