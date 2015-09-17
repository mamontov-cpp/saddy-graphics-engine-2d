#include "scenenodesschemasetter.h"

#include "../tovalue.h"

#include "../../history/customobject/customobjectchangeschema.h"

#include "../../qstdstring.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

scripting::scenenodes::SchemaSetter::SchemaSetter(QScriptEngine* e)
    : scripting::scenenodes::AbstractSetter<sad::String>(e)
{
    addMatched("schema");
}

scripting::scenenodes::SchemaSetter::~SchemaSetter()
{

}

scripting::MatchResult  scripting::scenenodes::SchemaSetter::canBeCalled(QScriptContext* ctx)
{
   scripting::MatchResult result = this->scripting::scenenodes::AbstractSetter<sad::String>::canBeCalled(ctx);
   if (result._2().exists() == false)
   {
        sad::Maybe<sad::String> r = scripting::ToValue<sad::String>::perform(ctx->argument(2));
        sad::String resourcename = r.value();
        sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resourcename);
        bool valid = false;
        if (resource) {
            if (resource->metaData()->canBeCastedTo("sad::db::custom::Schema"))
            {
                valid = true;
            }
        }
        if (!valid)
        {
            result._2().setValue(STD2QSTRING(resourcename) + " is not a schema resource");
        }
   }
    if (result._2().exists() == false)
    {
         sad::Maybe<sad::SceneNode*> r = scripting::ToValue<sad::SceneNode*>::perform(ctx->argument(0));
         sad::SceneNode* node = r.value();
         if (node->metaData()->canBeCastedTo("sad::db::custom::Object") == false)
         {
             result._2().setValue("first argument is not a custom object");
         }
    }
   return result;
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