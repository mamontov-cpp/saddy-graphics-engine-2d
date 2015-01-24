#include "scripting.h"
#include "databasebindings.h"
#include "../mainpanel.h"

#include "../core/editor.h"

#include "../gui/table/delegate.h"

bool scripting::addProperty(scripting::Scripting* s, sad::String type, sad::String name)
{
	return s->panel()->scriptableAddProperty(type, name, false);	
}

bool scripting::removeProperty(scripting::Scripting* s, sad::String name)
{
	MainPanel* panel = s->panel();
	bool result = false;
	if(sad::Renderer::ref()->database("")->propertyByName(name) != NULL)
	{
		if (panel->delegatesByName().contains(name))
		{
			result = true;
			panel->delegatesByName()[name]->removeWithCommand();
		}
	}
	return result;
}
