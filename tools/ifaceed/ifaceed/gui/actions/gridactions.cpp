#include "gridactions.h"

#include <algorithm>

#include <QLineEdit>
#include <QScrollArea>
#include <QScrollBar>

#include "../../qstdstring.h"
#include "../../keytovector.h"

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
#include "../../history/layouts/layoutsremove.h"
#include "../../history/layouts/layoutschangename.h"
#include "../../history/layouts/layoutschange.h"
#include "../../history/layouts/layoutsaddchild.h"
#include "../../history/layouts/layoutsremovechild.h"
#include "../../history/layouts/layoutsclearcell.h"
#include "../../history/layouts/layoutsswapchildren.h"
#include "../../history/layouts/layoutschangecell.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"


#include <renderer.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>

Q_DECLARE_METATYPE(sad::layouts::Grid*)
Q_DECLARE_METATYPE(gui::actions::GridActions::GridUpdateOptions)

gui::actions::GridActions::GridActions(QObject* parent)
: QObject(parent), m_provider(nullptr), m_is_stretching(false)
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
    sad::layouts::Grid* result = nullptr;
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

void gui::actions::GridActions::insertGridToGridList(sad::layouts::Grid* grid, int position) const
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
        lst->insertItem(position, item);
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

void gui::actions::GridActions::insertChildToGrid(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, size_t majorid)
{
    sad::SceneNode* node = sad::Renderer::ref()->database("")->objectByMajorId<sad::SceneNode>(majorid);
    if (node)
    {
        this->insertChildToGrid(g, row, col, pos, node);
    }
}

void gui::actions::GridActions::insertChildToGrid(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, sad::SceneNode* node)
{
    g->cell(row, col)->insertChild(pos, node);
    this->insertNodeToGridEntry(node, g);
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
    double scrollvaluex = 0;
    double scrollvaluey = 0;
        
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();

    QScrollBar* hbar = layout_blk->tblLayoutCells->horizontalScrollBar();
    QScrollBar* vbar = layout_blk->tblLayoutCells->verticalScrollBar();
    if (hbar)
    {
        scrollvaluex = hbar->value();
    }
    if (vbar)
    {
        scrollvaluey = vbar->value();
    }


    clearGridCellsBrowser();

    // Update proxy just in case, if it was invalidated by setting it not in needed time
    m_provider->setProxy(m_editor->panelProxy());

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
    hbar = layout_blk->tblLayoutCells->horizontalScrollBar();
    vbar = layout_blk->tblLayoutCells->verticalScrollBar();
    if (hbar)
    {
        if (scrollvaluex < hbar->maximum())
        {
            hbar->setValue(scrollvaluex);
        }        
    }

    if (vbar)
    {
        if (scrollvaluey < vbar->maximum())
        {
            vbar->setValue(scrollvaluey);
        }
    }

}

void gui::actions::GridActions::updateOnlyGridPropertiesInUI(
    gui::actions::GridActions::GridUpdateOptions group,
    bool immediate
)
{
    if (!immediate)
    {
        m_editor->emitClosure(::bind(this, &gui::actions::GridActions::updateOnlyGridPropertiesInUI, group, true));
        return;
    }

    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    sad::layouts::Grid* grid = this->selectedGrid();
    if (grid)
    {
        const size_t size = 9;
        QWidget* widgets[size] = {
            layout_blk->txtLayoutGridName,
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

        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_Rows)
        {
            layout_blk->spnLayoutGridRows->setValue(grid->rows());
        }
        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_Cols)
        {
            layout_blk->spnLayoutGridCols->setValue(grid->columns());
        }
        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedWidth)
        {
            layout_blk->cbLayoutFixedWidth->setCheckState((grid->fixedWidth()) ? Qt::Checked : Qt::Unchecked);
        }
        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedHeight)
        {
            layout_blk->cbLayoutFixedHeight->setCheckState((grid->fixedHeight()) ? Qt::Checked : Qt::Unchecked);
        }
        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding)
        {
            layout_blk->dsbLayoutPaddingLeft->setValue(grid->paddingLeft());
        }
        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding)
        {
            layout_blk->dsbLayoutPaddingRight->setValue(grid->paddingRight());
        }
        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding)
        {
            layout_blk->dsbLayoutPaddingTop->setValue(grid->paddingTop());
        }
        if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding)
        {
            layout_blk->dsbLayoutPaddingBottom->setValue(grid->paddingBottom());
        }

        for(size_t i = 0; i < size; i++)
        {
            widgets[i]->blockSignals(states[i]);
        }
    }

    if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_Cells)
    {
        updateCellBrowser(true);
    }

    if (group == gui::actions::GridActions::GridUpdateOptions::GGAUO_Area)
    {
        updateRegion(true);
    }    
}

void gui::actions::GridActions::updateGridPropertiesInUIExcept(
    gui::actions::GridActions::GridUpdateOptions group,
    bool immediate
)
{
    if (!immediate)
    {
        m_editor->emitClosure(::bind(this, &gui::actions::GridActions::updateGridPropertiesInUIExcept, group, true));
        return;
    }

    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    sad::layouts::Grid* grid = this->selectedGrid();
    if (grid)
    {
        const size_t size = 9;
        QWidget* widgets[size] = {
            layout_blk->txtLayoutGridName,
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

        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_Rows)
        {
            layout_blk->spnLayoutGridRows->setValue(grid->rows());
        }
        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_Cols)
        {
            layout_blk->spnLayoutGridCols->setValue(grid->columns());
        }
        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedWidth)
        {
            layout_blk->cbLayoutFixedWidth->setCheckState((grid->fixedWidth()) ? Qt::Checked : Qt::Unchecked);
        }
        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedHeight)
        {
            layout_blk->cbLayoutFixedHeight->setCheckState((grid->fixedHeight()) ? Qt::Checked : Qt::Unchecked);
        }
        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding)
        {
            layout_blk->dsbLayoutPaddingLeft->setValue(grid->paddingLeft());
        }
        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding)
        {
            layout_blk->dsbLayoutPaddingRight->setValue(grid->paddingRight());
        }
        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding)
        {
            layout_blk->dsbLayoutPaddingTop->setValue(grid->paddingTop());
        }
        if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding)
        {
            layout_blk->dsbLayoutPaddingBottom->setValue(grid->paddingBottom());
        }

        for(size_t i = 0; i < size; i++)
        {
            widgets[i]->blockSignals(states[i]);
        }
    }

    if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_Cells)
    {
        updateCellBrowser(true);
    }

    if (group != gui::actions::GridActions::GridUpdateOptions::GGAUO_Area)
    {
        updateRegion(true);
    }
}

