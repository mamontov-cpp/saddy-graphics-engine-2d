/*! \file cobuilder.h
	\author HiddenSeeker

	Describes a builder, that builds a command for setting a value for a custom object property
 */
#pragma once
#include "../history/customobject/customobjectchangeproperty.h"

#include <QScriptEngine>

#include "abstractcobuilder.h"

namespace scripting
{
	
template<
	typename T
>
class ToValue
{
	
};

}