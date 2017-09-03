#include "queryresource.h"
#include "scripting.h"
#include "fromvalue.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

#include <sprite2d.h>

#include <db/dbproperty.h>

#include <db/custom/customschema.h>

#include "../qstdstring.h"

QString scripting::resource_type(QString name)
{
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* resource = tree->root()->resource(name.toStdString());
    QString result;
    if (resource)
    {
        result = resource->metaData()->name().c_str();
    }
    return result;
}

QScriptValue scripting::resource_options(scripting::Scripting* scripting, QString name)
{
    QScriptEngine* e = scripting->engine();
    QScriptValue result = e->nullValue();

    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* resource = tree->root()->resource(name.toStdString());

    if (resource)
    {
        if (resource->metaData()->canBeCastedTo("sad::Sprite2D::Options"))
        {
            sad::Sprite2D::Options* o = sad::checked_cast<sad::Sprite2D::Options>(resource);

            result = scripting->engine()->newObject();
            result.setProperty("Texture",
                scripting::FromValue<sad::String>::perform(o->Texture, e)
            );
            result.setProperty("TextureRectangle",
                scripting::FromValue<sad::Rect2D>::perform(o->TextureRectangle, e)
            );
            result.setProperty("Rectangle",
                scripting::FromValue<sad::Rect2D>::perform(o->Rectangle, e)
            );
            result.setProperty("Transparent",
                scripting::FromValue<bool>::perform(o->Transparent, e)
            );
            result.setProperty("TransparentColor",
                scripting::FromValue<sad::Color>::perform(o->TransparentColor, e)
            );
        }
    }

    return result;
}

QScriptValue scripting::resource_schema(scripting::Scripting* scripting, QString name)
{
    QScriptEngine* e = scripting->engine();
    QScriptValue result = e->nullValue();

    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* resource = tree->root()->resource(name.toStdString());

    if (resource)
    {
        if (resource->metaData()->canBeCastedTo("sad::db::custom::Schema"))
        {
            sad::db::custom::Schema* o = sad::checked_cast<sad::db::custom::Schema>(resource);

            result = scripting->engine()->newObject();
            result.setProperty("TreeItemName",
                scripting::FromValue<sad::String>::perform(o->treeItemName(), e)
            );

            QScriptValue resultprops = e->newObject();

            sad::Hash<sad::String, sad::db::Property*> props;
            o->getCustomProperties(props);
            for(sad::Hash<sad::String, sad::db::Property*>::iterator it = props.begin();
                it != props.end();
                ++it)
            {
                QString proptype = STD2QSTRING(it.value()->baseType());
                for(size_t i = 0; i < it.value()->pointerStarsCount(); i++)
                {
                    proptype += "*";
                }
                resultprops.setProperty(STD2QSTRING(it.key()), proptype);
            }

            result.setProperty("Properties", resultprops);
        }
    }

    return result;
}
