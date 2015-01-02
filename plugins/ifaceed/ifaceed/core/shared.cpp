#include "core/shared.h"

#include <db/custom/customobject.h>

core::Shared::Shared() : 
m_selected(NULL),
m_active(NULL),
m_editor(NULL),
m_selected_way(NULL),
m_selected_dialogue(NULL),
m_selected_animation(NULL)
{
	m_show_active_border = true;
	m_nonresizeable_nodetypes << "sad::Label";
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

bool core::Shared::isSelectionResizeable() const
{
	if (m_selected == NULL)
		return false;
	bool result  = true;
	for(size_t i = 0; i < m_nonresizeable_nodetypes.size(); i++)
	{
		if (m_selected->metaData()->canBeCastedTo(m_nonresizeable_nodetypes[i]))
		{
			result = false;
		}
	}
	if (m_selected->metaData()->canBeCastedTo("sad::db::custom::Object"))
	{
		sad::db::custom::Object* o  = static_cast<sad::db::custom::Object*>(m_selected);
		result = o->innerTypeIs("sad::Label") == false;
	}
	return result;
}

void core::Shared::setResizingIndexes(const sad::Pair<int, int>& indexes)
{
	m_resizing_indexes = indexes;
}

const sad::Pair<int, int>& core::Shared::resizingIndexes() const
{
	return m_resizing_indexes;
}

void core::Shared::setResizingDirection(const sad::p2d::Vector& direction)
{
	m_resizing_direction = direction;
}

const sad::p2d::Vector& core::Shared::resizingDirection() const
{
	return m_resizing_direction;
}

void core::Shared::setNormalizedResizingDirection(const sad::p2d::Vector& direction)
{
	m_normalized_resizing_direction = direction;
}

const sad::p2d::Vector& core::Shared::normalizedResizingDirection() const
{
	return m_normalized_resizing_direction;
}

void core::Shared::setFileName(const QString& name)
{
	m_filename = name;
}

const QString& core::Shared::fileName() const
{
	return m_filename;
}

void core::Shared::setOldRegion(const sad::Rect2D& r)
{
	m_old_region = r;
}

const sad::Rect2D&  core::Shared::oldRegion() const
{
	return m_old_region;
}


void core::Shared::setSelectedWay(sad::p2d::app::Way* way)
{
	m_selected_way = way;
}

sad::p2d::app::Way* core::Shared::selectedWay() const
{
	return m_selected_way;
}

void core::Shared::setWayPointPosition(int p)
{
    m_way_point_position = p;
}

int core::Shared::wayPointPosition() const
{
    return m_way_point_position;
}

void core::Shared::setOldPoint(const sad::Point2D& p)
{
    m_old_point = p;
}

const sad::Point2D& core::Shared::oldPoint() const
{
    return m_old_point;
}


void core::Shared::setSelectedDialogue(sad::dialogue::Dialogue* d)
{
	m_selected_dialogue = d;
}

sad::dialogue::Dialogue*  core::Shared::selectedDialogue() const
{
	return m_selected_dialogue;
}

void core::Shared::setSelectedAnimation(sad::animations::Animation* a)
{
	m_selected_animation = a;
}

sad::animations::Animation* core::Shared::selectedAnimation() const
{
	return m_selected_animation;	
}
