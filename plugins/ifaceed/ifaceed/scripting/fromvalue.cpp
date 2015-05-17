#include "fromvalue.h"
#include "point2d.h"
#include "point2i.h"
#include "point3d.h"
#include "point3i.h"
#include "rect2d.h"
#include "rect2i.h"
#include "size2d.h"
#include "size2i.h"
#include "color.h"
#include "acolor.h"

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
Q_DECLARE_METATYPE(sad::Point3D)
Q_DECLARE_METATYPE(sad::Point3I)
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

QScriptValue scripting::FromValue<sad::Point2D>::perform(const sad::Point2D& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Point2D(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Point2I>::perform(const sad::Point2I& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Point2I(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Point3D>::perform(const sad::Point3D& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Point3D(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Point3I>::perform(const sad::Point3I& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Point3I(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Size2D>::perform(const sad::Size2D& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Size2D(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Size2I>::perform(const sad::Size2I& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Size2I(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Rect2D>::perform(const sad::Rect2D& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Rect2D(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Rect2I>::perform(const sad::Rect2I& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Rect2I(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::Color>::perform(const sad::Color& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::Color(v), QScriptEngine::AutoOwnership);
}

QScriptValue scripting::FromValue<sad::AColor>::perform(const sad::AColor& v, QScriptEngine* e)
{
	return e->newQObject(new scripting::AColor(v), QScriptEngine::AutoOwnership);
}

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


QScriptValue scripting::FromValue<QStringList>::perform(const QStringList& v, QScriptEngine* e)
{
    QScriptValue result = e->newArray(v.size());
    for(size_t i = 0; i < v.size(); i++)
    {
        result.setProperty(i, v[i]);
    }

    return result;
}

QScriptValue scripting::FromValue<QVector<unsigned long long> >::perform(const QVector<unsigned long long>& v, QScriptEngine* e)
{
    QScriptValue result = e->newArray(v.size());
    for(size_t i = 0; i < v.size(); i++)
    {
        result.setProperty(i, scripting::FromValue<unsigned long long>::perform(v[i], e));
    }
    return result;
}

QScriptValue scripting::FromValue<QScriptValue >::perform(const QScriptValue& v, QScriptEngine* e)
{
    return v;
}

QScriptValue scripting::FromValue<sad::Vector<sad::String> >::perform(const sad::Vector<sad::String>& v, QScriptEngine* e)
{
    QScriptValue result = e->newArray(v.size());
    for(size_t i = 0; i < v.size(); i++)
    {
        result.setProperty(i, scripting::FromValue<sad::String>::perform(v[i], e));
    }
    return result;
}

QScriptValue scripting::FromValue<sad::Vector<unsigned long long> >::perform(const sad::Vector<unsigned long long>& v, QScriptEngine* e)
{
    QScriptValue result = e->newArray(v.size());
    for(size_t i = 0; i < v.size(); i++)
    {
        result.setProperty(i, scripting::FromValue<unsigned long long>::perform(v[i], e));
    }
    return result;
}
