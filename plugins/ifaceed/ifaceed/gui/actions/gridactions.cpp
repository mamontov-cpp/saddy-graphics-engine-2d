#include "gridactions.h"
#include "../../core/editor.h"
#include "../../core/shared.h"


gui::actions::GridActions::GridActions(QObject* parent)
: QObject(parent), m_provider(NULL)
{

}

void gui::actions::GridActions::setEditor(core::Editor* e)
{
	m_provider = new gui::ChildrenProvider();
	m_provider->setProxy(e->panelProxy());
}

gui::actions::GridActions::~GridActions()
{
	delete m_provider;
}

sad::layouts::Grid* gui::actions::GridActions::selectedGrid() const
{
	sad::layouts::Grid* result = NULL;
	if (m_editor)
	{
		result = m_editor->shared()->selectedGrid();
	}
	return result;
}

void gui::actions::GridActions::insertCellEditor(size_t row,  size_t col, gui::layouts::LayoutCellEdit* cell)
{
	if (m_cell_editors.contains(row) == false)
	{
		m_cell_editors.insert(row, QHash<size_t, gui::layouts::LayoutCellEdit*>());
	}
	m_cell_editors[row].insert(col, cell);
}

gui::layouts::LayoutCellEdit* gui::actions::GridActions::cellEditor(size_t row,  size_t col)
{
	gui::layouts::LayoutCellEdit* edit = NULL;
	if (m_cell_editors.contains(row))
	{
		if (m_cell_editors[row].contains(col))
		{
			edit = m_cell_editors[row][col];
		}
	}
	return edit;
}
