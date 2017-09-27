#include "queryresource.h"
#include "scripting.h"

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

dukpp03::Maybe<QHash<QString, QVariant> > scripting::resource_options(scripting::Scripting* scripting, QString name)
{
    dukpp03::Maybe<QHash<QString, QVariant> > result;

    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* resource = tree->root()->resource(name.toStdString());

    if (resource)
    {
        if (resource->metaData()->canBeCastedTo("sad::Sprite2D::Options"))
        {
            QHash<QString, QVariant> props;
            sad::Sprite2D::Options* o = sad::checked_cast<sad::Sprite2D::Options>(resource);

            props.insert("Texture", QString(o->Texture.c_str()));
            
            QVariant tr;
            tr.setValue(o->TextureRectangle);
            props.insert("TextureRectangle", tr);

            QVariant r;
            r.setValue(o->Rectangle);
            props.insert("Rectangle", r);

            props.insert("Transparent", o->Transparent);
            
            QVariant tc;
            tc.setValue(o->TransparentColor);
            props.insert("TransparentColor", tc);

            result.setValue(props);
        }
    }

    return result;
}

dukpp03::qt::JSObject* scripting::resource_schema(scripting::Scripting* scripting, QString name)
{
    dukpp03::qt::JSObject* result = NULL;

    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* resource = tree->root()->resource(name.toStdString());

    if (resource)
    {
        if (resource->metaData()->canBeCastedTo("sad::db::custom::Schema"))
        {
            result = new dukpp03::qt::JSObject();
            
            sad::db::custom::Schema* o = sad::checked_cast<sad::db::custom::Schema>(resource);

            result->setProperty("TreeItemName", o->treeItemName().c_str());

            dukpp03::qt::JSObject* properties = new dukpp03::qt::JSObject();


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
                properties->setProperty(it.key(), proptype);
            }

            result->setProperty("Properties", properties);
        }
    }

    return result;
}
