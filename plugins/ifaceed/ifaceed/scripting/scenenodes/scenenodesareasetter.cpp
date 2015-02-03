#include "scenenodesareasetter.h"

#include "../tovalue.h"

#include <geometry2d.h>


scripting::scenenodes::AreaSetter::AreaSetter(QScriptEngine* e)
: scripting::scenenodes::Setter<sad::Rect2D, history::scenenodes::ChangeArea>(e, "area")
{

}

scripting::scenenodes::AreaSetter::~AreaSetter()
{

}

sad::Maybe<QString> scripting::scenenodes::AreaSetter::canBeCalled(QScriptContext* ctx)
{
   sad::Maybe<QString> result = this->scripting::AbstractSetter<sad::SceneNode*, sad::Rect2D>::canBeCalled(ctx);
   if (result.exists() == false)
   {
        sad::Maybe<sad::Rect2D> r = scripting::ToValue<sad::Rect2D>::perform(ctx->argument(2));
        if (sad::isAABB(r.value()) == false)
        {
            result.setValue("Rectangle must be axis-aligned");
        }
   }
   return result;
}
