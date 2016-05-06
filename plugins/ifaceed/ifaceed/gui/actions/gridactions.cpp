#include "gridactions.h"

#include <QLineEdit>
#include <QScrollArea>

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

void gui::actions::GridActions::removeLastGrid()
{
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    this->clearGridCellsBrowser();
    QListWidget* list = layout_blk->lstLayoutGridList;
    int lastrow = list->count() -1;
    if (lastrow > -1)
    {
        delete layout_blk->lstLayoutGridList->takeItem(lastrow);
        sad::layouts::Grid* g = m_editor->shared()->selectedGrid();
        bool set = false;
        if (g)
        {
            int row = this->findInList(list, g);
            if (row > -1)
            {
                list->setCurrentRow(row);
                this->updateGridPropertiesInUI(true);
                set = true;
            }
        }

        if (!set)
        {
            list->setCurrentRow(-1);
        }
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

void gui::actions::GridActions::updateCellBrowser(bool immediate)
{
	if (!immediate)
    {
        m_editor->emitClosure(::bind(this, &gui::actions::GridActions::updateCellBrowser, true));
        return;
    }

	clearGridCellsBrowser();
	
	gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
	sad::layouts::Grid* grid = this->selectedGrid();
	QGridLayout* table = new QGridLayout();
	if (grid)
	{
		sad::Vector<sad::layouts::SerializableCell> cells = grid->cells();
		for(size_t i = 0; i < cells.size(); i++) 
		{
			gui::layouts::LayoutCellEdit* edit = new gui::layouts::LayoutCellEdit();
			edit->setRowAndColumn(cells[i].Row, cells[i].Col);
			edit->set(grid->cell(cells[i].Row, cells[i].Col));
			edit->setChildrenProvider(m_provider);
            connect(
                edit,
                SIGNAL(widthChanged(size_t,size_t,sad::layouts::LengthValue)),
                this,
                SLOT(cellWidthChanged(size_t,size_t,sad::layouts::LengthValue))
            );
            connect(
                edit,
                SIGNAL(heightChanged(size_t,size_t,sad::layouts::LengthValue)),
                this,
                SLOT(cellHeightChanged(size_t,size_t,sad::layouts::LengthValue))
            );
            connect(
                edit,
                SIGNAL(horizontalAlignmentChanged(size_t,size_t,sad::layouts::HorizontalAlignment)),
                this,
                SLOT(cellHorizontalAlignmentChanged(size_t,size_t,sad::layouts::HorizontalAlignment))
            );
            connect(
                edit,
                SIGNAL(verticalAlignmentChanged(size_t,size_t,sad::layouts::VerticalAlignment)),
                this,
                SLOT(cellVerticalAlignmentChanged(size_t,size_t,sad::layouts::VerticalAlignment))
            );
            connect(
                edit,
                SIGNAL(stackingTypeChanged(size_t,size_t,sad::layouts::StackingType)),
                this,
                SLOT(cellStackingTypeChanged(size_t,size_t,sad::layouts::StackingType))
            );
            connect(
                edit,
                SIGNAL(topPaddingChanged(size_t,size_t,double)),
                this,
                SLOT(cellTopPaddingChanged(size_t,size_t,double))
            );
            connect(
                edit,
                SIGNAL(bottomPaddingChanged(size_t,size_t,double)),
                this,
                SLOT(cellBottomPaddingChanged(size_t,size_t,double))
            );
            connect(
                edit,
                SIGNAL(leftPaddingChanged(size_t,size_t,double)),
                this,
                SLOT(cellLeftPaddingChanged(size_t,size_t,double))
            );
            connect(
                edit,
                SIGNAL(rightPaddingChanged(size_t,size_t,double)),
                this,
                SLOT(cellRightPaddingChanged(size_t,size_t,double))
            );
            connect(
                edit,
                SIGNAL(childAdded(size_t,size_t,unsigned long long,QString)),
                this,
                SLOT(cellChildAdded(size_t,size_t,unsigned long long,QString))
            );
            connect(
                edit,
                SIGNAL(childRemoved(size_t,size_t,size_t)),
                this,
                SLOT(cellChildRemoved(size_t,size_t,size_t))
            );
            connect(
                edit,
                SIGNAL(cleared(size_t,size_t)),
                this,
                SLOT(cellCleared(size_t,size_t))
            );
            connect(
                edit,
                SIGNAL(childrenSwapped(size_t,size_t,size_t,size_t)),
                this,
                SLOT(cellChildrenSwapped(size_t,size_t,size_t,size_t))
            );
			table->addWidget(edit, cells[i].Row, cells[i].Col, cells[i].RowSpan, cells[i].ColSpan);
			insertCellEditor(cells[i].Row, cells[i].Col, edit);
		}
	}

	QWidget* w = new QWidget();
    w->setLayout(table);
    layout_blk->tblLayoutCells->setWidget(w);
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
	gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
	QGridLayout* table = new QGridLayout();	
	QWidget* w = new QWidget();
    w->setLayout(table);
    layout_blk->tblLayoutCells->setWidget(w);
	
	m_cell_editors.clear();
}

void gui::actions::GridActions::higlightAddingState() const
{
	m_editor->panelProxy()->highlightState("Click where you want layout to be placed");
}

void gui::actions::GridActions::higlightMovingState() const
{
	m_editor->panelProxy()->highlightState("Click where you want layout to be placed");	
}

void gui::actions::GridActions::cancelAddGrid()
{
    sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
    if (grid)
    {
        m_editor->emitClosure(::bind(
            this,
            &gui::actions::GridActions::removeLastGrid
        ));
        m_editor->renderGrids()->remove(grid);
        m_editor->shared()->setActiveGrid(NULL);
        m_editor->machine()->enterState(m_editor->machine()->previousState());
    }
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
    layout_blk->lstLayoutGridList->setCurrentRow(
        layout_blk->lstLayoutGridList->count() - 1
    );
	m_editor->renderGrids()->add(grid);
	m_editor->shared()->setActiveGrid(grid);
	m_editor->machine()->enterState("layouts/adding");    
	this->updateRegion();
	this->updateCellBrowser();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::showGridsClicked(bool state)
{
    if (m_editor)
    {
        m_editor->renderGrids()->setEnabled(state);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellWidthChanged(size_t row, size_t col, sad::layouts::LengthValue newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellHeightChanged(size_t row, size_t col, sad::layouts::LengthValue newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellHorizontalAlignmentChanged(size_t row, size_t col, sad::layouts::HorizontalAlignment newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellVerticalAlignmentChanged(size_t row, size_t col, sad::layouts::VerticalAlignment newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellStackingTypeChanged(size_t row, size_t col, sad::layouts::StackingType newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellTopPaddingChanged(size_t row, size_t col, double newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellBottomPaddingChanged(size_t row, size_t col, double newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellLeftPaddingChanged(size_t row, size_t col, double newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellRightPaddingChanged(size_t row, size_t col, double newvalue)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellChildAdded(size_t row, size_t col, unsigned long long majorid, QString nodename)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellChildRemoved(size_t row, size_t col_t, size_t pos)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellCleared(size_t row, size_t col)
{
    // TODO: Implement this
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellChildrenSwapped(size_t row, size_t col, size_t pos1, size_t pos2)
{
    // TODO: Implement this
}
