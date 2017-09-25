#include "waysbindings.h"

#include "wayspointref.h"
#include "wayssetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../scripting.h"

#include "../abstractgetter.h"
#include "../abstractsetter.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../history/ways/waysnew.h"
#include "../../history/ways/wayswaypointnew.h"
#include "../../history/ways/wayswaypointremove.h"
#include "../../history/ways/wayschangename.h"
#include "../../history/ways/wayschangetotaltime.h"
#include "../../history/ways/wayschangeclosed.h"

Q_DECLARE_METATYPE(scripting::ways::PointRef*)
Q_DECLARE_METATYPE(QVector<sad::Point2D>)

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

dukpp03::qt::JSObject* scripting::ways::init(scripting::Scripting* s, dukpp03::qt::JSObject* e)
{
    dukpp03::qt::registerMetaType<scripting::ways::PointRef>();

    dukpp03::qt::JSObject* ways = new dukpp03::qt::JSObject();
    ways->setProperty("list", dukpp03::qt::make_function::from(scripting::ways::list)); // E.ways.list
    ways->setProperty("_add", dukpp03::qt::curried1::from(s, scripting::ways::_add)); // E.ways._add
    ways->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::ways::remove)); // E.ways.remove
    ways->setProperty("length", dukpp03::qt::curried1::from(s, scripting::ways::length)); // E.ways.length
    ways->setProperty("addPoint", dukpp03::qt::curried1::from(s, scripting::ways::addPoint)); // E.ways.addPoint
    ways->setProperty("removePoint", dukpp03::qt::curried1::from(s, scripting::ways::removePoint)); // E.ways.removePoint
    ways->setProperty("point", dukpp03::qt::curried1::from(s, scripting::ways::point)); // E.ways.point

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();
    {
        set->add(new scripting::ways::Setter<sad::String, history::ways::ChangeName>(s, "name"));
        set->add(new scripting::ways::Setter<double, history::ways::ChangeTotalTime>(s, "totaltime"));
        set->add(new scripting::ways::Setter<bool, history::ways::ChangeClosed>(s, "closed"));
    }
    ways->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.ways.set

    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    {
        get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, sad::String>("name"));
        get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, unsigned long long>("majorid"));
        get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, unsigned long long>("minorid"));
        get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, double>("totaltime"));
        get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, bool>("closed"));
    }
    ways->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.ways.get

    {
        dukpp03::qt::ClassBinding* binding = new dukpp03::qt::ClassBinding();
        binding->addMethod("toPoint", dukpp03::qt::bind_method::from(&scripting::ways::PointRef::toPoint));
        binding->registerMetaObject<scripting::ways::PointRef>();
        s->context()->addClassBinding("scripting::ways::PointRef", binding);
    }

    e->setProperty("ways", ways);

    bool b = s->context()->eval(
        "E.ways.add = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"totaltime\" in o == false)   "
        "   {"
        "      o[\"totaltime\"] = 0;       "
        "   }"
        "   if (\"closed\" in o == false)  "
        "   {"
        "      o[\"closed\"] = false;      "
        "   }"
        "   if (\"points\" in o == false)  "
        "   {"
        "      o[\"points\"] = [];         "
        "   }"
        "   return E.ways._add(o[\"name\"], o[\"totaltime\"], o[\"closed\"], o[\"points\"]);"
        "};"
        "E.ways.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.ways.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.ways.set(arguments[0], arguments[1], arguments[2]); return E.ways;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert(b);

    return ways;
}
