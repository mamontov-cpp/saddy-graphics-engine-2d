#include "instancesbindings.h"

#include "../querytable.h"
#include "../queryobject.h"
#include "../tovalue.h"
#include "../fromvalue.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/instanceactions.h"

#include "../../history/instances/instancesnew.h"
#include "../../history/instances/instancesremove.h"

#include <animations/animationswayinstance.h>

Q_DECLARE_METATYPE(sad::animations::Instance*);
Q_DECLARE_METATYPE(sad::animations::WayInstance*);

QScriptValue scripting::instances::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("animationinstances", "sad::animations::Instance", ctx, engine);
}

unsigned long long scripting::instances::_addInstance(
    scripting::Scripting* scripting,
    sad::String name,
    unsigned long long animationid,
    sad::String animationname,
    QScriptValue object,
    double starttime
)
{
    unsigned long long result = 0;

    sad::animations::Instance* instance = new sad::animations::Instance();
    bool setmajorid = false;
    if (animationname.length())
    {
        sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(animationname);
        if (r)
        {
            instance->setProperty("animation", animationname);
        }
        else
        {
            bool valid = false;
            if (animationid == 0) {
                QScriptValue v(QString(animationname.c_str()));
                sad::Maybe<sad::animations::Animation*> a = scripting::query<sad::animations::Animation*>(v);
                if (a.exists())
                {
                    valid = true;
                    instance->setProperty("animation", sad::String(""));
                    instance->setProperty("animationmajorid", a.value()->MajorId);
                    setmajorid = true;
                }
            }
            if (!valid)
            {
                delete instance;
                scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, QString("_addInstance(): \"") + QString(animationname.c_str()) + "\" does not name a valid resource");
                return result;
            }
        }
    } else
    {
        instance->setProperty("animation", sad::String(""));
    }

    if (animationid != 0)
    {
         QScriptValue v = scripting::FromValue<unsigned long long>::perform(animationid, scripting->engine());
         sad::Maybe<sad::animations::Animation*> a = scripting::query<sad::animations::Animation*>(v);
         if (a.exists())
         {
              instance->setProperty("animationmajorid", animationid);
         }
         else
         {
             delete instance;
             scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, QString("_addInstance(): \"") + QString::number(animationid) + "\" does not name a valid animation");
             return result;
         }
    }
    else
    {
        if (setmajorid == false)
        {
            instance->setProperty("animationmajorid", 0ull);
        }
    }

    sad::Maybe<unsigned long long> maybeobjectid = scripting::ToValue<unsigned long long>::perform(object);
    sad::Maybe<sad::String> maybeobjectname = scripting::ToValue<sad::String>::perform(object);
    bool validobject = false;
    unsigned long long objectid = 0ull;
    if (maybeobjectid.exists() || maybeobjectname.exists())
    {
        bool donotscan = false;
        if (maybeobjectid.exists())
        {
           donotscan =  maybeobjectid.value() == 0;
        }
        else
        {
           donotscan =  maybeobjectname.value().length() == 0;
        }
        if (!donotscan)
        {
            sad::db::Object* a = scripting::query_object(object);
            if (a)
            {
				if (a->isInstanceOf("sad::Scene") || a->isInstanceOf("sad::SceneNode"))
				{
                 validobject = true;
                 objectid = a->MajorId;
				}
            }
        }
        else
        {
            validobject = true;
        }
    }
    if (validobject)
    {
        instance->setProperty("object", objectid);
    }
    else
    {
        delete instance;
        scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, QString("_addInstance(): \"") + object.toString() + "\" does not name a valid object");
        return result;
    }

    instance->setProperty("starttime", starttime);
    instance->setObjectName(name);

    sad::Renderer::ref()->database("")->table("animationinstances")->add(instance);

    result = instance->MajorId;
    core::Editor* e = scripting->panel()->editor();
    history::instances::New* c  = new history::instances::New(instance);
    c->commit(e);
    e->currentBatchCommand()->add(c);

   return result;
}

unsigned long long scripting::instances::_addWayInstance(
    scripting::Scripting* scripting,
    sad::String name,
    QScriptValue way,
    QScriptValue object,
    double starttime
)
{
    unsigned long long result = 0;

    sad::animations::WayInstance* instance = new sad::animations::WayInstance();

    sad::Maybe<sad::p2d::app::Way*> maybeway = scripting::ToValue<sad::p2d::app::Way*>::perform(way);
    unsigned long long wayid = 0ull;
    bool validway = false;
    if (maybeway.exists())
    {
        validway = true;
        wayid = maybeway.value()->MajorId;
    }
    else
    {
        sad::Maybe<unsigned long long> maybewayid = scripting::ToValue<unsigned long long>::perform(way);
        sad::Maybe<sad::String> maybewayname = scripting::ToValue<sad::String>::perform(way);
        if (maybewayid.exists())
        {
            validway = maybewayid.value() == 0;
        }
        else
        {
            if (maybewayname.exists())
            {
                 validway = maybewayname.value().length() == 0;
            }
        }
    }

    if (validway)
    {
        instance->setProperty("way", wayid);
    }
    else
    {
        delete instance;
        scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, QString("_addWayInstance(): \"") + way.toString() + "\" does not name a valid way and is not 0 or empty string");
        return result;
    }

    sad::Maybe<unsigned long long> maybeobjectid = scripting::ToValue<unsigned long long>::perform(object);
    sad::Maybe<sad::String> maybeobjectname = scripting::ToValue<sad::String>::perform(object);
    bool validobject = false;
    unsigned long long objectid = 0ull;
    if (maybeobjectid.exists() || maybeobjectname.exists())
    {
        bool donotscan = false;
        if (maybeobjectid.exists())
        {
           donotscan =  maybeobjectid.value() == 0;
        }
        else
        {
           donotscan =  maybeobjectname.value().length() == 0;
        }
        if (!donotscan)
        {
            sad::db::Object* a = scripting::query_object(object);
            if (a)
            {
                if (a->isInstanceOf("sad::Scene") || a->isInstanceOf("sad::SceneNode"))
				{
					validobject = true;
					objectid = a->MajorId;
				}
            }
        }
        else
        {
            validobject = true;
        }
    }
    if (validobject)
    {
        instance->setProperty("object", objectid);
    }
    else
    {
        delete instance;
        scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, QString("_addWayInstance(): \"") + object.toString() + "\" does not name a valid object");
        return result;
    }

    instance->setProperty("starttime", starttime);
    instance->setObjectName(name);

    sad::Renderer::ref()->database("")->table("animationinstances")->add(instance);

    result = instance->MajorId;
    core::Editor* e = scripting->panel()->editor();
    history::instances::New* c  = new history::instances::New(instance);
    c->commit(e);
    e->currentBatchCommand()->add(c);

   return result;
}

void scripting::instances::remove(
    scripting::Scripting* scripting,
    sad::animations::Instance* instance
)
{
    scripting->panel()->instanceActions()->removeInstanceFromDatabase(instance, false);
}


void scripting::instances::checkProperties(
	const sad::Maybe<sad::db::Object*>& obj,
	QStringList& list,
	bool
)
{
	if (obj.value()->isInstanceOf("sad::animations::Instance"))
	{
		list << "animation";
		list << "animationmajorid";
		list << "object";
		list << "starttime";
	}

	if (obj.value()->isInstanceOf("sad::animations::WayInstance"))
	{
		list << "way";
	}
}
