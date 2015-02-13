#include "groupsbindings.h"

#include "../querytable.h"
#include "../queryobject.h"
#include "../tovalue.h"
#include "../fromvalue.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/groupactions.h"

#include "../../history/groups/groupsnew.h"

Q_DECLARE_METATYPE(sad::animations::Group*);

QScriptValue scripting::groups::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("animationgroups", "sad::animations::Group", ctx, engine);
}


unsigned long long scripting::groups::_add(
	scripting::Scripting* scripting, 
	sad::String name,
	bool looped
)
{
	MainPanel* panel = scripting->panel();

	sad::animations::Group* d = new sad::animations::Group();
    d->setObjectName(name);
	d->setLooped(looped);


	sad::Renderer::ref()->database("")->table("animationgroups")->add(d);
    history::groups::New* c = new history::groups::New(d);
    c->commit(panel->editor());
    panel->editor()->currentBatchCommand()->add(c);

	return d->MajorId;
}


void scripting::groups::remove(
    scripting::Scripting* scripting,
    sad::animations::Group* group
)
{
	scripting->panel()->groupActions()->removeFromDatabase(group, false);
}

unsigned int scripting::groups::length(
    scripting::Scripting*,
    sad::animations::Group* group	
)
{
	return group->instances().size();
}

unsigned long long scripting::groups::entry(
    scripting::Scripting*,
    sad::animations::Group* group,	
	unsigned int pos
)
{
	sad::Vector<unsigned long long> v = group->instances();
	unsigned long long result = 0;
	if (pos < v.size())
	{
		result = v[pos];
	}
	return result;
}

