#include "waysbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/wayactions.h"

#include "../../history/ways/waysnew.h"


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
	MainPanel* panel = scripting->panel();

	sad::p2d::app::Way* w = new sad::p2d::app::Way();
    w->setObjectName(name);
	w->setTotalTime(totaltime);
	w->setClosed(closed);
	w->setProperty("waypoints", points);

	sad::Renderer::ref()->database("")->table("ways")->add(w);
    history::ways::New* c = new history::ways::New(w);
    c->commit(panel->editor());
    panel->editor()->history()->add(c);

	return 0;
}

void scripting::ways::remove(scripting::Scripting* scripting, sad::p2d::app::Way* node)
{
	MainPanel* panel = scripting->panel();
	panel->wayActions()->removeWayFromDatabase(node, false);
}
