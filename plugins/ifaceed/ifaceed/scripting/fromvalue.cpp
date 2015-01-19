#include "fromvalue.h"
#include "point2d.h"

#include <scene.h>
#include <scenenode.h>
#include <label.h>
#include <sprite2d.h>
#include <db/custom/customobject.h>
#include <p2d/app/way.h>
#include <dialogue/dialogue.h>
#include <dialogue/phrase.h>

Q_DECLARE_METATYPE(sad::Color)
Q_DECLARE_METATYPE(sad::AColor)
Q_DECLARE_METATYPE(sad::Point2D)
Q_DECLARE_METATYPE(sad::Point2I)
Q_DECLARE_METATYPE(sad::Size2D)
Q_DECLARE_METATYPE(sad::Size2I)
Q_DECLARE_METATYPE(sad::Rect2D)
Q_DECLARE_METATYPE(sad::String)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(sad::dialogue::Phrase)

#define DECLARE_FROM_VALUE_AS_VARIANT(TYPE)                                           \
QScriptValue scripting::FromValue<TYPE>::perform(const TYPE& v, QScriptEngine* e)         \
{                                                                                         \
	QVariant variant;                                                                     \
	variant.setValue(v);                                                                  \
	return e->newVariant(variant);                                                        \
}


QScriptValue scripting::FromValue<double>::perform(const double& v, QScriptEngine* )
{
	return QScriptValue(v);
}

QScriptValue scripting::FromValue<float>::perform(const float& v, QScriptEngine* )
{
	return QScriptValue(static_cast<double>(v));
}

QScriptValue scripting::FromValue<int>::perform(const int& v, QScriptEngine* )
{
	return QScriptValue(v);
}

DECLARE_FROM_VALUE_AS_VARIANT(long)
DECLARE_FROM_VALUE_AS_VARIANT(long long)
DECLARE_FROM_VALUE_AS_VARIANT( sad::AColor )
DECLARE_FROM_VALUE_AS_VARIANT( sad::Color )

QScriptValue scripting::FromValue<sad::Point2D>::perform(const sad::Point2D& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Point2D(v), QScriptEngine::AutoOwnership);
}

DECLARE_FROM_VALUE_AS_VARIANT( sad::Point2I )
DECLARE_FROM_VALUE_AS_VARIANT( sad::Size2D )
DECLARE_FROM_VALUE_AS_VARIANT( sad::Size2I )
DECLARE_FROM_VALUE_AS_VARIANT( sad::Rect2D )

QScriptValue scripting::FromValue<sad::String>::perform(const sad::String& v, QScriptEngine* )
{
	return QScriptValue(v.c_str());
}


QScriptValue scripting::FromValue<std::string>::perform(const std::string& v, QScriptEngine* )
{
	return QScriptValue(v.c_str());
}

QScriptValue scripting::FromValue<QString>::perform(const QString& v, QScriptEngine* )
{
	return QScriptValue(v);
}

QScriptValue scripting::FromValue<short>::perform(const short& v, QScriptEngine* )
{
	return QScriptValue(static_cast<int>(v));
}

QScriptValue scripting::FromValue<bool>::perform(const bool& v, QScriptEngine* )
{
	return QScriptValue(v);
}

QScriptValue scripting::FromValue<char>::perform(const char& v, QScriptEngine* )
{
	char string[2] = { v, 0 };
	return QScriptValue(string);
}

QScriptValue scripting::FromValue<signed char>::perform(const signed char& v, QScriptEngine* )
{
	char string[2] = { v, 0 };
	return QScriptValue(string);
}

QScriptValue scripting::FromValue<unsigned char>::perform(const unsigned char& v, QScriptEngine* )
{
	char string[2] = { static_cast<signed char>(v), 0 };
	return QScriptValue(string);
}

QScriptValue scripting::FromValue<unsigned int>::perform(const unsigned int& v, QScriptEngine* )
{
	return QScriptValue(v);
}

DECLARE_FROM_VALUE_AS_VARIANT(unsigned long)
DECLARE_FROM_VALUE_AS_VARIANT(unsigned long long)

QScriptValue scripting::FromValue<unsigned short>::perform(const unsigned short& v, QScriptEngine* )
{
	return QScriptValue(static_cast<unsigned int>(v));
}

QScriptValue scripting::FromValue<sad::db::Object*>::perform(const sad::db::Object*& v, QScriptEngine* e)
{
	unsigned long long majorid = 0;
	if (v)
	{
		majorid = v->MajorId;
	}
	return scripting::FromValue<unsigned long long>::perform(majorid, e);
}

#define DECLARE_FROM_VALUE_AS_DBOBJECT(TYPE)                                          \
QScriptValue scripting::FromValue<TYPE>::perform(const TYPE& v, QScriptEngine* e)         \
{                                                                                         \
	TYPE a = const_cast<TYPE>(v);                                                         \
	sad::db::Object const * o = a;                                                        \
	return scripting::FromValue<sad::db::Object*>::perform(o, e);                         \
}

DECLARE_FROM_VALUE_AS_DBOBJECT(sad::Scene*)
DECLARE_FROM_VALUE_AS_DBOBJECT(sad::SceneNode*)
DECLARE_FROM_VALUE_AS_DBOBJECT(sad::Label*)
DECLARE_FROM_VALUE_AS_DBOBJECT(sad::Sprite2D*)
DECLARE_FROM_VALUE_AS_DBOBJECT(sad::db::custom::Object*)
DECLARE_FROM_VALUE_AS_DBOBJECT(sad::p2d::app::Way*)
DECLARE_FROM_VALUE_AS_DBOBJECT(sad::dialogue::Dialogue*)

DECLARE_FROM_VALUE_AS_VARIANT(sad::dialogue::Phrase)