void gui::actions::GridActions::updateGridPropertiesInUI(bool immediate)
{    
    this->updateGridPropertiesInUIExcept(gui::actions::GridActions::GridUpdateOptions::GGAUO_None, immediate);    
}

gui::layouts::LayoutCellEdit* gui::actions::GridActions::cellEditor(size_t row,  size_t col)
{
    gui::layouts::LayoutCellEdit* edit = nullptr;
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

void gui::actions::GridActions::highlightAddingState() const
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

void gui::actions::GridActions::highlightMovingState() const
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
        m_editor->shared()->setActiveGrid(nullptr);

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

void gui::actions::GridActions::moveByPivotPoint(const sad::input::MouseMoveEvent& e)
{
    sad::layouts::Grid* grid = m_editor->shared()->selectedGrid();
    if (grid)
    {
        sad::Rect2D area  = m_editor->shared()->oldArea();
        sad::moveBy(e.pos2D() - m_editor->shared()->pivotPoint(), area);
        grid->setArea(area);
        this->updateRegion();
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            if (this->parentGridFor(node) == grid)
            {
                m_editor->actions()->sceneNodeActions()->updateRegionForNode();
            }
        }
    }
}

void gui::actions::GridActions::commitMovingGrid(const sad::input::MouseReleaseEvent& e)
{
    sad::layouts::Grid* grid = m_editor->shared()->selectedGrid();
    if (grid)
    {
        sad::input::MouseMoveEvent ev;
        ev.Point3D = e.Point3D;
        moveByPivotPoint(ev);
        picojson::value value(picojson::object_type, false);
        grid->save(value);
        history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Area>* change = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Area>(grid);
        change->saveOldState(m_editor->shared()->oldState());
        change->saveNewState(value);
        change->addAffectedNodes(grid->children());
        m_editor->history()->add(change);
        sad::String previous_state = m_editor->machine()->previousState();
        m_editor->machine()->enterState(previous_state);
    }
}

void gui::actions::GridActions::resizeGridUsingHotspot(const sad::input::MouseMoveEvent& e)
{
    sad::layouts::Grid* grid = m_editor->shared()->selectedGrid();
    sad::Rect2D area = m_editor->shared()->oldArea();

    // Compute movement distance
    sad::Point2D direction = e.pos2D() - m_editor->shared()->pivotPoint();
    sad::p2d::Vector movement = m_editor->shared()->resizingDirection();
    movement *= sad::p2d::scalar(direction, m_editor->shared()->resizingDirection());

    // Apply distance
    const sad::Pair<int, int> & indexes = m_editor->shared()->resizingIndexes();
    area[indexes.p1()] += movement;
    area[indexes.p2()] += movement;

    grid->setArea(area);
    grid->update();
    this->updateRegion();

    sad::layouts::Grid* g = this->parentGridFor(grid);
    if (g)
    {
        g->update();
        this->updateRegion();
    }
}

