#include "../scripting.h"
#include "databasebindings.h"
#include "databasepropertysetter.h"
#include "databasepropertygetter.h"

#include <renderer.h>

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/table/delegate.h"

#include "../../gui/mainpanelproxy.h"

#include "../animations/animationsbindings.h"
#include "../instances/instancesbindings.h"
#include "../groups/groupsbindings.h"

static QSet<QString> invisible_properties;

void scripting::database::initializeInvisiblePropertiesList()
{
    invisible_properties.insert("palette");
    invisible_properties.insert("global_renderer_offset");
    invisible_properties.insert("global_renderer_grid_enabled");
    invisible_properties.insert("global_renderer_grid_settings");
    invisible_properties.insert("global_renderer_grid_color");
}

const QSet<QString>& scripting::database::getInvisibleProperties()
{
    if (invisible_properties.size() == 0)
    {
        initializeInvisiblePropertiesList();
    }
    return invisible_properties;
}

bool scripting::database::addProperty(scripting::Scripting* s, sad::String type, sad::String name)
{
    return s->editor()->panelProxy()->scriptableAddProperty(type, name, false);	
}

bool scripting::database::removeProperty(scripting::Scripting* s, sad::String name)
{
    gui::MainPanelProxy* panel = s->editor()->panelProxy();
    bool result = false;
    if(sad::Renderer::ref()->database("")->propertyByName(name) != NULL)
    {
        if (panel->delegatesByName().contains(name))
        {
            result = true;
            panel->delegatesByName()[name]->removeWithCommand();
        }
    }
    return result;
}

QStringList scripting::database::list()
{
    QStringList list;
    sad::db::Database* db = sad::Renderer::ref()->database("");
    sad::db::Database::Properties::const_iterator it = db->begin();
    const QSet<QString>& set = scripting::database::getInvisibleProperties();
    for(; it != db->end(); ++it)
    {
        QString key = STD2QSTRING(it.key());
        if (set.contains(key) == false)
        {
            list << key;
        }
    }

    return list;
}

sad::String scripting::database::type(sad::db::Object* o)
{
    return o->serializableName();
}

QStringList scripting::database::readableProperties(sad::db::Object* o)
{

    QStringList list;
    list << "majorid";
    list << "minorid";
    list << "name";

    if (o->isInstanceOf("sad::Scene"))
    {
        list << "layer";
    }

    if (o->isInstanceOf("sad::SceneNode"))
    {
        list << "layer";
        list << "scene";
        list << "visible";
        list << "area";
        list << "angle";
        list << "color";
    }

    if (o->isInstanceOf("sad::Label")
        || o->isInstanceOf("sad::db::custom::Object"))
    {
        list << "fontsize";
        list << "text";
        list << "linespacing";
        list << "font";
        list << "maximallinewidth";
        list << "overflowstrategy";
        list << "breaktext";
        list << "textellipsisposition";
        list << "maximallinescount";
        list << "overflowstrategyforlines";
        list << "textellipsispositionforlines";
        list << "hasformatting";
    }

    if (o->isInstanceOf("sad::Sprite2D")
        || o->isInstanceOf("sad::db::custom::Object"))
    {
        list << "flipx";
        list << "flipy";
        list << "options";
    }

    if (o->isInstanceOf("sad::db::custom::Object"))
    {
        list << "schema";
        sad::db::custom::Object* co = static_cast<sad::db::custom::Object*>(o);
        const sad::Hash<sad::String, sad::db::Property*> & sprops =  co->schemaProperties();
        for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = sprops.const_begin();
            it != sprops.const_end();
            ++it)
        {
            list << STD2QSTRING(it.key());
        }
    }

    if (o->isInstanceOf("sad::p2d::app::Way"))
    {
        list << "totaltime";
        list << "closed";
    }

    scripting::animations::checkPropertiesForAnimations(o, list, true);
    scripting::instances::checkProperties(o, list, true);
    scripting::groups::checkProperties(o, list, true);

    return list;
}

QStringList scripting::database::writableProperties(sad::db::Object* o)
{
    QStringList list;
    list << "name";

    if (o->isInstanceOf("sad::SceneNode"))
    {
        list << "visible";
        list << "area";
        list << "angle";
        list << "color";
    }

    if (o->isInstanceOf("sad::Label")
        || o->isInstanceOf("sad::db::custom::Object"))
    {
        list << "fontsize";
        list << "text";
        list << "linespacing";
        list << "font";
        list << "maximallinewidth";
        list << "overflowstrategy";
        list << "breaktext";
        list << "textellipsisposition";
        list << "maximallinescount";
        list << "overflowstrategyforlines";
        list << "textellipsispositionforlines";
        list << "hasformatting";
    }

    if (o->isInstanceOf("sad::Sprite2D")
        || o->isInstanceOf("sad::db::custom::Object"))
    {
        list << "flipx";
        list << "flipy";
        list << "options";
    }

    if (o->isInstanceOf("sad::db::custom::Object"))
    {
        list << "schema";
        sad::db::custom::Object* co = static_cast<sad::db::custom::Object*>(o);
        const sad::Hash<sad::String, sad::db::Property*> & sprops =  co->schemaProperties();
        for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = sprops.const_begin();
            it != sprops.const_end();
            ++it)
        {
            list << STD2QSTRING(it.key());
        }
    }

    if (o->isInstanceOf("sad::p2d::app::Way"))
    {
        list << "totaltime";
        list << "closed";
    }


    scripting::animations::checkPropertiesForAnimations(o, list, false);
    scripting::instances::checkProperties(o, list, false);
    scripting::groups::checkProperties(o, list, false);

    return list;
}

