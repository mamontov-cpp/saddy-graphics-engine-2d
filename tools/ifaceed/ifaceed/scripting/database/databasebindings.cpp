#include "../scripting.h"
#include "databasebindings.h"

#include <renderer.h>

#include "../tovalue.h"
#include "../fromvalue.h"

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

QScriptValue scripting::database::readableProperties(QScriptContext* ctx, QScriptEngine* engine)
{
    if (ctx->argumentCount() != 1)
    {
        ctx->throwError("readableProperties(): accepts only 1 argument");
        return QScriptValue();
    }

    sad::Maybe<sad::db::Object*> obj = scripting::ToValue<sad::db::Object*>::perform(ctx->argument(0));
    if (obj.exists() == false)
    {
        ctx->throwError("readableProperties(): first argument must be sad::db::Object");
        return QScriptValue();
    }

    QStringList list;
    list << "majorid";
    list << "minorid";
    list << "name";

    if (obj.value()->isInstanceOf("sad::Scene"))
    {
        list << "layer";
    }

    if (obj.value()->isInstanceOf("sad::SceneNode"))
    {
        list << "layer";
        list << "scene";
        list << "visible";
        list << "area";
        list << "angle";
        list << "color";
    }

    if (obj.value()->isInstanceOf("sad::Label")
        || obj.value()->isInstanceOf("sad::db::custom::Object"))
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

    if (obj.value()->isInstanceOf("sad::Sprite2D")
        || obj.value()->isInstanceOf("sad::db::custom::Object"))
    {
        list << "flipx";
        list << "flipy";
        list << "options";
    }

    if (obj.value()->isInstanceOf("sad::db::custom::Object"))
    {
        list << "schema";
        sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(obj.value());
        const sad::Hash<sad::String, sad::db::Property*> & sprops =  o->schemaProperties();
        for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = sprops.const_begin();
            it != sprops.const_end();
            ++it)
        {
            list << STD2QSTRING(it.key());
        }
    }

    if (obj.value()->isInstanceOf("sad::p2d::app::Way"))
    {
        list << "totaltime";
        list << "closed";
    }

    scripting::animations::checkPropertiesForAnimations(obj, list, true);
    scripting::instances::checkProperties(obj, list, true);
    scripting::groups::checkProperties(obj, list, true);

    return scripting::FromValue<QStringList>::perform(list, engine);
}

QScriptValue scripting::database::writableProperties(QScriptContext* ctx, QScriptEngine* engine)
{
    if (ctx->argumentCount() != 1)
    {
        ctx->throwError("writableProperties(): accepts only 1 argument");
        return QScriptValue();
    }

    sad::Maybe<sad::db::Object*> obj = scripting::ToValue<sad::db::Object*>::perform(ctx->argument(0));
    if (obj.exists() == false)
    {
        ctx->throwError("writableProperties(): first argument must be sad::db::Object");
        return QScriptValue();
    }

    QStringList list;
    list << "name";

    if (obj.value()->isInstanceOf("sad::SceneNode"))
    {
        list << "visible";
        list << "area";
        list << "angle";
        list << "color";
    }

    if (obj.value()->isInstanceOf("sad::Label")
        || obj.value()->isInstanceOf("sad::db::custom::Object"))
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

    if (obj.value()->isInstanceOf("sad::Sprite2D")
        || obj.value()->isInstanceOf("sad::db::custom::Object"))
    {
        list << "flipx";
        list << "flipy";
        list << "options";
    }

    if (obj.value()->isInstanceOf("sad::db::custom::Object"))
    {
        list << "schema";
        sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(obj.value());
        const sad::Hash<sad::String, sad::db::Property*> & sprops =  o->schemaProperties();
        for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = sprops.const_begin();
            it != sprops.const_end();
            ++it)
        {
            list << STD2QSTRING(it.key());
        }
    }

    if (obj.value()->isInstanceOf("sad::p2d::app::Way"))
    {
        list << "totaltime";
        list << "closed";
    }


    scripting::animations::checkPropertiesForAnimations(obj, list, false);
    scripting::instances::checkProperties(obj, list, false);
    scripting::groups::checkProperties(obj, list, false);

    return scripting::FromValue<QStringList>::perform(list, engine);
}
