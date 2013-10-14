/*! \file ifacestate.h
	\author HiddenSeeker

	A special state, that knows that it's from IFaceEditor
 */
#include "../../editorcore/editorbehaviourstate.h"
#pragma once


class IFaceEditor;
class IFaceSharedData;
class EditorBehaviourSharedData;

class IFaceState: public EditorBehaviourState
{
 public:
	/*! Returns an editor for state
	 */
	IFaceEditor * editor();
	/*! Returns a shared data for state
	 */
	IFaceSharedData * shdata();

};