dukpp03::qt::JSObject* scripting::database::init(scripting::Scripting* s, dukpp03::qt::JSObject* e)
{
    dukpp03::qt::JSObject* db = new dukpp03::qt::JSObject();
    db->setProperty("list", dukpp03::qt::make_function::from(scripting::database::list));
    db->setProperty("type", dukpp03::qt::make_function::from(scripting::database::type));
    db->setProperty("readableProperties", dukpp03::qt::make_function::from(scripting::database::readableProperties));
    db->setProperty("writableProperties", dukpp03::qt::make_function::from(scripting::database::writableProperties));
    db->setProperty("addProperty", dukpp03::qt::curried1::from(s, scripting::database::addProperty));
    db->setProperty("removeProperty", dukpp03::qt::curried1::from(s, scripting::database::removeProperty));

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();
#define PUSH_SETTER(TYPE) set->add(new scripting::database::PropertySetter< TYPE >(s));
    PUSH_SETTER(double)
        PUSH_SETTER(float)
        PUSH_SETTER(int)
        PUSH_SETTER(long)
        PUSH_SETTER(long long)
        PUSH_SETTER(sad::AColor)
        PUSH_SETTER(sad::Color)
        PUSH_SETTER(sad::Point2D)
        PUSH_SETTER(sad::Point2I)
        PUSH_SETTER(sad::Point3D)
        PUSH_SETTER(sad::Point3I)
        PUSH_SETTER(sad::Size2D)
        PUSH_SETTER(sad::Size2I)
        PUSH_SETTER(sad::Rect2D)
        PUSH_SETTER(sad::Rect2I)
        PUSH_SETTER(sad::String)
        PUSH_SETTER(std::string)
        PUSH_SETTER(QString)
        PUSH_SETTER(short)
        PUSH_SETTER(bool)
        PUSH_SETTER(char)
        PUSH_SETTER(signed char)
        PUSH_SETTER(unsigned char)
        PUSH_SETTER(unsigned int)
        PUSH_SETTER(unsigned long)
        PUSH_SETTER(unsigned long long)
        PUSH_SETTER(unsigned short)
#undef PUSH_SETTER
        db->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.db.set


    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
#define PUSH_GETTER(TYPE) get->add(new scripting::database::PropertyGetter< TYPE >());
    PUSH_GETTER(double)
        PUSH_GETTER(float)
        PUSH_GETTER(int)
        PUSH_GETTER(long)
        PUSH_GETTER(long long)
        PUSH_GETTER(sad::AColor)
        PUSH_GETTER(sad::Color)
        PUSH_GETTER(sad::Point2D)
        PUSH_GETTER(sad::Point2I)
        PUSH_GETTER(sad::Point3D)
        PUSH_GETTER(sad::Point3I)
        PUSH_GETTER(sad::Size2D)
        PUSH_GETTER(sad::Size2I)
        PUSH_GETTER(sad::Rect2D)
        PUSH_GETTER(sad::Rect2I)
        PUSH_GETTER(sad::String)
        PUSH_GETTER(std::string)
        PUSH_GETTER(QString)
        PUSH_GETTER(short)
        PUSH_GETTER(bool)
        PUSH_GETTER(char)
        PUSH_GETTER(signed char)
        PUSH_GETTER(unsigned char)
        PUSH_GETTER(unsigned int)
        PUSH_GETTER(unsigned long)
        PUSH_GETTER(unsigned long long)
        PUSH_GETTER(unsigned short)
#undef PUSH_GETTER
        db->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.db.get

    e->setProperty("db", db);

    bool b = s->context()->eval(
        "E.db.attr = function() {"
        "   if (arguments.length == 1)"
        "   {"
        "       return E.db.get(arguments[0]);"
        "   }"
        "   if (arguments.length == 2)"
        "   {"
        "       E.db.set(arguments[0], arguments[1]); return E.db;"
        "   }"
        "   throw new Error(\"Specify 1 or 2 arguments\");"
        "};"
    );
    assert(b);
    return db;
}