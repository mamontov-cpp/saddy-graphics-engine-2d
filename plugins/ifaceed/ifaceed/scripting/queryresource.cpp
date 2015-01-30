#include "queryresource.h"
#include "scripting.h"
#include "fromvalue.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

#include <sprite2d.h>

#include <db/custom/customschema.h>


sad::String scripting::resource_type(scripting::Scripting* scripting, sad::String& name)
{
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* resource = tree->root()->resource(name);
    sad::String result = "";
    if (resource)
    {
        result = resource->metaData()->name();
    }
    return result;
}

QScriptValue scripting::resource_options(scripting::Scripting* scripting, sad::String& name)
{
    QScriptEngine* e = scripting->engine();
    QScriptValue result = e->nullValue();

    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* resource = tree->root()->resource(name);

    if (resource)
    {
        if (resource->metaData()->canBeCastedTo("sad::Sprite2D::Options"))
        {
            sad::Spite2D::Options* o = sad::checked_cast<sad::Sprite2D::Options>(resource);

            result = scripting->engine()->newObject();
            result.setProperty("Texture",
                scripting::FromValue<sad::String>::perform(o->Texture, e)
            );
        }
    }

    return result;
}
