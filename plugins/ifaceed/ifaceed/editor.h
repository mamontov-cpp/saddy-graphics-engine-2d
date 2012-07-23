/*! \file editor.h
	\author HiddenSeeker

	Describes a global editor state
 */
#include "objects/abstractgameobject.h"
#pragma once

class EditorState
{
 private:
		 AbstractGameObject * m_selectedobject; //!< Selected game object
};


extern EditorState * editor;
