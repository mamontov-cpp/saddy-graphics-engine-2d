/*! \file fromvalue.h
 *	\author HiddenSeeker
 *
 *	Converts a specific value to a QScriptValue
 */
#pragma once
#include <QScriptEngine>
#include <QVector>
#include <QStringList>

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

namespace custom
{
class Object;	
}

class Object;

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
class FromValue
{
public:
    /*! Tries to perform conversion from specified type
     *  \return a value
     */
    inline static QScriptValue perform(const T&, QScriptEngine*)
    {
	    return QScriptValue();
    }
};


#define DEFINE_FROM_VALUE_FOR_TYPE(A)                       \
template<                                                       \
>                                                               \
class FromValue< A >                                            \
{                                                               \
public:                                                         \
    static QScriptValue perform(const A&, QScriptEngine*e);     \
};
DEFINE_FROM_VALUE_FOR_TYPE( double )
DEFINE_FROM_VALUE_FOR_TYPE( float )
DEFINE_FROM_VALUE_FOR_TYPE( int )
DEFINE_FROM_VALUE_FOR_TYPE( long )
DEFINE_FROM_VALUE_FOR_TYPE( long long )
DEFINE_FROM_VALUE_FOR_TYPE( sad::AColor )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Color )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Point2D )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Point2I )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Point3D )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Point3I )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Size2D )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Size2I )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Rect2D )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Rect2I )
DEFINE_FROM_VALUE_FOR_TYPE( sad::String )
DEFINE_FROM_VALUE_FOR_TYPE( std::string )
DEFINE_FROM_VALUE_FOR_TYPE( QString )
DEFINE_FROM_VALUE_FOR_TYPE( short )
DEFINE_FROM_VALUE_FOR_TYPE( bool )
DEFINE_FROM_VALUE_FOR_TYPE( char )
DEFINE_FROM_VALUE_FOR_TYPE( signed char )
DEFINE_FROM_VALUE_FOR_TYPE( unsigned char )
DEFINE_FROM_VALUE_FOR_TYPE( unsigned int )
DEFINE_FROM_VALUE_FOR_TYPE( unsigned long )
DEFINE_FROM_VALUE_FOR_TYPE( unsigned long long )
DEFINE_FROM_VALUE_FOR_TYPE( unsigned short )
DEFINE_FROM_VALUE_FOR_TYPE( sad::db::Object* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Scene* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::SceneNode* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Label* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::Sprite2D* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::db::custom::Object* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::p2d::app::Way* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::dialogue::Dialogue* )
DEFINE_FROM_VALUE_FOR_TYPE( sad::dialogue::Phrase )
DEFINE_FROM_VALUE_FOR_TYPE( QStringList )
DEFINE_FROM_VALUE_FOR_TYPE( QVector<unsigned long long> )
#undef DEFINE_FROM_VALUE_FOR_TYPE

}
