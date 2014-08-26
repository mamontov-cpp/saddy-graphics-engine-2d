#include "ifacestate.h"

#include "../ifaceeditor.h"

#include "../../core/editorbehaviour.h"
#include "../../core/editorbehaviourshareddata.h"

IFaceEditor * IFaceState::editor()
{
	return static_cast<IFaceEditor *>(this->behaviour()->parent());
}

IFaceSharedData * IFaceState::shdata()
{
	return this->editor()->shdata();
}

