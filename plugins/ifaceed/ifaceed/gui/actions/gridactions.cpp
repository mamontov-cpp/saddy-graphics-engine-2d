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

#include "../../history/layouts/layoutsnew.h"
#include "../../history/layouts/layoutschangename.h"
#include "../../history/layouts/layoutsaddchild.h"

#include <renderer.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>

Q_DECLARE_METATYPE(sad::layouts::Grid*)

gui::actions::GridActions::GridActions(QObject* parent)
: QObject(parent), m_provider(NULL), m_is_stretching(false)
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

    // Update proxy just in case, if it was invalidated by setting it not in needed time
    m_provider->setProxy(m_editor->panelProxy());

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
            edit->setChildrenProvider(m_provider);
            edit->set(grid->cell(cells[i].Row, cells[i].Col));
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

    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    sad::layouts::Grid* grid = this->selectedGrid();
    if (grid)
    {
        const size_t size = 8;
        QWidget* widgets[size] = {
            layout_blk->spnLayoutGridRows,
            layout_blk->spnLayoutGridCols,
            layout_blk->cbLayoutFixedWidth,
            layout_blk->cbLayoutFixedHeight,
            layout_blk->dsbLayoutPaddingTop,
            layout_blk->dsbLayoutPaddingRight,
            layout_blk->dsbLayoutPaddingLeft,
            layout_blk->dsbLayoutPaddingBottom
        };
        bool states[size];
        for(size_t i = 0; i < size; i++)
        {
            states[i] = widgets[i]->blockSignals(size);
        }

        layout_blk->spnLayoutGridRows->setValue(grid->rows());
        layout_blk->spnLayoutGridCols->setValue(grid->columns());
        layout_blk->cbLayoutFixedWidth->setCheckState((grid->fixedWidth()) ? Qt::Checked : Qt::Unchecked);
        layout_blk->cbLayoutFixedHeight->setCheckState((grid->fixedHeight()) ? Qt::Checked : Qt::Unchecked);
        layout_blk->dsbLayoutPaddingLeft->setValue(grid->paddingLeft());
        layout_blk->dsbLayoutPaddingRight->setValue(grid->paddingRight());
        layout_blk->dsbLayoutPaddingTop->setValue(grid->paddingTop());
        layout_blk->dsbLayoutPaddingBottom->setValue(grid->paddingBottom());

        for(size_t i = 0; i < size; i++)
        {
            widgets[i]->blockSignals(states[i]);
        }
    }

    updateCellBrowser(true);
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

void gui::actions::GridActions::highlightPlaceFirstPointState() const
{
    m_editor->panelProxy()->highlightState("Click where you want top-left corner of layout to be placed");
}

void gui::actions::GridActions::highlightPlaceSecondPointState() const
{
    m_editor->panelProxy()->highlightState("Click where you want bottom-right corner of layout to be placed");
}

void gui::actions::GridActions::higlightMovingState() const
{
    m_editor->panelProxy()->highlightState("Click where you want layout to be placed");	
}

void gui::actions::GridActions::highlightResizingState() const
{
    m_editor->panelProxy()->highlightState("Release button to stop resizing");
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
        m_editor->emitClosure(::bind(
            this,
            &gui::actions::GridActions::enableEditingAreaControls
        ));
        m_editor->renderGrids()->remove(grid);
        m_editor->shared()->setActiveGrid(NULL);

        m_editor->machine()->enterState(m_previous_machine_state);
    }
}

void gui::actions::GridActions::moveByCenter(const sad::input::MouseMoveEvent& e)
{
    sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
    if (grid)
    {
        const sad::Rect2D& r = grid->area();
        sad::Point2D p = r.p0() + r.p2();
        p /= 2.0;
        grid->moveBy(e.pos2D() - p);
        this->updateRegion();
    }
}

