/*! \file abstractcondition.h
	\author HiddenSeeker

	Defines an abstract condition for fetching conditions
 */
#pragma once
#include <maybe.h>

#include <QString>

namespace scripting
{
	
template<
	typename T
>
class AbstractCondition
{
public:
	/*! Constructs a default abstract condition
	 */
	AbstractCondition()
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~AbstractCondition()
	{
		
	}
	/*! Checks a condition for object
		\param[in] a an object
		\return if doesn't match -  a message
	 */
	virtual sad::Maybe<QString> check(const T& a) = 0;
};

}
