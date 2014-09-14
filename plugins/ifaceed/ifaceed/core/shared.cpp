#include "core/shared.h"

core::Shared::Shared() : 
m_selected(NULL),
m_active(NULL),
m_editor(NULL)
{
	m_show_active_border = true;
	m_rotation_timer = NULL;
	m_rotation_command_pending = false;
}

core::Shared::~Shared()
{
	
}

void core::Shared::setEditor(core::Editor* e)
{
	m_editor = e;
}

core::Editor* core::Shared::editor() const
{
	return m_editor;
}

void core::Shared::setSelectedObject(sad::SceneNode* o)
{
	m_selected = o;
}

void core::Shared::setActiveObject(sad::SceneNode* o)
{
	m_active = o;
}

sad::SceneNode* core::Shared::selectedObject() const
{
	return m_selected;
}

sad::SceneNode* core::Shared::activeObject() const
{
	return m_active;
}


bool core::Shared::canShowActiveBorder() const
{
	return m_show_active_border  && m_active != NULL;
}

void core::Shared::toggleActiveBorder(bool flag) 
{
	m_show_active_border = flag;
}