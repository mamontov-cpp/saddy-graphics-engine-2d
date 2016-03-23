#include "gridactions.h"
#include "../../closuremethodcall.h"
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

void gui::actions::GridActions::insertChildToGrid(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, sad::SceneNode* node)
{
	g->cell(row, col)->insertChild(pos, node);
	if (this->selectedGrid() == g)
	{
		gui::layouts::LayoutCellEdit* edit = this->cellEditor(row, col);
		if (edit)
		{
			m_editor->emitClosure(::bind(edit, &gui::layouts::LayoutCellEdit::insertChild, node, pos));
			updateGridPropertiesInUI();
		}
	}
}

void gui::actions::GridActions::removeChildFromGridInUI(sad::layouts::Grid* g, size_t row, size_t col, size_t pos)
{
	if (this->selectedGrid() == g)
	{
		gui::layouts::LayoutCellEdit* edit = this->cellEditor(row, col);
		if (edit)
		{
			void (gui::layouts::LayoutCellEdit::*method)(size_t) = &gui::layouts::LayoutCellEdit::removeChild;
			m_editor->emitClosure(::bind(edit, method, pos));;
			updateGridPropertiesInUI();
		}
	}
}

void  gui::actions::GridActions::removeChildFromGrid(sad::layouts::Grid* g, size_t row, size_t col, size_t pos)
{
	g->cell(row, col)->removeChild(pos);
	removeChildFromGridInUI(g, row, col, pos);
}

void gui::actions::GridActions::insertCellEditor(size_t row,  size_t col, gui::layouts::LayoutCellEdit* cell)
{
	if (m_cell_editors.contains(row) == false)
	{
		m_cell_editors.insert(row, QHash<size_t, gui::layouts::LayoutCellEdit*>());
	}
	m_cell_editors[row].insert(col, cell);
}

void gui::actions::GridActions::updateChildName(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, const QString& name)
{
	if (this->selectedGrid() == g)
	{
		gui::layouts::LayoutCellEdit* edit = this->cellEditor(row, col);
		if (edit)
		{			
			m_editor->emitClosure(::bind(edit, &gui::layouts::LayoutCellEdit::updateChildName, pos, name));
		}
	}
}

void gui::actions::GridActions::updateGridPropertiesInUI(bool immediate)
{
	if (!immediate)
	{
		m_editor->emitClosure(::bind(this, &gui::actions::GridActions::updateGridPropertiesInUI, true));
		return;
	}
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
