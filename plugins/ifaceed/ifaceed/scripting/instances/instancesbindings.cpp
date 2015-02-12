#include "instancesbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/instanceactions.h"

QScriptValue scripting::instances::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("animationinstances", "sad::animations::Instance", ctx, engine);
}
