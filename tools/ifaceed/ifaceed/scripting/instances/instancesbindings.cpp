#include "instancesbindings.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../queryobject.h"
#include "../tovalue.h"
#include "../fromvalue.h"
#include "../scripting.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../history/instances/instancesnew.h"
#include "../../history/instances/instancesremove.h"

#include <animations/animationswayinstance.h>

QVector<unsigned long long> scripting::instances::list()
{
    return scripting::query_table("animationinstances", "sad::animations::Instance");
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
    core::Editor* e = scripting->editor();
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
    scripting->editor()->actions()->instanceActions()->removeInstanceFromDatabase(instance, false);
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


dukpp03::qt::JSObject* scripting::instances::init(scripting::Scripting* s, dukpp03::qt::JSObject* animations)
{
    dukpp03::qt::JSObject*  instances_value = new dukpp03::qt::JSObject();
    instances_value->setProperty("list", dukpp03::qt::make_function::from(scripting::instances::list)); // E.animations.instances.list

    dukpp03::qt::MultiMethod* _add_instance = new dukpp03::qt::MultiMethod();
    {
        std::function<unsigned long long(scripting::Scripting*, sad::animations::Instance*, const sad::String& name, double start_time)>  add_instance_to_db = [](scripting::Scripting* sc, sad::animations::Instance* instance, const sad::String& name, double start_time) {
            instance->setProperty("starttime", start_time);
            instance->setObjectName(name);

            sad::Renderer::ref()->database("")->table("animationinstances")->add(instance);

            core::Editor* e = sc->editor();
            history::instances::New* c = new history::instances::New(instance);
            c->commit(e);
            e->currentBatchCommand()->add(c);
            return instance->MajorId;
        };
        // animation as sad::animations::Animation*
        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::animations::Animation*, sad::db::Object* o, double)> _add_instance_1 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::animations::Animation* animation, sad::db::Object* o, double start_time) -> unsigned long long {
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", sad::String(""));
            instance->setProperty("animationmajorid", animation->MajorId);
            if (o->isInstanceOf("sad::Scene") || o->isInstanceOf("sad::SceneNode"))
            {
                instance->setProperty("object", o->MajorId);
            }

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::animations::Animation*, unsigned long o, double)> _add_instance_2 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::animations::Animation* animation, unsigned long o, double start_time) {
            if (o != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must be valid object");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", sad::String(""));
            instance->setProperty("animationmajorid", animation->MajorId);

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::animations::Animation*, sad::String o, double)> _add_instance_3 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::animations::Animation* animation, sad::String o, double start_time) {
            if (o.length() != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must have type sad::db::Object*");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", sad::String(""));
            instance->setProperty("animationmajorid", animation->MajorId);

            return add_instance_to_db(sc, instance, name, start_time);
        };
        // animation as unsigned long long
        std::function<unsigned long long(scripting::Scripting*, sad::String, unsigned long long, sad::db::Object* o, double)> _add_instance_4 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, unsigned long long animation, sad::db::Object* o, double start_time) {
            if (animation != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            if (o->isInstanceOf("sad::Scene") || o->isInstanceOf("sad::SceneNode"))
            {
                instance->setProperty("object", o->MajorId);
            }

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, unsigned long long, unsigned long o, double)> _add_instance_5 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, unsigned long long animation, unsigned long o, double start_time) {
            if (animation != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
            }
            if (o != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must be valid object");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, unsigned long long,  sad::String o, double)> _add_instance_6 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, unsigned long long animation, sad::String o, double start_time) {
            if (animation != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
            }
            if (o.length() != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must have type sad::db::Object*");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();

            return add_instance_to_db(sc, instance, name, start_time);
        };

        // animation as sad::String
        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::String, sad::db::Object* o, double)> _add_instance_7 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::String animation, sad::db::Object* o, double start_time) {
            if (animation.length() != 0)
            {
                sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(animation);
                if (r)
                {
                    if (r->isInstanceOf("sad::animations::Animation") == false)
                    {
                        r = NULL;
                    }
                }
                if (!r)
                {
                    sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
                }
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", animation);
            instance->setProperty("animationmajorid", 0ull);
            if (o->isInstanceOf("sad::Scene") || o->isInstanceOf("sad::SceneNode"))
            {
                instance->setProperty("object", o->MajorId);
            }

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::String, unsigned long o, double)> _add_instance_8 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::String animation, unsigned long o, double start_time) {
            if (animation.length() != 0)
            {
                sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(animation);
                if (r)
                {
                    if (r->isInstanceOf("sad::animations::Animation") == false)
                    {
                        r = NULL;
                    }
                }
                if (!r)
                {
                    sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
                }
            }
            if (o != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must be valid object");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", animation);
            instance->setProperty("animationmajorid", 0ull);
            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::String, sad::String o, double)> _add_instance_9 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::String animation, sad::String o, double start_time) {
            if (animation.length() != 0)
            {
                sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(animation);
                if (r)
                {
                    if (r->isInstanceOf("sad::animations::Animation") == false)
                    {
                        r = NULL;
                    }
                }
                if (!r)
                {
                    sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
                }
            }
            if (o.length() != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must have type sad::db::Object*");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", animation);
            instance->setProperty("animationmajorid", 0ull);
            return add_instance_to_db(sc, instance, name, start_time);
        };

        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_1));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_2));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_3));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_4));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_5));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_6));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_7));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_8));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_9));
    }
    instances_value->setProperty("_addInstance", static_cast<dukpp03::qt::Callable*>(_add_instance)); // E.animations.instances._addInstance


    animations->setProperty("instances", instances_value);

    bool b = s->context()->eval(
        "E.animations.instances.addInstance = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"animation\" in o == false)"
        "   {                              "
        "     o[\"animation\"] = 0;        "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"object\" in o == false)  "
        "   {                              "
        "      o[\"object\"] = 0;          "
        "   }                              "
        "   if (\"starttime\" in o == false)    "
        "   {                              "
        "      o[\"starttime\"] = 0;            "
        "   }                              "
        "   return E.animations.instances._addInstance(o[\"name\"], o[\"animation\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.addWayInstance = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"way\" in o == false)     "
        "   {                              "
        "      o[\"way\"] = 0;             "
        "   }                              "
        "   if (\"object\" in o == false)  "
        "   {                              "
        "      o[\"object\"] = 0;          "
        "   }                              "
        "   if (\"starttime\" in o == false)    "
        "   {                              "
        "      o[\"starttime\"] = 0;            "
        "   }                              "
        "   return E.animations.instances._addWayInstance(o[\"name\"], o[\"way\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.animations.instances.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.animations.instances.set(arguments[0], arguments[1], arguments[2]); return E.animations.instances;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert( b );
    return instances_value;
}