void gui::actions::GridActions::moveByBottomRightCorner(const sad::input::MouseMoveEvent& e)
{
    core::Shared* s = m_editor->shared();
    sad::layouts::Grid* grid = s->activeGrid();
    if (grid)
    {
        sad::Point2D p1 = s->pivotPoint();
        sad::Point2D p2 = e.pos2D();
        sad::Point2D pmin(
           std::min(p1.x(), p2.x()),
           std::min(p1.y(), p2.y())
        );
        sad::Point2D pmax(
           std::max(p1.x(), p2.x()),
           std::max(p1.y(), p2.y())
        );
        grid->setArea(sad::Rect2D(pmin, pmax));
        this->updateRegion();
    }
}

void gui::actions::GridActions::removeGridFromGridList(sad::layouts::Grid* g)
{
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    this->clearGridCellsBrowser();
    QListWidget* list = layout_blk->lstLayoutGridList;
    int row = this->findInList(list, g);
    if (row > -1)
    {
        delete list->takeItem(row);
    }
    if (m_editor->shared()->selectedGrid() == g)
    {
        m_editor->shared()->setSelectedGrid(NULL);
        this->clearGridCellsBrowser();;
    }
}

void gui::actions::GridActions::commitGridAdd(const sad::input::MousePressEvent& e)
{
    sad::layouts::Grid* g = m_editor->shared()->activeGrid();
    if (!g)
    {
        return;
    }

    sad::input::MouseMoveEvent ev;
    ev.Point3D = e.Point3D;

    if (m_is_stretching)
    {
        this->moveByBottomRightCorner(ev);
    }
    else
    {
        this->moveByCenter(ev);
    }

    // Enable last disabled objects
    m_editor->emitClosure(::bind(this, &gui::actions::GridActions::enableEditingAreaControls));

    sad::Renderer::ref()->database("")->table("layouts")->add(g);
    m_editor->shared()->setActiveGrid(NULL);
    m_editor->shared()->setSelectedGrid(g);

    m_editor->machine()->enterState(m_previous_machine_state);

    m_editor->history()->add(new history::layouts::New(g));
}

void gui::actions::GridActions::commitTopLeftPoint(const sad::input::MousePressEvent& e)
{
    core::Shared* s = m_editor->shared();
    sad::layouts::Grid* ag = s->activeGrid();
    if (ag)
    {
        sad::Point2D p = e.pos2D();
        s->setPivotPoint(e.pos2D());
        ag->setVisible(true);
        ag->setArea(sad::Rect2D(p, p));

        this->updateRegion();

        m_editor->machine()->enterState("layouts/adding/secondpoint");
    }
}

void gui::actions::GridActions::setChildEditingEnabled(bool enabled)
{
    QHash<size_t, QHash<size_t, gui::layouts::LayoutCellEdit*> >::const_iterator iit;
    QHash<size_t, gui::layouts::LayoutCellEdit*>::const_iterator jit;
    for(iit = m_cell_editors.begin(); iit != m_cell_editors.end(); ++iit)
    {
        const QHash<size_t, gui::layouts::LayoutCellEdit*>& eds = iit.value();
        for(jit = eds.begin(); jit != eds.end(); ++jit)
        {
            jit.value()->setChildEditingEnabled(enabled);
        }
    }
}

void gui::actions::GridActions::enableEditingAreaControls()
{
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    layout_blk->cbLayoutFixedWidth->setEnabled(true);
    layout_blk->cbLayoutFixedHeight->setEnabled(true);
    layout_blk->rwLayoutArea->setEnabled(true);
    layout_blk->lstLayoutGridList->setEnabled(true);
    this->setChildEditingEnabled(true);
}


void gui::actions::GridActions::cancelSelection()
{
    m_editor->shared()->setSelectedGrid(NULL);
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    layout_blk->lstLayoutGridList->setCurrentRow(-1);
    this->clearGridCellsBrowser();
}

bool gui::actions::GridActions::isIndGrid(sad::SceneNode* node) const
{
    return this->findRelatedGrids(node).size() != 0;
}

