#include "ifacestate.h"

#include "../../core/editor.h"
#include "../../core/editorbehaviour.h"
#include "../../core/editorbehaviourshareddata.h"

core::Editor * IFaceState::editor()
{
	return static_cast<core::Editor *>(this->behaviour()->parent());
}

IFaceSharedData * IFaceState::shdata()
{
	return this->editor()->shdata();
}

