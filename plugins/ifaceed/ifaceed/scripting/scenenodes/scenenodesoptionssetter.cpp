#include "scenenodesoptionssetter.h"

#include "../tovalue.h"

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

sad::Maybe<QString> scripting::scenenodes::OptionsSetter::canBeCalled(QScriptContext* ctx)
{
   sad::Maybe<QString> result = this->scripting::scenenodes::AbstractSetter<sad::String>::canBeCalled(ctx);
   if (result.exists() == false)
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
            result.setValue(QString(resourcename.data()) + " is not an options resource");
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
