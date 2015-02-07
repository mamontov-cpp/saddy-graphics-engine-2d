#include "animationsbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/animationactions.h"


QScriptValue scripting::animations::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("animations", "sad::animations::Animation", ctx, engine);
}
