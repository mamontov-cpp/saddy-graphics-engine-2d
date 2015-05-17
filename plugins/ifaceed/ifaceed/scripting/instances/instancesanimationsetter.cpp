#include "instancesanimationsetter.h"

#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/instances/instanceschangeanimation.h"

Q_DECLARE_METATYPE(sad::animations::Instance*);

// ===================== scripting::instances::AnimationSetter::IsAnimationResource =====================

scripting::instances::AnimationSetter::IsAnimationResource::IsAnimationResource()
{
	
}

scripting::instances::AnimationSetter::IsAnimationResource::~IsAnimationResource()
{
	
}

sad::Maybe<QString> scripting::instances::AnimationSetter::IsAnimationResource::check(const sad::String& a)
{
	sad::Maybe<QString> result;
	if (a.length())
	{
		bool valid = false;
		sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(a);
		if (r)
		{
			valid = r->metaData()->canBeCastedTo("sad::animations::Animation");
		}
		if (!valid)
		{
			QString name = a.c_str();
			result.setValue(name + " : is not a reference to an animation from resources");
		}
	}
	return result;
}

// ===================== scripting::instances::AnimationSetter =====================


scripting::instances::AnimationSetter::AnimationSetter(
		QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Instance*, sad::String>(e, "animation")
{
	m_matched_property_names << "animation";
	this->addCondition(new scripting::instances::AnimationSetter::IsAnimationResource());
}

scripting::instances::AnimationSetter::~AnimationSetter()
{
		
}

void scripting::instances::AnimationSetter::setProperty(
	sad::animations::Instance* obj, 
	const sad::String&, 
	sad::String oldvalue,  
	sad::String newvalue
)
{
	QScriptValue main = this->engine()->globalObject().property("---");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	MainPanel* panel = e->panel();
	core::Editor* editor =  panel->editor();

	int row = panel->findInList<sad::animations::Instance*>(panel->UI()->lstAnimationInstances, obj);
	unsigned long long majorid = obj->getProperty<unsigned long long>("animationmajorid").value();
	if (row > - 1)
	{
		history::Command* c  = NULL;
		QRadioButton* treebutton = panel->UI()->rbAnimationInstanceFromTree;
		QRadioButton* dbbutton = panel->UI()->rbAnimationInstanceFromDatabase;
		if (newvalue.length() == 0)
		{
			c = new history::instances::ChangeAnimation(
				obj,
				dbbutton,
				treebutton,
				oldvalue,
				majorid,
				newvalue,
				majorid
			);
			
		}
		else
		{
			if (majorid > 0)
			{
				c = new history::instances::ChangeAnimation(
					obj,
					treebutton,
					dbbutton,
					oldvalue,
					majorid,
					newvalue,
					majorid
				);
			}
			else
			{
				c = new history::instances::ChangeAnimation(
					obj,
					oldvalue,
					newvalue
				);
			}
		}
		editor->currentBatchCommand()->add(c);
		c->commit(editor);
	}
}
