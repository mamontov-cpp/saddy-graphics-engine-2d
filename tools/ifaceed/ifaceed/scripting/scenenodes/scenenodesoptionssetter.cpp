#include "scenenodesoptionssetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../../qstdstring.h"

#include "../../history/sprite2d//changeoptions.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

scripting::scenenodes::OptionsSetter::OptionsSetter(scripting::Scripting* scripting)
    : scripting::scenenodes::AbstractSetter<sad::String>(scripting, "options")
{

    std::function<dukpp03::Maybe<sad::String>(const sad::String&)> is_options_exists = [](const sad::String& resource_name) {
        dukpp03::Maybe<sad::String> result;
        sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resource_name);
        bool valid = false;
        if (resource)
        {
            if (resource->metaData()->canBeCastedTo("sad::Sprite2D::Options"))
            {
                valid = true;
            }
        }
        if (!valid)
        {
            result.setValue(resource_name + " is not an options resource");
        }
        return result;
    };
    addCondition(is_options_exists);
}

dukpp03::qt::Callable* scripting::scenenodes::OptionsSetter::clone()
{
    return new scripting::scenenodes::OptionsSetter(*this);
}

scripting::scenenodes::OptionsSetter::~OptionsSetter()
{

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

