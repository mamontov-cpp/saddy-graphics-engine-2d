/*! \file scripting/isaabb.h
	\author HiddenSeeker

	Describes a condition, whether rectangle is AABB-box
 */
#pragma once
#include "abstractcondition.h"

#include <sadrect.h>

namespace scripting
{
	
/*! Checks, whether condition defines AABB-box
 */
class IsAABB: public scripting::AbstractCondition<sad::Rect2D>
{
public:
	/*! Constructs a condition
	 */
	IsAABB();
	/*! Destroys an object
	 */
	~IsAABB();
	/*! Checks, whether rectangle is AABB
		\param[in] r rectangle
		\return message if rectangle is not AABB
	 */
	virtual sad::Maybe<QString> check(const sad::Rect2D& r);
};


}
