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
			for(size_t i = 0; i < vector.size() && !result; i++)
			{
				if (vector[i]->Active) {
					result = vector[i];
				}
			}
		}
	}

	if (result != NULL)
	{
		sad::Maybe<unsigned long long> maybemajorid = scripting::ToValue<unsigned long long>::perform(v);
		if (maybemajorid.exists())
		{
			sad::db::Object* object = sad::Renderer::ref()->database("")->queryByMajorId(maybemajorid.value());
			if (object)
			{
				if (object->Active == false)
				{
					object = NULL;
				}
			}
			result = object;
		}
	}

	return result;
}
