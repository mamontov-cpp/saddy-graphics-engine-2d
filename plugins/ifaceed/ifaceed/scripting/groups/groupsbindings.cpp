#include "groupsbindings.h"

#include "../querytable.h"
#include "../queryobject.h"
#include "../tovalue.h"
#include "../fromvalue.h"
#include "../scripting.h"

#include "../../mainpanel.h"
#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/groupactions.h"

#include "../../history/groups/groupsnew.h"
#include "../../history/groups/groupsaddinstance.h"
#include "../../history/groups/groupsremoveinstance.h"


Q_DECLARE_METATYPE(sad::animations::Group*);
Q_DECLARE_METATYPE(sad::animations::Instance*);

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

bool scripting::groups::addInstance(
    scripting::Scripting* scripting,
    sad::animations::Group* group,	
	sad::animations::Instance* minstance
)
{
	
	bool alreadyinlist = false;
	int groupposition;
	int instanceposition = -1;
	bool result = false;

	MainPanel* panel = scripting->panel();
	groupposition =  panel->findInList<sad::animations::Group*>(panel->UI()->lstAnimationsGroup, group);
	if (groupposition > -1)
	{
		sad::Vector<unsigned long long> ids = group->instances();
		QListWidget* list = panel->UI()->lstAnimationInstances;
		
		if (std::find(ids.begin(), ids.end(), minstance->MajorId) != ids.end())
		{
			alreadyinlist = true;
		}
		if (!alreadyinlist)
		{
			bool positionfound = false;
			for(int i = 0; i < list->count() && !positionfound; i++)
			{
				sad::animations::Instance* instance = list->item(i)->data(Qt::UserRole).value<sad::animations::Instance*>();

				if (std::find(ids.begin(), ids.end(), instance->MajorId) == ids.end())
				{
					++instanceposition;
				}

				if (instance == minstance)
				{
					positionfound = true;
				}
			}

			if (positionfound)
			{
				result = true;

				history::groups::AddInstance* c = new history::groups::AddInstance(group, minstance, instanceposition);
				c->commit(panel->editor());

				panel->editor()->currentBatchCommand()->add(c);
			}
		}
	}

	return !alreadyinlist && result;
}

bool scripting::groups::removeInstance(
    scripting::Scripting* scripting,
    sad::animations::Group* group,	
	unsigned int pos
)
{
	sad::Vector<unsigned long long> instances = group->instances();
	bool result = false;
	if (pos < instances.size())
	{
		unsigned long long id = instances[pos];
		sad::db::Object* d = sad::Renderer::ref()->database("")->queryByMajorId(id);
		if (d)
		{
			if (d->isInstanceOf("sad::animations::Instance"))
			{
				sad::animations::Instance* i = static_cast<sad::animations::Instance*>(d);
				result = true;

				MainPanel* panel = scripting->panel();
				core::Editor* editor = panel->editor();

				history::groups::RemoveInstance* c = new history::groups::RemoveInstance(group, i, pos);
				c->commit(editor);

				editor->currentBatchCommand()->add(c);				
			}
		}
	}
	return result;
}

void scripting::groups::checkProperties(
	const sad::Maybe<sad::db::Object*>& obj,
	QStringList& list,
	bool readable
)
{
	if (obj.value()->isInstanceOf("sad::animations::Group"))
	{
		list << "looped";
		if (readable)
		{
			list << "instances";
		}
	}
}
