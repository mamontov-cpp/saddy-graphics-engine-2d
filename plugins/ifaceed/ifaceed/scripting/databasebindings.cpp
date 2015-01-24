#include "scripting.h"
#include "databasebindings.h"
#include "../mainpanel.h"

#include "../core/editor.h"

bool scripting::addProperty(scripting::Scripting* s, sad::String type, sad::String name)
{
	return s->panel()->scriptableAddProperty(type, name, false);	
}

