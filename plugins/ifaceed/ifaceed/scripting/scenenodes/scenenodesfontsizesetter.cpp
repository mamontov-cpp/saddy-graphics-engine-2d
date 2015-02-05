#include "scenenodesfontsizesetter.h"

#include "../tovalue.h"

#include <geometry2d.h>


scripting::scenenodes::FontSizeSetter::FontSizeSetter(QScriptEngine* e)
: scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>(e, "fontsize")
{

}

scripting::scenenodes::FontSizeSetter::~FontSizeSetter()
{

}

sad::Maybe<QString> scripting::scenenodes::FontSizeSetter::canBeCalled(QScriptContext* ctx)
{
   sad::Maybe<QString> result = this->scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>::canBeCalled(ctx);
   if (result.exists() == false)
   {
        sad::Maybe<unsigned int> r = scripting::ToValue<unsigned int>::perform(ctx->argument(2));
        if (r.value() == 0)
        {
            result.setValue("Value must be greater than zero");
        }
   }
   return result;
}
