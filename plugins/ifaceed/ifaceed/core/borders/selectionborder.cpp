#include "selectionborder.h"

#include <renderer.h>

core::borders::SelectionBorder::SelectionBorder(core::Shared* data) : core::borders::Border(data)
{
	m_delete_hotspot = new core::borders::DeleteHotspot();
}


core::borders::SelectionBorder::~SelectionBorder()
{
	delete m_delete_hotspot;
}


core::borders::DeleteHotspot* core::borders::SelectionBorder::deleteHotspot()
{
	return m_delete_hotspot;
}

void core::borders::SelectionBorder::_process()
{
	sad::SceneNode * o = this->m_data->selectedObject();
	if (o) 
	{
		sad::Vector<sad::Rect2D> regions;
		o->regions(regions);
		sad::Renderer::ref()->render()
							->rectangle(regions[0], sad::AColor(0, 255, 255, 255));
		m_delete_hotspot->render(regions[0]);

	}
}

