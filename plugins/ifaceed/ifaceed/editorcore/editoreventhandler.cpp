#include "editor.h"
#include "editorbehaviour.h"
#include "editoreventhandler.h"

EditorEventHandler::EditorEventHandler(Editor * ed, void (EditorBehaviour::*cb)(const sad::Event & ev))
{
	m_editor = ed;
	m_cb = cb;
}

EditorEventHandler::~EditorEventHandler()
{

}


bool EditorEventHandler::empty()
{
	return false;
}

void EditorEventHandler::operator()(const sad::Event & o)
{
	if (m_editor)
		m_editor->postBehaviourCallback(m_cb,o);
}
