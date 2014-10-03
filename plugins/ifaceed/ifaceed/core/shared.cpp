#include "core/shared.h"

core::Shared::Shared() : 
m_selected(NULL),
m_active(NULL),
m_editor(NULL)
{
	m_show_active_border = true;
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

void core::Shared::setPivotPoint(const sad::Point2D& p)
{
	m_pivot_point = p;
}

const sad::Point2D& core::Shared::pivotPoint() const
{
	return m_pivot_point;
}

void core::Shared::setOldArea(const sad::Rect2D& r)
{
	m_old_area = r;
}

const sad::Rect2D& core::Shared::oldArea() const
{
	return m_old_area;
}
