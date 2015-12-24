#include "waysbindings.h"
#include "wayspointref.h"

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../scripting.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../history/ways/waysnew.h"
#include "../../history/ways/wayswaypointnew.h"
#include "../../history/ways/wayswaypointremove.h"


QScriptValue scripting::ways::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("ways", "sad::p2d::app::Way", ctx, engine);
}

unsigned long long scripting::ways::_add(
    scripting::Scripting* scripting, 
    sad::String name,
    double totaltime,
    bool closed,
    sad::Vector<sad::p2d::app::WayPoint> points
)
{
    core::Editor* e = scripting->editor();

    sad::p2d::app::Way* w = new sad::p2d::app::Way();
    w->setObjectName(name);
    w->setTotalTime(totaltime);
    w->setClosed(closed);
    w->setProperty("waypoints", points);

    sad::Renderer::ref()->database("")->table("ways")->add(w);
    history::ways::New* c = new history::ways::New(w);
    c->commit(e);
    e->currentBatchCommand()->add(c);

    return w->MajorId;
}

void scripting::ways::remove(scripting::Scripting* scripting, sad::p2d::app::Way* node)
{
    core::Editor* e = scripting->editor();
    e->actions()->wayActions()->removeWayFromDatabase(node, false);
}

unsigned int scripting::ways::length(scripting::Scripting*, sad::p2d::app::Way* way)
{
    return way->wayPoints().count();
}

void scripting::ways::addPoint(scripting::Scripting* scripting, sad::p2d::app::Way* way, sad::Point2D point)
{
    history::ways::WayPointNew* command = new history::ways::WayPointNew(way);
    command->setPoint(point);

    core::Editor* e = scripting->editor();
    command->commit(e);
    e->currentBatchCommand()->add(command);
}

bool scripting::ways::removePoint(scripting::Scripting* scripting, sad::p2d::app::Way* way, unsigned int pos)
{
    if (pos >= way->wayPoints().count())
    {
        return false;
    }

    history::ways::WayPointRemove* command = new history::ways::WayPointRemove(way, pos);

    core::Editor* e = scripting->editor();
    command->commit(e);
    e->currentBatchCommand()->add(command);
    return true;
}

QScriptValue scripting::ways::point(scripting::Scripting* scripting, sad::p2d::app::Way* way, unsigned int pos)
{
    if (pos >= way->wayPoints().count())
    {
        scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, "point: position is out of way");
        return scripting->engine()->undefinedValue();
    }

    return scripting->engine()->newQObject(new scripting::ways::PointRef(way, pos));
}
