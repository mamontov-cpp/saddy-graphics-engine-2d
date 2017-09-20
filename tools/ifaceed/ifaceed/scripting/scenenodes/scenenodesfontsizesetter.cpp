/*#include "scenenodesfontsizesetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../tovalue.h"

scripting::scenenodes::FontSizeSetter::FontSizeSetter(QScriptEngine* e)
: scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>(e, "fontsize")
{

}

scripting::scenenodes::FontSizeSetter::~FontSizeSetter()
{

}

scripting::MatchResult  scripting::scenenodes::FontSizeSetter::canBeCalled(QScriptContext* ctx)
{
   scripting::MatchResult  result = this->scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>::canBeCalled(ctx);
   if (result._2().exists() == false)
   {
        sad::Maybe<unsigned int> r = scripting::ToValue<unsigned int>::perform(ctx->argument(2));
        if (r.value() == 0)
        {
            result._2().setValue("Value must be greater than zero");
        }
   }
   return result;
}
*/
