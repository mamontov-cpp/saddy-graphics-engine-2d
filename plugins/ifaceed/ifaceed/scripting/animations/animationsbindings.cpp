#include "animationsbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"
#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/animationactions.h"

#include "../../history/animations/animationsnew.h"
#include "../../history/animations/animationsswapincomposite.h"

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

	QString	animationtypename = QString("sad::animations::") + STD2QSTRING(type);

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

int scripting::animations::compositeLength(
	scripting::Scripting*, 
	sad::animations::Composite* list
)
{
	return list->animationMajorIds().size();
}

unsigned long long scripting::animations::getAnimation(
	scripting::Scripting*, 
	sad::animations::Composite* list,
	unsigned int pos
)
{
	sad::Vector<unsigned long long> v = list->animationMajorIds();
	unsigned long long result = 0;
	if (pos < v.size())
	{
		result = v[pos];
	}
	return result;
}


bool scripting::animations::moveBackInCompositeList(
	scripting::Scripting* scripting, 
	sad::animations::Composite* list,
	unsigned int pos
)
{
	sad::Vector<unsigned long long> v = list->animationMajorIds();
	if (pos < v.count() - 1)
	{
		core::Editor* e = scripting->panel()->editor();
		history::Command* c = new history::animations::SwapInComposite(list, pos, pos + 1);
		c->commit(e);
		e->currentBatchCommand()->add(c);
		return true;
	}
	return false;
}

bool scripting::animations::moveFrontInCompositeList(
	scripting::Scripting* scripting, 
	sad::animations::Composite* list,
	unsigned int pos
)
{
	sad::Vector<unsigned long long> v = list->animationMajorIds();
	if (pos > 0 && pos < v.count() - 1)
	{
		core::Editor* e = scripting->panel()->editor();
		history::Command* c = new history::animations::SwapInComposite(list, pos - 1, pos);
		c->commit(e);
		e->currentBatchCommand()->add(c);
		return true;
	}
	return false;
}


void scripting::animations::checkPropertiesForAnimations(
	const sad::Maybe<sad::db::Object*>& obj,
	QStringList& list,
	bool readable
)
{
	if (obj.value()->isInstanceOf("sad::animations::Animation"))
	{
		list << "time";
		list << "looped";
	}

	if (obj.value()->isInstanceOf("sad::animations::Blinking"))
	{
		list << "frequency";
	}

	if (obj.value()->isInstanceOf("sad::animations::CameraShaking"))
	{
		list << "offset";
		list << "frequency";
	}

	if (obj.value()->isInstanceOf("sad::animations::CameraRotation"))
	{
		list << "pivot";
		list << "min_angle";
		list << "max_angle";
	}

	if (obj.value()->isInstanceOf("sad::animations::Color"))
	{
		list << "min_color";
		list << "max_color";
	}

	if (obj.value()->isInstanceOf("sad::animations::FontList"))
	{
		list << "fonts";
	}

	if (obj.value()->isInstanceOf("sad::animations::FontSize"))
	{
		list << "min_size";
		list << "max_size";
	}

	if (obj.value()->isInstanceOf("sad::animations::Resize"))
	{
		list << "start_size";
        list << "end_size";
	}

	if (obj.value()->isInstanceOf("sad::animations::Rotate"))
	{
		list << "min_angle";
		list << "max_angle";
	}

	if (obj.value()->isInstanceOf("sad::animations::OptionList") 
		|| obj.value()->isInstanceOf("sad::animations::TextureCoordinatesList"))
	{
		list << "list";
	}

	if (obj.value()->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
	{
		list << "start_rect";
		list << "end_rect";
	}

	if (obj.value()->isInstanceOf("sad::animations::WayMoving"))
	{
		list << "way";
	}

	if (readable)
	{
		if (obj.value()->isInstanceOf("sad::animations::Composite"))
		{
			list << "list";
		}	
	}
}
