/*! \file databasebindings.h
	\author HiddenSeeker

	Describes bindings for editing database properties
 */
#pragma once
#include <sadstring.h>


namespace scripting
{

class Scripting;

/*! Adds new property
	\param[in] s scripting part
	\param[in] type a type
	\param[in] name name of property
 */
bool addProperty(scripting::Scripting* s, sad::String type, sad::String name);

}

