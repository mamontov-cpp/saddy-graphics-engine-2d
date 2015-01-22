/*! \file abstractsetter.h
	\author HiddenSeeker

	Defines a basic setter for property
 */
#pragma once
#include "maybe.h"
#include "callable.h"

namespace scripting
{
	
template<
	typename _Type,
	typename _PropetyType
>
class AbstractSetter: public scripting::Callable
{
public:

protected:
	/*! A matched properties list. If empty - every kind of property is matched
	 */
	sad::Vector<sad::String> m_matched_property_names;
	/*! A property names, that should be excluded from this name
	 */
	sad::Vector<sad::String> m_excluded_property_names;
};

}
