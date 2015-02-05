#include "scenenodesfontsetter.h"

#include "../tovalue.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

scripting::scenenodes::FontSetter::FontSetter(QScriptEngine* e)
    : scripting::scenenodes::Setter<sad::String, history::label::ChangeFontName>(e, "font")
{

}

scripting::scenenodes::FontSetter::~FontSetter()
{

}

sad::Maybe<QString> scripting::scenenodes::FontSetter::canBeCalled(QScriptContext* ctx)
{
   sad::Maybe<QString> result = this->scripting::scenenodes::Setter<sad::String, history::label::ChangeFontName>::canBeCalled(ctx);
   if (result.exists() == false)
   {
        sad::Maybe<sad::String> r = scripting::ToValue<sad::String>::perform(ctx->argument(2));
        sad::String resourcename = r.value();
        sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resourcename);
        bool valid = false;
        if (resource) {
            if (resource->metaData()->canBeCastedTo("sad::freetype::Font")
                || resource->metaData()->canBeCastedTo("sad::TextureMappedFont")
                || resource->metaData()->canBeCastedTo("sad::Font"))
            {
                valid = true;
            }
        }
        if (!valid)
        {
            result.setValue(QString(resourcename.data()) + " is not a font resource");
        }
   }
   return result;
}