// ================================ PUBLIC SLOTS  ================================

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

    this->prepareGridForAdding();
    m_is_stretching = false;
    m_editor->machine()->enterState("layouts/adding");
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void gui::actions::GridActions::addGridByStretchingClicked()
{
    if (m_editor->isInEditingState())
        return;
    // TODO: Implement it
    // Enable displaying grids
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    layout_blk->cbLayoutShow->setCheckState(Qt::Checked);
    m_editor->renderGrids()->setEnabled(true);

    QCheckBox* cbfh = layout_blk->cbLayoutFixedHeight,
             * cbfw = layout_blk->cbLayoutFixedWidth;
    bool b1 = cbfh->blockSignals(true), b2 = cbfw->blockSignals(true);

    cbfh->setCheckState(Qt::Checked);
    cbfw->setCheckState(Qt::Checked);

    cbfh->setEnabled(false);
    cbfw->setEnabled(false);

    cbfh->blockSignals(b1);
    cbfw->blockSignals(b2);

    gui::rectwidget::RectWidget* rw = layout_blk->rwLayoutArea;

    b1 = rw->blockSignals(true);

    rw->setValue(QRectF());
    rw->setEnabled(false);

    rw->blockSignals(b1);

    sad::layouts::Grid* g = this->prepareGridForAdding();
    g->setVisible(false);
    m_is_stretching = true;
    m_editor->machine()->enterState("layouts/adding/firstpoint");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::showGridsClicked(bool state)
{
    if (m_editor)
    {
        m_editor->renderGrids()->setEnabled(state);
    }
}

void gui::actions::GridActions::currentGridChanged(int row)
{
    if (m_editor)
    {
        if (row > -1)
        {
            gui::uiblocks::UIBlocks* b = m_editor->uiBlocks();
            gui::uiblocks::UILayoutBlock* bl = b->uiLayoutBlock();
            QListWidget* lst = bl->lstLayoutGridList;
            QListWidgetItem* item = lst->item(row);
            sad::layouts::Grid* g = item->data(Qt::UserRole).value<sad::layouts::Grid*>();
            m_editor->shared()->setSelectedGrid(g);
            this->updateGridPropertiesInUI();
        }
        else
        {
            m_editor->shared()->setSelectedGrid(NULL);
            this->clearGridCellsBrowser();
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void  gui::actions::GridActions::nameChanged(const QString &text)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->setObjectName(Q2STDSTRING(text));
            gui::uiblocks::UIBlocks* b = m_editor->uiBlocks();
            gui::uiblocks::UILayoutBlock* bl = b->uiLayoutBlock();
            int row = bl->lstLayoutGridList->currentRow();
            if (row > -1)
            {
                bl->lstLayoutGridList->item(row)->setText(this->viewableObjectName(grid));
            }
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                sad::String new_name = Q2STDSTRING(text);
                if (grid->objectName() != new_name)
                {
                    history::layouts::ChangeName* c = new history::layouts::ChangeName(grid, grid->objectName(), new_name);
                    c->commitWithoutUpdatingUI(m_editor);
                    m_editor->history()->add(c);
                }
            }
        }
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
    Q_UNUSED(nodename);
    sad::layouts::Grid* g = m_editor->shared()->selectedGrid();
    if (!g)
    {
        return;
    }
    sad::SceneNode* o = static_cast<sad::SceneNode*>(sad::Renderer::ref()->database("")->queryByMajorId(majorid));
    if (o)
    {
        sad::Maybe<sad::Rect2D> oldarea = o->getProperty<sad::Rect2D>("area");
        if (oldarea.exists())
        {
            // TODO: Make new history entry here and commit it
            history::layouts::AddChild* c = new history::layouts::AddChild(g, row, col, o, oldarea.value());
            c->commitWithoutUpdatingUI(m_editor);
            m_editor->history()->add(c);
        }
    }
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

// =============================== PRIVATE METHODS ===============================

sad::layouts::Grid* gui::actions::GridActions::prepareGridForAdding()
{
    // Make new grid
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    // Disable selecting any other grid
    layout_blk->lstLayoutGridList->setEnabled(false);
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
    this->updateRegion();
    this->updateCellBrowser(true);
    this->setChildEditingEnabled(false);

    this->m_previous_machine_state = m_editor->machine()->currentState();
    return grid;
}
