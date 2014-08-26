/*! \file ifacestate.h
	\author HiddenSeeker

	A special state, that knows that it's from core::Editor
 */
#include "../../core/editorbehaviourstate.h"
#pragma once

namespace core
{
class Editor;
}

class IFaceSharedData;
class EditorBehaviourSharedData;

class IFaceState: public core::EditorBehaviourState
{
 public:
	/*! Returns an editor for state
	 */
	core::Editor * editor();
	/*! Returns a shared data for state
	 */
	IFaceSharedData * shdata();

};

