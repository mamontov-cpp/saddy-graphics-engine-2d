#include "gridactions.h"

#include <QLineEdit>

#include "../../qstdstring.h"

#include "../../closuremethodcall.h"

#include "../../core/editor.h"
#include "../../core/shared.h"

#include "../../core/typeconverters/qrectftosadrect2d.h"
#include "../../core/typeconverters/sadrect2dtoqrectf.h"

#include "../mainpanelproxy.h"

#include "../../gui/rectwidget/rectwidget.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uilayoutblock.h"

#include "../rendergrids.h"

#include <renderer.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>

Q_DECLARE_METATYPE(sad::layouts::Grid*);

gui::actions::GridActions::GridActions(QObject* parent)
: QObject(parent), m_provider(NULL)
{

}

void gui::actions::GridActions::setEditor(core::Editor* e)
{
	this->gui::actions::AbstractActions::setEditor(e);
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
		core::Shared* shared = m_editor->shared(); 
        result = shared->selectedGrid();
		if (shared->activeGrid())
		{
			result = shared->activeGrid();
		}
    }
    return result;
}

void gui::actions::GridActions::activeGrids(sad::Vector<sad::layouts::Grid*>& grids)
{
    grids.clear();
    sad::db::Database* db = sad::Renderer::ref()->database("");
    if (db)
    {
        gui::RenderGrids::gridsFromTable(db->table("layouts"), grids);
        for(size_t i = 0; i < grids.size(); i++)
        {
            if (grids[i]->Active == false)
            {
                grids.removeAt(i);
                --i;
            }
        }
    }
}

void gui::actions::GridActions::addGridToGridList(sad::layouts::Grid* grid) const
{
    if (m_editor)
    {
        gui::uiblocks::UILayoutBlock* blk = m_editor->uiBlocks()->uiLayoutBlock();
        QListWidget* lst = blk->lstLayoutGridList;
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(this->viewableObjectName(grid));
        QVariant v;
        v.setValue(grid);
        item->setData(Qt::UserRole, v);
        lst->addItem(item);
    }
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

void gui::actions::GridActions::updateRegion(bool immediate)
{
	if (!immediate)
    {
        m_editor->emitClosure(::bind(this, &gui::actions::GridActions::updateRegion, true));
        return;
    }

	gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
	sad::layouts::Grid* grid = this->selectedGrid();
	if (grid)
	{
		QRectF rect;
		core::typeconverters::SadRect2DToQRectF::convert(grid->area(), rect);
		layout_blk->rwLayoutArea->setValue(rect);
	}
}

void gui::actions::GridActions::updateGridPropertiesInUI(bool immediate)
{
    if (!immediate)
    {
        m_editor->emitClosure(::bind(this, &gui::actions::GridActions::updateGridPropertiesInUI, true));
        return;
    }

	updateRegion(true);
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


sad::Vector<gui::GridPosition> gui::actions::GridActions::findRelatedGrids(sad::SceneNode* node)
{
    sad::Vector<gui::GridPosition> result;
    sad::db::Database* db = sad::Renderer::ref()->database("");
    sad::db::Table* tbl = db->table("layouts");

    sad::Vector<unsigned long long> already_children;
    QHash<unsigned long long, bool> already_children_hash;
    if (tbl)
    {
        sad::Vector<sad::db::Object*> objs;
        tbl->objects(objs);
        for(size_t i = 0; i < objs.size(); i++)
        {
            if (objs[i]->Active && objs[i]->isInstanceOf("sad::layouts::Grid"))
            {
                sad::layouts::Grid* grid = static_cast<sad::layouts::Grid*>(objs[i]);
                sad::Maybe<sad::layouts::Grid::SearchResult> mayberesult = grid->find(node);
                if (mayberesult.exists())
                {
                    sad::layouts::Cell* cell = grid->cell(mayberesult.value().p1());
                    result << gui::GridPosition(grid, cell->Row, cell->Col, mayberesult.value().p2());
                }
            }
        }
    }
    return result;
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void gui::actions::GridActions::clearGridCellsBrowser()
{
	// TODO: Actually implement this
}

void gui::actions::GridActions::higlightAddingState() const
{
	m_editor->panelProxy()->highlightState("Click where you want layout to be placed");
}

void gui::actions::GridActions::higlightMovingState() const
{
	m_editor->panelProxy()->highlightState("Click where you want layout to be placed");	
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void gui::actions::GridActions::addGridClicked()
{
	if (m_editor->isInEditingState())
		return;
	// Enable displaying grids
	gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
	layout_blk->cbLayoutShow->setCheckState(Qt::Checked);
	m_editor->renderGrids()->setEnabled(true);

	// Make new grid
	sad::layouts::Grid* grid = new sad::layouts::Grid();
	grid->setTreeName(sad::Renderer::ref(), "");
	grid->setFixedWidth(layout_blk->cbLayoutFixedWidth->checkState() == Qt::Checked);
	grid->setFixedHeight(layout_blk->cbLayoutFixedHeight->checkState() == Qt::Checked);
	sad::Rect2D rect;
	core::typeconverters::QRectFToSadRect2D::convert(layout_blk->rwLayoutArea->value(), rect);
	grid->setArea(rect);
	QString name = layout_blk->txtLayoutGridName->text();
	grid->setObjectName(Q2STDSTRING(name));
	grid->setPaddingTop(layout_blk->dsbLayoutPaddingTop->value(), true);
	grid->setPaddingBottom(layout_blk->dsbLayoutPaddingBottom->value(), true);
	grid->setPaddingLeft(layout_blk->dsbLayoutPaddingLeft->value(), true);
	grid->setPaddingRight(layout_blk->dsbLayoutPaddingRight->value(), true);
	grid->setRows(layout_blk->spnLayoutGridRows->value());
	grid->setColumns(layout_blk->spnLayoutGridCols->value());

	sad::layouts::Grid* selected_grid = m_editor->shared()->selectedGrid();
	if (selected_grid)
	{
		sad::Vector<sad::layouts::SerializableCell> scells = selected_grid->cells();
		for(size_t i = 0; i < scells.size(); i++)
		{
			scells[i].Children.clear();
		}
		grid->setCells(scells);
	}

	this->addGridToGridList(grid);
	m_editor->renderGrids()->add(grid);
	m_editor->shared()->setActiveGrid(grid);
	m_editor->machine()->enterState("layouts/adding");
	this->updateRegion();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::showGridsClicked(bool state)
{
    if (m_editor)
    {
        m_editor->renderGrids()->setEnabled(state);
    }
}