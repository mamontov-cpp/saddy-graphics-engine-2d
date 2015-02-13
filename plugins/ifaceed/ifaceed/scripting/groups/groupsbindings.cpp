#include "groupsbindings.h"

#include "../querytable.h"
#include "../queryobject.h"
#include "../tovalue.h"
#include "../fromvalue.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/instanceactions.h"

#include "../../history/instances/instancesnew.h"
#include "../../history/instances/instancesremove.h"

#include <animations/animationswayinstance.h>

Q_DECLARE_METATYPE(sad::animations::Group*);

QScriptValue scripting::groups::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("animationgroups", "sad::animations::Group", ctx, engine);
}
