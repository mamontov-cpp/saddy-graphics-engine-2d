#include "waysbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"


QScriptValue scripting::ways::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("ways", "sad::p2d::app::Way", ctx, engine);
}
