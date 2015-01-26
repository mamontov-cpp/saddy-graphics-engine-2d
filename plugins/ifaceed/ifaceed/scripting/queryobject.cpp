#include "queryobject.h"
#include "tovalue.h"

#include <renderer.h>

#include <db/dbdatabase.h>

sad::db::Object* scripting::query_object(const QScriptValue& v)
{
	sad::Maybe<sad::String> maybename = scripting::ToValue<sad::String>::perform(v);
	
	sad::db::Object* result = NULL;
	if (maybename.exists())
	{
		sad::Vector<sad::db::Object*> vector = sad::Renderer::ref()->database("")->queryByName(maybename.value());
		if (vector.size())
		{
			result = vector[0];
		}
	}

	if (result != NULL)
	{
		sad::Maybe<unsigned long long> maybemajorid = scripting::ToValue<unsigned long long>::perform(v);
		if (maybemajorid.exists())
		{
			result = sad::Renderer::ref()->database("")->queryByMajorId(maybemajorid.value());
		}
	}

	return result;
}