void gui::actions::GridActions::commitGridResizingUsingHotspot(const sad::input::MouseReleaseEvent& e)
{
    sad::layouts::Grid* grid = m_editor->shared()->selectedGrid();
    if (grid)
    {
        sad::input::MouseMoveEvent ev;
        ev.Point3D = e.Point3D;
        resizeGridUsingHotspot(ev);
        picojson::value value(picojson::object_type, false);
        grid->save(value);
        history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Area>* change = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Area>(grid);
        change->saveOldState(m_editor->shared()->oldState());
        change->saveNewState(value);
        change->addAffectedNodes(grid->children());
        m_editor->history()->add(change);
        sad::String previous_state = m_editor->machine()->previousState();
        m_editor->machine()->enterState(previous_state);
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
        m_editor->shared()->setSelectedGrid(nullptr);
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
    m_editor->shared()->setActiveGrid(nullptr);
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
    m_editor->shared()->setSelectedGrid(nullptr);
    gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
    layout_blk->lstLayoutGridList->setCurrentRow(-1);
    this->clearGridCellsBrowser();
}

bool gui::actions::GridActions::isInGrid(sad::SceneNode* node)
{
    return this->parentGridFor(node) != nullptr;
}

sad::layouts::Grid* gui::actions::GridActions::parentGridFor(sad::SceneNode* node)
{
    if (m_grid_to_parent.contains(node) == false)
    {
        sad::Vector<gui::GridPosition> poses = this->findRelatedGrids(node);
        if (poses.size() == 0)
        {
            m_grid_to_parent.insert(node, nullptr);
        }
        else
        {
            m_grid_to_parent.insert(node, poses[0].Grid);
        }
    }
    return m_grid_to_parent[node];
}

void gui::actions::GridActions::insertNodeToGridEntry(sad::SceneNode* node, sad::layouts::Grid* g)
{
    m_grid_to_parent.insert(node, g);
}

void gui::actions::GridActions::eraseNodeToGridEntry(sad::SceneNode* node)
{
    m_grid_to_parent.remove(node);
}

void gui::actions::GridActions::clearNodeToGridCache()
{
    m_grid_to_parent.clear();
}

void  gui::actions::GridActions::tryUpdateNodeNameInGrid(sad::SceneNode* node)
{	
    sad::layouts::Grid* grid = this->parentGridFor(node);
    if (grid)
    {
        if (m_editor->shared()->selectedGrid() == grid)
        {
            sad::Maybe<sad::layouts::Grid::SearchResult> gpos = grid->find(node);
            if (gpos.exists())
            {
                sad::layouts::Cell* cell = grid->cell(gpos.value().p1());
                this->cellEditor(cell->Row, cell->Col)->updateChildName(gpos.value().p2(), m_editor->actions()->sceneNodeActions()->fullNameForNode(node));
            }
        }
    }
}

void gui::actions::GridActions::findParentGrids(
    const sad::Vector<sad::SceneNode*>& list,
    sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs
) const
{
    parent_pairs.clear();
    for(size_t i = 0; i < list.size(); i++)
    {
        sad::SceneNode* node = list[i];
        sad::Vector<gui::GridPosition> vcs = this->findRelatedGrids(node); 
        if (vcs.size())
        {
            parent_pairs << sad::Pair<sad::SceneNode*, gui::GridPosition>(node, vcs[0]);
        }
    }
}

void gui::actions::GridActions::eraseNodesFromGrids(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs)
{

    gui::SortingBuckets buckets;
    this->makeBuckets(parent_pairs, buckets);
    for(size_t i = 0; i < buckets.size(); i++)
    {
        sad::layouts::Grid* grid = buckets[i].Cell.Grid;
        sad::layouts::Cell* cell = grid->cell(buckets[i].Cell.Row, buckets[i].Cell.Col);
        sad::Vector<gui::NodeLocationInCell> & pos_cells = buckets[i].List;
        std::reverse(pos_cells.begin(), pos_cells.end());
        for(size_t j = 0; j < pos_cells.size(); j++)
        {
            sad::SceneNode* node = pos_cells[j].Node;
            cell->removeChild(pos_cells[j].Pos);
            this->eraseNodeToGridEntry(node);
            if (m_editor->shared()->selectedGrid() == grid)
            {
                this->updateRegion();
                this->updateCellBrowser();
            }
        }
    }	
}

void gui::actions::GridActions::insertNodesToGrids(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs)
{
    gui::SortingBuckets buckets;
    this->makeBuckets(parent_pairs, buckets);
    for(size_t i = 0; i < buckets.size(); i++)
    {
        sad::layouts::Grid* grid = buckets[i].Cell.Grid;
        sad::layouts::Cell* cell = grid->cell(buckets[i].Cell.Row, buckets[i].Cell.Col);
        sad::Vector<gui::NodeLocationInCell> & pos_cells = buckets[i].List;
        for(size_t j = 0; j < pos_cells.size(); j++)
        {
            sad::SceneNode* node = pos_cells[j].Node;
            cell->insertChild(pos_cells[j].Pos, node->MajorId);
            this->insertNodeToGridEntry(node, grid);
            if (m_editor->shared()->selectedGrid() == grid)
            {
                this->updateRegion();
                this->updateCellBrowser();
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::scriptableRemoveGrid(sad::layouts::Grid* grid, bool from_editor)
{
    if (m_editor->isInEditingState())
        return;
    if (!grid)
    {
        grid = m_editor->shared()->selectedGrid();
    }
    if (grid)
    {
        // Try updatable
        gui::uiblocks::UILayoutBlock* layout_blk = m_editor->uiBlocks()->uiLayoutBlock();
        int pos = this->findInList(layout_blk->lstLayoutGridList, grid);

        sad::Vector<gui::GridPosition> positions = this->findRelatedGrids(grid);

        history::Command* c  = new history::layouts::Remove(grid, pos, positions);
        c->commit(m_editor);
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}


void gui::actions::GridActions::updateParentGridsRecursively(sad::layouts::Grid* grid, bool immediate)
{
    sad::layouts::Grid* pgrid = this->parentGridFor(grid);
    while (pgrid)
    {
        pgrid->update();
        if (m_editor->shared()->selectedGrid() == pgrid)
        {
            this->updateRegion(immediate);
        }
        pgrid = this->parentGridFor(pgrid);
    }

    if (m_editor->shared()->selectedGrid() == grid)
    {
        this->updateRegion(immediate);
    }
}

void gui::actions::GridActions::tryUpdateRegionsInChildren(const sad::Vector<sad::SceneNode*>& v)
{
    sad::SceneNode* sgrid = m_editor->shared()->selectedGrid();
    gui::actions::SceneNodeActions* scene_node_actions = m_editor->actions()->sceneNodeActions();
    
    for(size_t i = 0; i < v.size(); i++)
    {
        if (v[i] == sgrid)
        {
            this->updateRegion();
        }
        if (m_editor->isNodeSelected(v[i]))
        {
            scene_node_actions->updateRegionForNode();
        }
    }
}

void gui::actions::GridActions::tryUpdateRegionsInChildren(sad::layouts::Grid* grid)
{
    sad::Vector<sad::SceneNode*> children = grid->children();
    tryUpdateRegionsInChildren(children);
}

void gui::actions::GridActions::tryUpdateRegionsInChildren(sad::layouts::Cell* cell)
{
    sad::Vector<sad::SceneNode*> children = cell->children();
    tryUpdateRegionsInChildren(children);
}

void gui::actions::GridActions::updateCellPartInUI(
    size_t row,
    size_t col,
    gui::actions::GridActions::CellUpdateOptions opts,
    const sad::db::Variant& v,
    bool immediate
)
{
    if (!immediate)
    {
        m_editor->emitClosure(::bind(this, &gui::actions::GridActions::updateCellPartInUI, row, col, opts, sad::db::Variant(v), true));
    }
    else
    {
        gui::layouts::LayoutCellEdit* edit = this->cellEditor(row, col);
        if (edit)
        {
#define M(X) &gui::layouts::LayoutCellEdit:: X
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_Width, sad::layouts::LengthValue>(opts, edit, M(setWidth), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_Height, sad::layouts::LengthValue>(opts, edit, M(setHeight), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_HorizontalAlignment, sad::layouts::HorizontalAlignment>(opts, edit, M(setHorizontalAlignment), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_VerticalAlignment, sad::layouts::VerticalAlignment>(opts, edit, M(setVerticalAlignment), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_StackingType, sad::layouts::StackingType>(opts, edit, M(setStackingType), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_TopPadding, double>(opts, edit, M(setPaddingTop), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_BottomPadding, double>(opts, edit, M(setPaddingBottom), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_LeftPadding, double>(opts, edit, M(setPaddingLeft), v);
            setCellPropertyInUIIf<gui::actions::GridActions::CellUpdateOptions::GCAUO_RightPadding, double>(opts, edit, M(setPaddingRight), v);
#undef M             
        }
    }
}

void gui::actions::GridActions::tryChangeAreaForGrid(
    sad::layouts::Grid* grid,
    const sad::Rect2D& v,
    bool from_editor
)
{
    if (!sad::equal(grid->area(), v)) {
        picojson::value oldstate(picojson::object_type, false);
        grid->save(oldstate);

        grid->setArea(v);
        this->updateParentGridsRecursively(grid);

        picojson::value newstate(picojson::object_type, false);
        grid->save(newstate);

        if (m_editor->shared()->selectedGrid() == grid)
        {
            if (!sad::equal(v, grid->area()))
            {
                this->updateRegion();
            }
        }

        sad::Vector<sad::SceneNode*> nodes = grid->children();
        gui::actions::SceneNodeActions* a = m_editor->actions()->sceneNodeActions();
        for(size_t i = 0; i < nodes.size(); i++)
        {
            if (m_editor->isNodeSelected(nodes[i]))
            {
                a->updateRegionForNode();
            }
        }

        history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Area>* c = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Area>(grid);
        c->saveOldState(oldstate);
        c->saveNewState(newstate);
        c->addAffectedNodes(nodes);
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::tryChangeNameForGrid(
    sad::layouts::Grid* grid,
    const sad::String& name,
    bool from_editor
)
{
    if (grid->objectName() != name)
    {
        history::layouts::ChangeName* c = new history::layouts::ChangeName(grid, grid->objectName(), name);
        if (from_editor) 
        {
            c->commitWithoutUpdatingUI(m_editor);
            m_editor->history()->add(c);
        } 
        else
        {
            c->commit(m_editor);  
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

void gui::actions::GridActions::tryChangeFixedWidthForGrid(
    sad::layouts::Grid* grid,
    bool fixed_width,
    bool from_editor        
)
{
    if (grid->fixedWidth() != fixed_width)
    {
        sad::Vector<sad::SceneNode*> children = grid->children();
        picojson::value oldstate(picojson::object_type, false);
        grid->save(oldstate);

        grid->setFixedWidth(fixed_width);
        this->updateParentGridsRecursively(grid);

        picojson::value newstate(picojson::object_type, false);
        grid->save(newstate);
    
        if (from_editor) 
        {
            this->updateRegion();            
        }
        else
        {
            if (m_editor->shared()->selectedGrid() == grid)
            {
                this->updateRegion();
                this->updateOnlyGridPropertiesInUI(gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedWidth);
            }
        }
    
        history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedWidth>* c = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedWidth>(grid);
        c->saveOldState(oldstate);
        c->saveNewState(newstate);
        c->addAffectedNodes(children);

        if (from_editor)
        {
            m_editor->history()->add(c);   
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

void gui::actions::GridActions::tryChangeFixedHeightForGrid(
    sad::layouts::Grid* grid,
    bool fixed_height,
    bool from_editor
)
{
    if (grid->fixedHeight() != fixed_height)
    {
        sad::Vector<sad::SceneNode*> children = grid->children();
        picojson::value oldstate(picojson::object_type, false);
        grid->save(oldstate);

        grid->setFixedHeight(fixed_height);
        this->updateParentGridsRecursively(grid);

        picojson::value newstate(picojson::object_type, false);
        grid->save(newstate);

        if (from_editor)
        {
            this->updateRegion();
        }
        else
        {
            if (m_editor->shared()->selectedGrid() == grid)
            {
                this->updateRegion();
                this->updateOnlyGridPropertiesInUI(gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedHeight);
            }
        }

        history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedHeight>* c = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_FixedHeight>(grid);
        c->saveOldState(oldstate);
        c->saveNewState(newstate);
        c->addAffectedNodes(children);

        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

void gui::actions::GridActions::tryChangeRowCountForGrid(
    sad::layouts::Grid* grid,
    size_t rows,
    bool from_editor
)
{
    if (grid->rows() != rows)
    {
        sad::Vector<sad::SceneNode*> children = grid->children();
        picojson::value oldstate(picojson::object_type, false);
        grid->save(oldstate);

        grid->setRows(rows);
        this->updateParentGridsRecursively(grid);
        if (!from_editor)
        {
            if (m_editor->shared()->selectedGrid() == grid)
            {
                this->updateOnlyGridPropertiesInUI(gui::actions::GridActions::GridUpdateOptions::GGAUO_Rows);
            }
        }

        picojson::value newstate(picojson::object_type, false);
        grid->save(newstate);

        if (from_editor || (m_editor->shared()->selectedGrid() == grid))
        {
            this->updateCellBrowser();
        }
        for(size_t i = 0; i < children.size(); i++)
        {
            if (grid->find(children[i]).exists())
            {
                this->insertNodeToGridEntry(children[i], grid);
            }
            else
            {
                this->eraseNodeToGridEntry(children[i]);
            }
        }

        history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Rows>* c = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Rows>(grid);
        c->saveOldState(oldstate);
        c->saveNewState(newstate);
        c->addAffectedNodes(children);
        c->markAsChangingChildrenList();
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

void gui::actions::GridActions::tryChangeColumnCountForGrid(
    sad::layouts::Grid* grid,
    size_t columns,
    bool from_editor
)
{
    if (columns != grid->columns())
    {
        sad::layouts::Grid* g = grid;

        sad::Vector<sad::SceneNode*> children = g->children();
        picojson::value oldstate(picojson::object_type, false);
        g->save(oldstate);

        g->setColumns(columns);
        this->updateParentGridsRecursively(grid);
        if (!from_editor)
        {
            if (m_editor->shared()->selectedGrid() == grid)
            {
                this->updateOnlyGridPropertiesInUI(gui::actions::GridActions::GridUpdateOptions::GGAUO_Cols);
            }
        }

        picojson::value newstate(picojson::object_type, false);
        g->save(newstate);

        if (from_editor || (m_editor->shared()->selectedGrid() == grid))
        {
            this->updateCellBrowser();
        }
        for(size_t i = 0; i < children.size(); i++)
        {
            if (g->find(children[i]).exists())
            {
                this->insertNodeToGridEntry(children[i], g);
            }
            else
            {
                this->eraseNodeToGridEntry(children[i]);
            }
        }

        history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Cols>* c = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Cols>(g);
        c->saveOldState(oldstate);
        c->saveNewState(newstate);
        c->addAffectedNodes(children);
        c->markAsChangingChildrenList();

        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
QCheckBox* gui::actions::GridActions::propagateCheckboxForPadding(gui::actions::GridActions::GridUpdateOptions opts)
{
    assert(
        opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding
    );

    QCheckBox* c = nullptr;

    gui::uiblocks::UILayoutBlock* blk = m_editor->uiBlocks()->uiLayoutBlock();
    // ReSharper disable once CppIncompleteSwitchStatement
    switch(opts)
    {
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding:
            c = blk->cbLayoutPaddingTopPropagate;
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding:
            c = blk->cbLayoutPaddingBottomPropagate;
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding:
            c = blk->cbLayoutPaddingLeftPropagate;
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding:
            c = blk->cbLayoutPaddingRightPropagate;
            break;
    }
    return c;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::applyPaddingChangeToGrid(
    gui::actions::GridActions::GridUpdateOptions opts,
    sad::layouts::Grid* grid,
    double new_value,
    bool propagate
)
{
    assert(
        opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding
    );
    // ReSharper disable once CppIncompleteSwitchStatement
    switch(opts)
    {
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding:
            grid->setPaddingTop(new_value, propagate);
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding:
            grid->setPaddingBottom(new_value, propagate);
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding:
            grid->setPaddingLeft(new_value, propagate);
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding:
            grid->setPaddingRight(new_value, propagate);
            break;
    }
}

void gui::actions::GridActions::tryChangePaddingForGrid(
    gui::actions::GridActions::GridUpdateOptions opts,
    sad::layouts::Grid* grid,
    double value,
    bool propagate,
    bool from_editor
)
{
    assert(
        opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding
    ||  opts == gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding
    );
    double old_value = 0;
    // ReSharper disable once CppIncompleteSwitchStatement
    switch(opts)
    {
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding:
            old_value = grid->paddingTop();
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding:
            old_value = grid->paddingBottom();
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding:
            old_value = grid->paddingLeft();
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding:
            old_value = grid->paddingRight();
            break;
    }
    if (value != old_value)
    {
        sad::layouts::Grid* g = grid;
        sad::Vector<sad::SceneNode*> children = g->children();
        picojson::value oldstate(picojson::object_type, false);
        g->save(oldstate);

        this->applyPaddingChangeToGrid(opts, g, value, propagate);
        this->updateParentGridsRecursively(g);

        picojson::value newstate(picojson::object_type, false);
        g->save(newstate);

        if (from_editor || m_editor->shared()->selectedGrid() == g)
        {
            if (!from_editor)
            {
                this->updateOnlyGridPropertiesInUI(opts);
            }
            this->updateRegion();
            if (propagate)
            {
                this->updateCellBrowser();
            }
        }

        history::Command* c = this->makePaddingChangeCommand(
            opts,
            g,
            oldstate,
            newstate,
            children,
            propagate
        );
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

void gui::actions::GridActions::paddingChanged(
    gui::actions::GridActions::GridUpdateOptions opts,
    double new_value
)
{
    sad::layouts::Grid* g = m_editor->shared()->activeGrid();
    QCheckBox* cb = this->propagateCheckboxForPadding(opts);
    bool propagate = cb->checkState() == Qt::Checked;
    if (g)
    {
        this->applyPaddingChangeToGrid(opts, g, new_value, propagate);
        if (propagate)
        {
            this->updateCellBrowser();
        }
        this->updateRegion();
    }
    else
    {
        g = m_editor->shared()->selectedGrid();
        if (g)
        {
            this->tryChangePaddingForGrid(
                opts,
                g,
                new_value,
                propagate,
                true
            );
        }
    }
}

bool gui::actions::GridActions::tryPerformMergeOrSplit(
    bool merge,
    sad::layouts::Grid* grid,
    int row,
    int col,
    int row_span,
    int col_span,
    bool from_editor
)
{
    if (row < 0 || col < 0 || row_span <= 0 || col_span <= 0)
    {
        return false;
    }
    if (row_span == 1 && col_span == 1)
    {
        return false;
    }


    history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Cells>* c = new history::layouts::Change<gui::actions::GridActions::GridUpdateOptions::GGAUO_Cells>(grid);
    c->saveOldState();
    c->addAffectedNodes(grid->children());

    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    bool result = false;
    if (merge)
    {
        result = grid->merge(row, col, row_span, col_span);
    }
    else
    {
        result = grid->split(row, col, row_span, col_span);
    }

    if (result)
    {
        this->updateParentGridsRecursively(grid);
    }

    c->saveNewState();

    if (result)
    {
        if (grid == m_editor->shared()->selectedGrid())
        {
            this->updateCellBrowser();
            this->updateRegion();
        }
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
    else
    {
        delete c;
    }
    return result;
}


void gui::actions::GridActions::tryMoveSelectedGridByKeyboard(const sad::input::KeyPressEvent& ev) 
{
    sad::Point2D p = keyToVector(ev.Key);
    if (sad::non_fuzzy_zero(p.x()) || sad::non_fuzzy_zero(p.y()))
    {
        sad::layouts::Grid* g = m_editor->shared()->activeGrid();
        if (g)
        {
            sad::Rect2D r = g->area();
            for(size_t i = 0; i < 4; i++)
            {
                r[i] += p;
            }
            g->setArea(r);
            this->updateOnlyGridPropertiesInUI(gui::actions::GridActions::GridUpdateOptions::GGAUO_Area);
            this->tryUpdateRegionsInChildren(g->children());
        }
        else
        {
            g = m_editor->shared()->selectedGrid();
            if (g && (this->parentGridFor(g) == nullptr))
            {
                sad::Rect2D r = g->area();
                for(size_t i = 0; i < 4; i++)
                {
                    r[i] += p;
                }
                if (m_editor->machine()->isInState("layouts/moving"))
                {
                    g->setArea(r);
                    this->updateOnlyGridPropertiesInUI(gui::actions::GridActions::GridUpdateOptions::GGAUO_Area);
                    this->tryUpdateRegionsInChildren(g->children());
                }
                else
                {
                    this->tryChangeAreaForGrid(g, r, true);
                }
            }
        }
    }
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

void gui::actions::GridActions::removeGridClicked()
{
    if (m_editor->isInEditingState())
        return;
    this->scriptableRemoveGrid(nullptr, true);
}

void gui::actions::GridActions::areaChanged(QRectF new_area)
{
    sad::layouts::Grid* g = m_editor->shared()->activeGrid();
    sad::Rect2D new_value;
    core::typeconverters::QRectFToSadRect2D::convert(new_area, new_value);
    if (g)
    {
        g->setArea(new_value);
        if (!sad::equal(new_value, g->area()))
        {
            this->updateRegion();
        }
    }
    else
    {
        g = m_editor->shared()->selectedGrid();
        if (g)
        {
            this->tryChangeAreaForGrid(g, new_value, true);
        }
    }
}

void gui::actions::GridActions::navigateCellRow(int new_value)
{
    int row = new_value;
    int column = m_editor->uiBlocks()->uiLayoutBlock()->spnLayoutNavigateCellY->value();
    this->tryNavigateToCell(row, column);
}

void gui::actions::GridActions::navigateCellColumn(int new_value)
{
    int row = m_editor->uiBlocks()->uiLayoutBlock()->spnLayoutNavigateCellX->value();
    int column = new_value;
    this->tryNavigateToCell(row, column);    
}

void gui::actions::GridActions::rowCountChanged(int new_value)
{
    sad::layouts::Grid* g = m_editor->shared()->activeGrid();
    if (g)
    {
        g->setRows(new_value);  
        this->updateCellBrowser();
    }
    else
    {
        g = m_editor->shared()->selectedGrid();
        if (g)
        {
            this->tryChangeRowCountForGrid(g, static_cast<size_t>(new_value), true);
        }
    }
}

void gui::actions::GridActions::columnCountChanged(int new_value)
{
    sad::layouts::Grid* g = m_editor->shared()->activeGrid();
    if (g)
    {
        g->setColumns(new_value); 
        this->updateCellBrowser();
    }
    else
    {
        g = m_editor->shared()->selectedGrid();
        if (g)
        {
            this->tryChangeColumnCountForGrid(g, new_value, true);
        }
    }    
}

void gui::actions::GridActions::fixedWidthClicked(bool new_value)
{
    sad::layouts::Grid* g = m_editor->shared()->activeGrid();
    if (g)
    {
        g->setFixedWidth(new_value); 
        this->updateRegion();
    }
    else
    {
        g = m_editor->shared()->selectedGrid();
        if (g)
        {
            tryChangeFixedWidthForGrid(g, new_value, true);            
        }
    }
}

void gui::actions::GridActions::fixedHeightClicked(bool new_value)
{
    sad::layouts::Grid* g = m_editor->shared()->activeGrid();
    if (g)
    {
        g->setFixedHeight(new_value); 
        this->updateRegion();
    }
    else
    {
        g = m_editor->shared()->selectedGrid();
        if (g)
        {
             tryChangeFixedHeightForGrid(g, new_value, true);
        }
    }
}

void gui::actions::GridActions::topPaddingChanged(double new_value)
{
    this->paddingChanged(gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding, new_value);
}

void gui::actions::GridActions::bottomPaddingChanged(double new_value)
{
    this->paddingChanged(gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding, new_value);
}

void gui::actions::GridActions::leftPaddingChanged(double new_value)
{
    this->paddingChanged(gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding, new_value);
}

void gui::actions::GridActions::rightPaddingChanged(double new_value)
{
    this->paddingChanged(gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding, new_value);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void gui::actions::GridActions::addGridByStretchingClicked()
{
    if (m_editor->isInEditingState())
        return;
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
            m_editor->shared()->setSelectedGrid(nullptr);
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
                this->tryChangeNameForGrid(grid, new_name, true);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellWidthChanged(size_t row, size_t col, sad::layouts::LengthValue new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setWidth(new_value);
        }
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_Width, sad::layouts::LengthValue>* c =
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_Width, sad::layouts::LengthValue>(grid, row, col, "width");
                c->setOldValue(grid->cell(row, col)->width());
                c->setNewValue(new_value);
                c->markAsCouldChangeRegion();
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellHeightChanged(size_t row, size_t col, sad::layouts::LengthValue new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setHeight(new_value);
        }
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_Height, sad::layouts::LengthValue>* c =
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_Height, sad::layouts::LengthValue>(grid, row, col, "height");
                c->setOldValue(grid->cell(row, col)->height());
                c->setNewValue(new_value);
                c->markAsCouldChangeRegion();
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellHorizontalAlignmentChanged(size_t row, size_t col, sad::layouts::HorizontalAlignment new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setHorizontalAlignment(new_value);            
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_HorizontalAlignment, sad::layouts::HorizontalAlignment>* c = 
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_HorizontalAlignment, sad::layouts::HorizontalAlignment>(grid, row, col, "halign");
                c->setOldValue(grid->cell(row, col)->horizontalAlignment());
                c->setNewValue(new_value);
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellVerticalAlignmentChanged(size_t row, size_t col, sad::layouts::VerticalAlignment new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setVerticalAlignment(new_value);            
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_VerticalAlignment, sad::layouts::VerticalAlignment>* c = 
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_VerticalAlignment, sad::layouts::VerticalAlignment>(grid, row, col, "valign");
                c->setOldValue(grid->cell(row, col)->verticalAlignment());
                c->setNewValue(new_value);
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellStackingTypeChanged(size_t row, size_t col, sad::layouts::StackingType new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setStackingType(new_value);            
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_StackingType, sad::layouts::StackingType>* c = 
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_StackingType, sad::layouts::StackingType>(grid, row, col, "stacking_type");
                c->setOldValue(grid->cell(row, col)->stackingType());
                c->setNewValue(new_value);
                c->markAsCouldChangeRegion();
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellTopPaddingChanged(size_t row, size_t col, double new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setPaddingTop(new_value);            
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_TopPadding, double>* c = 
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_TopPadding, double>(grid, row, col, "padding_top");
                c->setOldValue(grid->cell(row, col)->paddingTop());
                c->setNewValue(new_value);
                c->markAsCouldChangeRegion();
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellBottomPaddingChanged(size_t row, size_t col, double new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setPaddingTop(new_value);            
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_BottomPadding, double>* c = 
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_BottomPadding, double>(grid, row, col, "padding_bottom");
                c->setOldValue(grid->cell(row, col)->paddingBottom());
                c->setNewValue(new_value);
                c->markAsCouldChangeRegion();
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellLeftPaddingChanged(size_t row, size_t col, double new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setPaddingLeft(new_value);            
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_LeftPadding, double>* c = 
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_LeftPadding, double>(grid, row, col, "padding_left");
                c->setOldValue(grid->cell(row, col)->paddingLeft());
                c->setNewValue(new_value);
                c->markAsCouldChangeRegion();
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellRightPaddingChanged(size_t row, size_t col, double new_value)
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        if (grid)
        {
            grid->cell(row, col)->setPaddingRight(new_value);            
        } 
        else
        {
            grid = m_editor->shared()->selectedGrid();
            if (grid)
            {
                history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_RightPadding, double>* c = 
                    new history::layouts::ChangeCell<gui::actions::GridActions::CellUpdateOptions::GCAUO_RightPadding, double>(grid, row, col, "padding_right");
                c->setOldValue(grid->cell(row, col)->paddingRight());
                c->setNewValue(new_value);
                c->markAsCouldChangeRegion();
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellChildAdded(size_t row, size_t col, unsigned long long majorid, QString node_name)
{
    Q_UNUSED(node_name);
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
            history::layouts::AddChild* c = new history::layouts::AddChild(g, row, col, o, oldarea.value());
            c->commitWithoutUpdatingUI(m_editor);
            m_editor->history()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellChildRemoved(size_t row, size_t col, size_t pos)
{
    sad::layouts::Grid* g = m_editor->shared()->selectedGrid();
    if (!g)
    {
        return;
    }
    sad::SceneNode* node = g->cell(row, col)->child(pos);
    if (node)
    {
        history::layouts::RemoveChild* c = new history::layouts::RemoveChild(g, row, col, pos, node);
        c->commitWithoutUpdatingUI(m_editor);
        m_editor->history()->add(c);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellCleared(size_t row, size_t col)
{
    sad::layouts::Grid* g = m_editor->shared()->selectedGrid();
    if (!g)
    {
        return;
    }
    sad::Vector<sad::SceneNode*> children = g->cell(row,col)->children();
    if (children.size())
    {
        history::layouts::ClearCell* c = new history::layouts::ClearCell(g, row, col, children);
        c->commitWithoutUpdatingUI(m_editor);
        m_editor->history()->add(c);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::cellChildrenSwapped(size_t row, size_t col, size_t pos1, size_t pos2)
{
    sad::layouts::Grid* g = m_editor->shared()->selectedGrid();
    if (!g)
    {
        return;
    }

    history::layouts::SwapChildren* c = new history::layouts::SwapChildren(g, row, col, pos1, pos2);
    c->commitWithoutUpdatingUI(m_editor);
    m_editor->history()->add(c);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::GridActions::mergeButtonClicked()
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        bool active = (grid != nullptr);
        if (!grid)
        {
            grid = m_editor->shared()->selectedGrid();
        }
        if (grid)
        {
            gui::CellRegion region = this->getSelectedCellRegion();
            if (region.Flags != gui::CellRegion::Flags::GCRF_EMPTY)
            {
                if (region.Flags == gui::CellRegion::Flags::GCRF_INVALID)
                {
                    QMessageBox::critical(nullptr, "Invalid selected region", "To merge cells in region, please select a valid rectangular region");
                }
                else
                {
                    size_t row = region.Region.p0().x();
                    size_t col = region.Region.p0().y();
                    size_t rowspan = region.Region.p2().x() - region.Region.p0().x() + 1;
                    size_t colspan = region.Region.p2().y() - region.Region.p0().y() + 1;
                    if (rowspan != 1 || colspan != 1)
                    {
                        if (active)
                        {
                            grid->merge(row, col, rowspan, colspan);
                            this->updateCellBrowser();
                            this->updateRegion();
                        }
                        else
                        {
                            this->tryPerformMergeOrSplit(
                                true,
                                grid,
                                row,
                                col,
                                rowspan,
                                colspan,
                                true
                            );
                        }
                    }
                }
            }
        }
    }
}

void gui::actions::GridActions::splitButtonClicked()
{
    if (m_editor)
    {
        sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
        bool active = (grid != nullptr);
        if (!grid)
        {
            grid = m_editor->shared()->selectedGrid();
        }
        if (grid)
        {
            gui::CellRegion region = this->getSelectedCellRegion();
            if (region.Flags != gui::CellRegion::Flags::GCRF_EMPTY)
            {
                if (region.Flags == gui::CellRegion::Flags::GCRF_INVALID)
                {
                    QMessageBox::critical(nullptr, "Invalid selected region", "To merge cells in region, please select a valid rectangular region");
                }
                else
                {
                    size_t row = region.Region.p0().x();
                    size_t col = region.Region.p0().y();
                    size_t rowspan = region.Region.p2().x() - region.Region.p0().x() + 1;
                    size_t colspan = region.Region.p2().y() - region.Region.p0().y() + 1;
                    if (rowspan != 1 || colspan != 1)
                    {
                        if (active)
                        {
                            grid->split(row, col, rowspan, colspan);
                            this->updateCellBrowser();
                            this->updateRegion();
                        }
                        else
                        {
                            this->tryPerformMergeOrSplit(
                                false,
                                grid,
                                row,
                                col,
                                rowspan,
                                colspan,
                                true
                            );
                        }
                    }
                }
            }
        }
    }
}

// =============================== PRIVATE METHODS ===============================

void gui::actions::GridActions::tryNavigateToCell(size_t row, size_t column)
{
    sad::layouts::Grid* grid = m_editor->shared()->activeGrid();    
    if (!grid)
    {
        grid = m_editor->shared()->selectedGrid();
    }

    if (!grid)
    {
        return;
    }
    if (row < grid->rows() && column < grid->columns())
    {
        // Change values of row and column, if cell is merged
        sad::layouts::Cell* cell = grid->cell(row, column);
        row = cell->Row;
        column = cell->Col;

        if (m_cell_editors.contains(row))
        {
            QHash<size_t, gui::layouts::LayoutCellEdit*>& part =  m_cell_editors[row];
            if (part.contains(column))
            {
                gui::layouts::LayoutCellEdit* editor = part[column];
                // Set margin to bigger to ensure, we navigate to top-left point
                QScrollArea* area =  m_editor->uiBlocks()->uiLayoutBlock()->tblLayoutCells; 
                area->ensureVisible(editor->x(), editor->y(), 0, 0);
                /* Added small padding */
                area->ensureVisible(editor->x() + area->width(), editor->y() + area->height() - 20, 0, 0);
            }
        }
    }
}


gui::CellRegion gui::actions::GridActions::getSelectedCellRegion() const
{
    gui::CellRegion result;
    result.Flags = gui::CellRegion::Flags::GCRF_EMPTY;
    sad::layouts::Grid* grid = m_editor->shared()->activeGrid();
    if (!grid)
    {
        grid = m_editor->shared()->selectedGrid();
    }

    if (!grid)
    {
        return result;
    }

    sad::Vector<sad::layouts::Cell*> selectedcells;
    for(QHash<size_t, QHash<size_t, gui::layouts::LayoutCellEdit*> >::const_iterator it = m_cell_editors.constBegin(); 
        it != m_cell_editors.constEnd();
        ++it)
    {
        size_t row = it.key();
        const QHash<size_t, gui::layouts::LayoutCellEdit*>&part = it.value();
        for(QHash<size_t, gui::layouts::LayoutCellEdit*>::const_iterator jt = part.constBegin(); jt != part.constEnd(); ++jt)
        {
            size_t col = jt.key();
            gui::layouts::LayoutCellEdit* celleditor = jt.value();
            if (celleditor->checked())
            {
                selectedcells << grid->cell(row, col);
            }
        }
    }

    if (selectedcells.size())
    {
        size_t currowmin = selectedcells[0]->Row;
        size_t curcolmin = selectedcells[0]->Col;
        size_t currowmax  = currowmin + selectedcells[0]->rowSpan() - 1;
        size_t curcolmax = curcolmin + selectedcells[0]->colSpan() - 1;
        result.Flags = gui::CellRegion::Flags::GCRF_OK;
        for(size_t i = 1; i < selectedcells.size(); i++)
        {
            size_t rowmin = selectedcells[i]->Row;
            size_t colmin = selectedcells[i]->Col;
            size_t rowmax  = rowmin + selectedcells[i]->rowSpan() - 1;
            size_t colmax = colmin + selectedcells[i]->colSpan() - 1;
            currowmin = std::min(rowmin, currowmin);
            curcolmin = std::min(colmin, curcolmin);
            currowmax = std::max(rowmax, currowmax);
            curcolmax = std::max(colmax, curcolmax);            
        }
        result.Region = sad::Rect2I(currowmin, curcolmin, currowmax, curcolmax);
        bool valid = true;
        for(size_t row = currowmin; row <= currowmax; row++)
        {
            for(size_t col = curcolmin; col <= curcolmax; col++)
            {
                if (m_cell_editors.contains(row))
                {
                    const QHash<size_t, gui::layouts::LayoutCellEdit*>&part = m_cell_editors[row];
                    if (part.contains(col))
                    {
                        valid = valid && part[col]->checked();
                    }
                }
            }
        }

        if (!valid)
        {
            result.Flags = gui::CellRegion::Flags::GCRF_INVALID;
        }
    }
    return result;
}

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
    QListWidget* list_of_grids = layout_blk->lstLayoutGridList;
    bool state = list_of_grids->blockSignals(true);
    list_of_grids->setCurrentRow(list_of_grids->count() - 1);
    list_of_grids->blockSignals(state);
    list_of_grids->update();

    m_editor->renderGrids()->add(grid);
    m_editor->shared()->setActiveGrid(grid);
    this->updateRegion();
    this->updateCellBrowser(true);
    this->setChildEditingEnabled(false);

    this->m_previous_machine_state = m_editor->machine()->currentState();
    return grid;
}

void gui::actions::GridActions::makeBuckets(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs, gui::SortingBuckets& buckets) const
{
    buckets.clear();
    for(size_t i  = 0; i < parent_pairs.size(); i++)
    {
        sad::SceneNode* node = parent_pairs[i].p1();
        gui::GridPosition position = parent_pairs[i].p2();
        gui::CellLocation location(position.Grid, position.Row, position.Col);
        gui::NodeLocationInCell locincell(node, position.Pos);
        bool found = false;
        for(size_t j = 0; (j < buckets.size()) && !found; j++)
        {
            if (buckets[j].Cell == location)
            {
                buckets[j].List << locincell;
                found = true;
            }
        }
        if (!found)
        {
            gui::SortingBucket bucket;
            bucket.Cell = location;
            bucket.List << locincell;
            buckets << bucket;
        }
    }

    for(size_t i = 0; i < buckets.size(); i++)
    {
        std::sort(buckets[i].List.begin(), buckets[i].List.end());
    }
}

history::Command* gui::actions::GridActions::makePaddingChangeCommand(
    gui::actions::GridActions::GridUpdateOptions opts,
    sad::layouts::Grid* g,
    const picojson::value& old_state,
    const picojson::value& new_state,
    const sad::Vector<sad::SceneNode*>& children,
    bool propagate
)
{
    history::Command* c = nullptr;
    // ReSharper disable once CppIncompleteSwitchStatement
    switch(opts)
    {
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding:
            c = this->makePaddingChangeCommand<gui::actions::GridActions::GridUpdateOptions::GGAUO_TopPadding>(
                g,
                old_state,
                new_state,
                children,
                propagate
            );
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding:
            c = this->makePaddingChangeCommand<gui::actions::GridActions::GridUpdateOptions::GGAUO_BottomPadding>(
                g,
                old_state,
                new_state,
                children,
                propagate
            );
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding:
            c = this->makePaddingChangeCommand<gui::actions::GridActions::GridUpdateOptions::GGAUO_LeftPadding>(
                g,
                old_state,
                new_state,
                children,
                propagate
            );
            break;
        case gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding:
            c = this->makePaddingChangeCommand<gui::actions::GridActions::GridUpdateOptions::GGAUO_RightPadding>(
                g,
                old_state,
                new_state,
                children,
                propagate
            );
            break;
    }
    return c;
}

template<
    gui::actions::GridActions::GridUpdateOptions _Opts
>
// ReSharper disable once CppMemberFunctionMayBeConst
history::Command* gui::actions::GridActions::makePaddingChangeCommand(
    sad::layouts::Grid* g,
    const picojson::value& old_state,
    const picojson::value& new_state,
    const sad::Vector<sad::SceneNode*>& children,
    bool propagate
)
{
    history::layouts::Change<_Opts>* c = new history::layouts::Change<_Opts>(g);
    c->saveOldState(old_state);
    c->saveNewState(new_state);
    c->addAffectedNodes(children);
    c->toggleWhetherShouldUpdateCells(propagate);
    return c;
}
