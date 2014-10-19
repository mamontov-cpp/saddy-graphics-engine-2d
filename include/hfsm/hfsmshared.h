/*! \file   hfsmshared.h
	\author HiddenSeeker

	Describes a shared data for state of hierarchical finite state machine
 */
#pragma once
#include "../object.h"

namespace sad
{

namespace hfsm
{
/*! Shared data for hierarchical finite state machine
 */
class Shared: public sad::Object
{
SAD_OBJECT
public:
	/*! Creates empty shared data
	 */
	Shared();
	/*! Destroys shared data
	 */
	virtual ~Shared();
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::hfsm::Shared)
