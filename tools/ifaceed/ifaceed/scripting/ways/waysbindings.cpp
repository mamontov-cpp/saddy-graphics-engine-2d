#include "waysbindings.h"
#include "wayspointref.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../scripting.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../history/ways/waysnew.h"
#include "../../history/ways/wayswaypointnew.h"
#include "../../history/ways/wayswaypointremove.h"


QVector<unsigned long long>  scripting::ways::list()
{
    return scripting::query_table("ways", "sad::p2d::app::Way");
}

unsigned long long scripting::ways::_add(
    scripting::Scripting* scripting, 
    sad::String name,
    double totaltime,
    bool closed,
    QVector<sad::Point2D> points
)
{
    core::Editor* e = scripting->editor();

    sad::p2d::app::Way* w = new sad::p2d::app::Way();
    w->setObjectName(name);
    w->setTotalTime(totaltime);
    w->setClosed(closed);
    for(size_t i = 0; i < points.size(); i++)
    {
        w->addPoint(points[i]);
    }

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

dukpp03::Maybe<scripting::ways::PointRef*> scripting::ways::point(scripting::Scripting* scripting, sad::p2d::app::Way* way, unsigned int pos)
{
    dukpp03::Maybe<scripting::ways::PointRef*> result;
    if (pos >= way->wayPoints().count())
    {
        scripting->context()->throwError("point: position is out of way");
        throw dukpp03::ArgumentException();
    }

    result.setValue(new scripting::ways::PointRef(scripting, way, pos));
    return result;
}
