#include "animationsbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/animationactions.h"

#include "../../history/animations/animationsnew.h"

QScriptValue scripting::animations::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("animations", "sad::animations::Animation", ctx, engine);
}


unsigned long long scripting::animations::_add(
	scripting::Scripting* scripting, 
	sad::String type,
	sad::String name,
	double time,
	bool looped
)
{
	MainPanel* panel = scripting->panel();

	QString	animationtypename = QString("sad::animations::") + type.c_str();

	sad::animations::Animation* a = panel->editor()->animationFactory()->create(animationtypename.toStdString());
	unsigned long long result  = 0;
	if (a)
	{
		a->setObjectName(name);
		a->setTime(time);
		a->setLooped(looped);

		sad::Renderer::ref()->database("")->table("animations")->add(a);		
		
		history::animations::New* c = new history::animations::New(a);
		c->commit(panel->editor());
		panel->editor()->currentBatchCommand()->add(c);

		result = a->MajorId;
	}
	else
	{
		scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, QString("_add(): cannot created animation of following type - ") + type.c_str());
	}
	return result;
}

void scripting::animations::remove(
	scripting::Scripting* scripting, 
	sad::animations::Animation* a
)
{
	scripting->panel()->animationActions()->removeAnimationFromDatabase(a, false);
}

bool scripting::animations::addToComposite(
	scripting::Scripting* scripting, 
	sad::animations::Composite* list,
	sad::animations::Animation* a
)
{
	return scripting->panel()->animationActions()->addAnimationToCompositeList(list, a, false);
}

bool scripting::animations::removeFromComposite(
	scripting::Scripting* scripting, 
	sad::animations::Composite* list,
	sad::animations::Animation* a
)
{
	return scripting->panel()->animationActions()->removeAnimationFromCompositeList(list, a, false, -1);
}
