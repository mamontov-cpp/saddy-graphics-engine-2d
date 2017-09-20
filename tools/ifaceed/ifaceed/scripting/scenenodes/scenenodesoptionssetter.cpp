/*#include "scenenodesoptionssetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../tovalue.h"

#include "../../qstdstring.h"

#include "../../history/sprite2d//changeoptions.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

scripting::scenenodes::OptionsSetter::OptionsSetter(QScriptEngine* e)
    : scripting::scenenodes::AbstractSetter<sad::String>(e)
{
    addMatched("options");
}

scripting::scenenodes::OptionsSetter::~OptionsSetter()
{

}

scripting::MatchResult  scripting::scenenodes::OptionsSetter::canBeCalled(QScriptContext* ctx)
{
   scripting::MatchResult  result = this->scripting::scenenodes::AbstractSetter<sad::String>::canBeCalled(ctx);
   if (result._2().exists() == false)
   {
        sad::Maybe<sad::String> r = scripting::ToValue<sad::String>::perform(ctx->argument(2));
        sad::String resourcename = r.value();
        sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resourcename);
        bool valid = false;
        if (resource) {
            if (resource->metaData()->canBeCastedTo("sad::Sprite2D::Options"))
            {
                valid = true;
            }
        }
        if (!valid)
        {
            result._2().setValue(STD2QSTRING(resourcename) + " is not an options resource");
        }
   }
   return result;
}

history::Command* scripting::scenenodes::OptionsSetter::command(
    sad::SceneNode* obj, 
    const sad::String&, 
    sad::String oldvalue,  
    sad::String newvalue
)
{
    sad::Maybe<sad::Rect2D> area = obj->getProperty<sad::Rect2D>("area");
    return new history::sprite2d::ChangeOptions(obj, area.value(), oldvalue, newvalue);
}

*/
