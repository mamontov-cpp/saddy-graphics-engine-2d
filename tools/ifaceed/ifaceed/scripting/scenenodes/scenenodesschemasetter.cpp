#include "scenenodesschemasetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../history/customobject/customobjectchangeschema.h"

#include "../../qstdstring.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

scripting::scenenodes::SchemaSetter::SchemaSetter(scripting::Scripting* scripting)
    : scripting::scenenodes::AbstractSetter<sad::String>(scripting, "schema")
{
    std::function<
        void(scripting::Scripting*, sad::SceneNode*, const sad::String&, sad::String oldvalue, sad::String newvalue)
    > check_if_custom_object = [](scripting::Scripting* s, sad::SceneNode* obj, const sad::String& propertyname, sad::String oldvalue, sad::String newvalue) {
        if (obj->metaData()->canBeCastedTo("sad::db::custom::Object") == false)
        {
          s->context()->throwError("First argument is not a custom object");
          throw new dukpp03::ArgumentException();
        }
    };
    addAction(check_if_custom_object);

    std::function<dukpp03::Maybe<sad::String>(const sad::String&)> is_schema_exists = [](const sad::String& resource_name) {
        dukpp03::Maybe<sad::String> result;
        sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resource_name);
        bool valid = false;
        if (resource) {
            if (resource->metaData()->canBeCastedTo("sad::db::custom::Schema"))
            {
                valid = true;
            }
        }
        if (!valid)
        {
            result.setValue(resource_name + " is not a schema resource");
        }
        return result;
    };
    addCondition(is_schema_exists);
}


dukpp03::qt::Callable* scripting::scenenodes::SchemaSetter::clone()
{
    return new scripting::scenenodes::SchemaSetter(*this);
}

scripting::scenenodes::SchemaSetter::~SchemaSetter()
{

}

history::Command* scripting::scenenodes::SchemaSetter::command(
    sad::SceneNode* obj,
    const sad::String&,
    sad::String oldvalue,
    sad::String newvalue
)
{
    return new history::customobject::ChangeSchema(
        static_cast<sad::db::custom::Object*>(obj),
        oldvalue,
        newvalue
    );
}
