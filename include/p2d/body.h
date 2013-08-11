/*! \file body.h
	\author HiddenSeeker

    Describes a body object
 */
#include "../primitives/object.h"
#include "../templates/hstring.h"
#pragma once

namespace p2d
{
class World;
/*! Describes a body in physics engine
 */
class Body
{
public:
	/*! Returns an inner user-defined object
		\return inner user-defined object
	 */
	virtual sad::Object * userObject() const;
	/*! Returns a type of user-defined object
		\return type of object
	 */
	virtual const hst::string & userType() const;
	virtual ~Body();
};

}

