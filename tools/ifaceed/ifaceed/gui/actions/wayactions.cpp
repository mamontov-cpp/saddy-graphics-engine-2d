#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>

#include "wayactions.h"

#include "../../qstdstring.h"
#include "../../keytovector.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"

#include "../../history/ways/waysnew.h"
#include "../../history/ways/waysremove.h"
#include "../../history/ways/wayschangename.h"
#include "../../history/ways/wayschangetotaltime.h"
#include "../../history/ways/wayschangeclosed.h"
#include "../../history/ways/wayswaypointnew.h"
#include "../../history/ways/wayswaypointremove.h"
#include "../../history/ways/wayswaypointchange.h"
#include "../../history/ways/wayswaypointswap.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uiwayblock.h"
#include "../uiblocks/uianimationblock.h"
#include "../uiblocks/uianimationinstanceblock.h"

#include "../actions/actions.h"
#include "../actions/gridactions.h"

#include "../mainpanelproxy.h"

#include <renderer.h>
#include <fuzzyequal.h>

#include <p2d/app/way.h>

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <animations/animationswaymoving.h>
#include <animations/animationswayinstance.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QListWidgetItem>

Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

gui::actions::WayActions::WayActions(QObject* parent) : QObject(parent)
{
    
}

gui::actions::WayActions::~WayActions()
{
    
}

void gui::actions::WayActions::moveWayPoint(const sad::input::MouseMoveEvent& e)
{
    core::Shared* s = m_editor->shared();
    sad::p2d::app::Way* w = s->selectedWay();
    int index = s->wayPointPosition();
    sad::Point2D newpoint = s->oldPoint() + (e.pos2D() - s->pivotPoint());
    w->setPoint(index, newpoint);
    m_editor->emitClosure(bind(this, &gui::actions::WayActions::updateWayPoint, index, newpoint));
    m_editor->emitClosure(bind(this, &gui::actions::WayActions::viewPoint, index));
}

void gui::actions::WayActions::commitWayPointMoving(const sad::input::MouseReleaseEvent& e)
{
    sad::input::MouseMoveEvent ev;
    ev.setPoint(e.Point3D);
    this->moveWayPoint(ev);

    core::Shared* s = m_editor->shared();
    sad::p2d::app::Way* w = s->selectedWay();
    int index = s->wayPointPosition();
    sad::Point2D newpoint = w->wayPoints()[index];

    history::Command* c = new history::ways::WayPointChange(
        w,
        index,
        s->oldPoint(),
        newpoint
    );
    m_editor->history()->add(c);

    m_editor->machine()->enterState("ways/selected");
}

void gui::actions::WayActions::updateWayPoint(int row, const sad::Point2D& p)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    blk->lstWayPoints->item(row)->setText(this->nameForPoint(p));
}

void gui::actions::WayActions::addWay()
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 

    sad::p2d::app::Way* w = new sad::p2d::app::Way();
    w->setObjectName(Q2STDSTRING(blk->txtWayName->text()));
    if (blk->cbWayClosed->checkState() == Qt::Checked) 
    {
        w->makeClosed();
    } 
    else 
    {
        w->makeOpen();
    }
    w->setTotalTime(blk->dsbWayTotalTime->value());
    sad::Renderer::ref()->database("")->table("ways")->add(w);
    history::ways::New* c = new history::ways::New(w);
    c->commit(m_editor);
    m_editor->history()->add(c);
    m_editor->machine()->enterState("ways/selected");
    m_editor->shared()->setSelectedWay(w);
    blk->lstWays->setCurrentRow(blk->lstWays->count() - 1);
}


void gui::actions::WayActions::removeWayFromDatabase(sad::p2d::app::Way* w, bool from_editor, int row)
{
    sad::Vector<sad::db::Object*> animationlist;
    sad::Renderer::ref()->database("")->table("animations")->objects(animationlist);
    gui::uiblocks::UIAnimationBlock* ablk = m_editor->uiBlocks()->uiAnimationBlock(); 
    gui::uiblocks::UIAnimationInstanceBlock* aiblk = m_editor->uiBlocks()->uiAnimationInstanceBlock(); 

    if (row == -1)
    {
        row = this->findWayInList(w);
    }

    sad::Vector<sad::animations::WayMoving*> dependentanimations;
    for(size_t i = 0; i < animationlist.size(); i++)
    {
        sad::db::Object* tmp = animationlist[i];
        if (tmp->isInstanceOf("sad::animations::WayMoving"))
        {
            sad::animations::WayMoving* a = static_cast<sad::animations::WayMoving*>(tmp);
            if (a->wayObjectId() == w->MajorId)
            {
                dependentanimations << a;
            }
        }
    }

    sad::Vector<sad::db::Object*> animationinstancelist;
    sad::Renderer::ref()->database("")->table("animationinstances")->objects(animationinstancelist);

    sad::Vector<sad::animations::WayInstance*> dependentinstances;
    for(size_t i = 0; i < animationinstancelist.size(); i++)
    {
        sad::db::Object* tmp = animationinstancelist[i];
        if (tmp->isInstanceOf("sad::animations::WayInstance"))
        {
            sad::animations::WayInstance* a = static_cast<sad::animations::WayInstance*>(tmp);
            if (a->wayMajorId() == w->MajorId)
            {
                dependentinstances << a;
            }
        }
    }

    int waymovingcombopos = this->findInComboBox(ablk->cmbWayAnimationWay, w);
    int wayinstancecombopos = this->findInComboBox(aiblk->cmbWayAnimationInstanceWay, w);

    history::ways::Remove* c = new history::ways::Remove(w, row);
    c->setDependencies(dependentanimations, waymovingcombopos, wayinstancecombopos, dependentinstances);
    c->commit(m_editor);
    core::Editor* editor = m_editor;
    if (from_editor)
    {
        editor->history()->add(c);
    } 
    else
    {
        editor->currentBatchCommand()->add(c);
    }
}



void gui::actions::WayActions::addLastWayToEnd(sad::p2d::app::Way* way)
{
    QString nameforway = this->viewableObjectName(way);

    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    gui::uiblocks::UIAnimationBlock* ablk = m_editor->uiBlocks()->uiAnimationBlock(); 
    gui::uiblocks::UIAnimationInstanceBlock* aiblk = m_editor->uiBlocks()->uiAnimationInstanceBlock(); 

    blk->lstWays->addItem(nameforway);
    
    QVariant v;
    v.setValue(way);
    blk->lstWays->item(blk->lstWays->count()-1)->setData(Qt::UserRole, v);

    ablk->cmbWayAnimationWay->addItem(nameforway, v);
    aiblk->cmbWayAnimationInstanceWay->addItem(nameforway, v);

}

void gui::actions::WayActions::removeLastWayFromWayList()
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    gui::uiblocks::UIAnimationBlock* ablk = m_editor->uiBlocks()->uiAnimationBlock(); 
    gui::uiblocks::UIAnimationInstanceBlock* aiblk = m_editor->uiBlocks()->uiAnimationInstanceBlock(); 

    if (blk->lstWays->count() > 0)
    {
        QVariant v = blk->lstWays->item(blk->lstWays->count() - 1)->data(Qt::UserRole);
        sad::p2d::app::Way* w  = v.value<sad::p2d::app::Way*>();
        if (w == m_editor->shared()->selectedWay())
        {
            m_editor->shared()->setSelectedWay(nullptr);
            m_editor->machine()->enterState("ways/idle");
        }
        delete blk->lstWays->takeItem(blk->lstWays->count() - 1);

        int pos = this->findInComboBox(ablk->cmbWayAnimationWay, w);
        if (pos >= 0)
        {
            ablk->cmbWayAnimationWay->removeItem(pos);
        }

        pos = this->findInComboBox(aiblk->cmbWayAnimationInstanceWay, w);
        if (pos >= 0)
        {
            aiblk->cmbWayAnimationInstanceWay->removeItem(pos);
        }
    }
}

void gui::actions::WayActions::insertWayToWayList(sad::p2d::app::Way* s, int position)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 

    QListWidgetItem* i = new QListWidgetItem(this->viewableObjectName(s));
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    blk->lstWays->insertItem(position, i);
}

void gui::actions::WayActions::removeWayFromWayList(int position)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 

    QVariant v = blk->lstWays->item(position)->data(Qt::UserRole);
    sad::p2d::app::Way* w  = v.value<sad::p2d::app::Way*>();
    if (w == m_editor->shared()->selectedWay())
    {
        m_editor->shared()->setSelectedWay(nullptr);
        m_editor->machine()->enterState("ways/idle");
    }
    delete blk->lstWays->takeItem(position);
}

void gui::actions::WayActions::removeWayFromWayList(sad::p2d::app::Way* s)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 

    int pos = this->findWayInList(s);
    if (s == m_editor->shared()->selectedWay())
    {
        m_editor->shared()->setSelectedWay(nullptr);
        m_editor->machine()->enterState("ways/idle");
    }
    if (pos >= 0)
    {
        delete blk->lstWays->takeItem(pos);
    }
}

int gui::actions::WayActions::findWayInList(sad::p2d::app::Way* s)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    return this->findInList(blk->lstWays, s);
}

void gui::actions::WayActions::updateWayName(sad::p2d::app::Way* s)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    gui::uiblocks::UIAnimationBlock* ablk = m_editor->uiBlocks()->uiAnimationBlock(); 
    gui::uiblocks::UIAnimationInstanceBlock* aiblk = m_editor->uiBlocks()->uiAnimationInstanceBlock(); 


    QString name = this->viewableObjectName(s);
    int row = this->findWayInList(s);
    if (row != -1)
    {
        blk->lstWays->item(row)->setText(name);
    }

    int pos = this->findInComboBox(ablk->cmbWayAnimationWay, s);
    if (pos >= 0)
    {
        ablk->cmbWayAnimationWay->setItemText(pos, name);
    }

    pos = this->findInComboBox(aiblk->cmbWayAnimationInstanceWay, s);
    if (pos >= 0)
    {
        aiblk->cmbWayAnimationInstanceWay->setItemText(pos, name);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::WayActions::removeRowInWayPointList(int row)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 

    if (row >=0  && row < blk->lstWayPoints->count()) {
        delete blk->lstWayPoints->takeItem(row);
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
QString gui::actions::WayActions::nameForPoint(const sad::Point2D& p) const
{
    return QString("(%1,%2)")
           .arg(static_cast<int>(p.x()))
           .arg(static_cast<int>(p.y()));
}

void gui::actions::WayActions::tryMoveWayByVector(const sad::input::KeyPressEvent&, const sad::Point2D& p) const
{
    if (sad::non_fuzzy_zero(p.x()) || sad::non_fuzzy_zero(p.y()))
    {
        gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
        sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
        if (w)
        {
            int row = blk->lstWayPoints->currentRow();
            if (row >= 0 && row < blk->lstWayPoints->count())
            {
                sad::Point2D old_value = w->wayPoints()[row];
                sad::Point2D new_value = old_value;
                new_value += p;
                history::Command* c = new history::ways::WayPointChange(w, row, old_value, new_value);
                c->commit(m_editor);
                if (m_editor->machine()->isInState("ways/selected/moving"))
                {
                    delete c;
                }
                else
                {
                    m_editor->history()->add(c);
                }
            }
        }
    }
}

void gui::actions::WayActions::tryMoveWayByKeys(const sad::input::KeyPressEvent& ev) const
{
    if (m_editor->machine()->isInState("ways/idle"))
    {
        if (m_editor->isInGridEditingState() && (m_editor->shared()->selectedGrid() != nullptr))
        {
            m_editor->actions()->gridActions()->tryMoveSelectedGridByKeyboard(ev);
        }
        else
        {
            m_editor->panelProxy()->handleGlobalOffsetChange(ev);
        }
    }
    else
    {
        if (m_editor->isInGridEditingState() && (m_editor->shared()->selectedGrid() != nullptr))
        {
            m_editor->actions()->gridActions()->tryMoveSelectedGridByKeyboard(ev);
        }
        else
        {
            sad::Point2D p = keyToVector(ev.Key);
            this->tryMoveWayByVector(ev, p);
        }
    }
}

void gui::actions::WayActions::removeWay()
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    int row = blk->lstWays->currentRow();
    if (row > -1)
    {
        QVariant variant = blk->lstWays->item(row)->data(Qt::UserRole);
        sad::p2d::app::Way* w = variant.value<sad::p2d::app::Way*>();

        this->removeWayFromDatabase(w, true, row);
    }
}

void gui::actions::WayActions::wayChanged(int i)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    if (i >= 0)
    {
        QListWidgetItem* item = blk->lstWays->item(i);
        QVariant v = item->data(Qt::UserRole);
        sad::p2d::app::Way* w = v.value<sad::p2d::app::Way*>();
        m_editor->machine()->enterState("ways/selected");
        m_editor->shared()->setSelectedWay(w);
        this->updateUIForSelectedWay();
    }
    else
    {
        m_editor->machine()->enterState("ways/idle");
        m_editor->shared()->setSelectedWay(nullptr);
    }
}

void gui::actions::WayActions::nameEdited(const QString& name)
{
    sad::String new_value = Q2STDSTRING(name);
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        sad::String old_value =  w->objectName();
        if (new_value != old_value)
        {
            w->setObjectName(new_value);
            this->updateWayName(w);
            m_editor->history()->add(new history::ways::ChangeName(w, old_value, new_value));
        }
    }
}


void gui::actions::WayActions::closednessChanged(bool state)
{
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        bool old_value =  w->closed();
        if (state != old_value)
        {
            w->setClosed(state);
            m_editor->history()->add(new history::ways::ChangeClosed(w, old_value, state));
        }
    }
}


void gui::actions::WayActions::totalTimeChanged(double value)
{
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        double old_value =  w->totalTime();
        if (fabs(value - old_value) > 0.0001)
        {
            w->setTotalTime(value);
            m_editor->history()->add(new history::ways::ChangeTotalTime(w, old_value, value));
        }
    }
}

void gui::actions::WayActions::addWayPoint()
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        history::Command* c = new history::ways::WayPointNew(w);
        c->commit(m_editor);
        m_editor->history()->add(c);

        blk->lstWayPoints->setCurrentRow(blk->lstWayPoints->count() - 1);
        this->viewPoint(w->wayPoints().size() - 1);
    }
}

void gui::actions::WayActions::removeWayPoint()
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        int row = blk->lstWayPoints->currentRow();
        if (row >= 0 && row < blk->lstWayPoints->count())
        {
            history::Command* c = new history::ways::WayPointRemove(w, row);
            c->commit(m_editor);
            m_editor->history()->add(c);
        }
    }
}

void gui::actions::WayActions::viewPoint(int i)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        if (i >= 0 && i < w->wayPoints().count())
        {
            sad::Point2D p = w->wayPoints()[i];
            invoke_blocked(blk->dsbWayPointX, &QDoubleSpinBox::setValue, p.x());
            invoke_blocked(blk->dsbWayPointY, &QDoubleSpinBox::setValue, p.y());
        }
    }
}


void gui::actions::WayActions::wayPointXChanged(double value)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        int row = blk->lstWayPoints->currentRow();
        if (row >= 0 && row < blk->lstWayPoints->count())
        {
            sad::Point2D old_value = w->wayPoints()[row];
            sad::Point2D new_value(value, blk->dsbWayPointY->value());
            if (sad::is_fuzzy_equal(new_value.x(), old_value.x()) == false)
            {
                history::Command* c = new history::ways::WayPointChange(w, row, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::WayActions::wayPointYChanged(double value)
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        int row = blk->lstWayPoints->currentRow();
        if (row >= 0 && row < blk->lstWayPoints->count())
        {
            sad::Point2D old_value = w->wayPoints()[row];
            sad::Point2D new_value(blk->dsbWayPointX->value(), value);
            if (sad::is_fuzzy_equal(new_value.y(), old_value.y()) == false)
            {
                history::Command* c = new history::ways::WayPointChange(w, row, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::WayActions::wayPointMoveBack()
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        int row = blk->lstWayPoints->currentRow();
        if (row > 0 && row < blk->lstWayPoints->count())
        {
            history::Command* c = new history::ways::WayPointSwap(w, row - 1, row);
            c->commit(m_editor);
            m_editor->history()->add(c);
        }
    }
}

void gui::actions::WayActions::wayPointMoveFront()
{
    gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 
    
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w)
    {
        int row = blk->lstWayPoints->currentRow();
        if (row >= 0 && row < blk->lstWayPoints->count() - 1)
        {
            history::Command* c = new history::ways::WayPointSwap(w, row, row + 1);
            c->commit(m_editor);
            m_editor->history()->add(c);
        }
    }
}


void gui::actions::WayActions::updateUIForSelectedWay()
{
    QTimer::singleShot(0, this, SLOT(updateUIForSelectedWayNow()));
}

void gui::actions::WayActions::updateUIForSelectedWayNow()
{
    if (m_editor->shared()->selectedWay())
    {
        gui::uiblocks::UIWayBlock* blk = m_editor->uiBlocks()->uiWayBlock(); 

        blk->lstWayPoints->clear();
        sad::p2d::app::Way* p = m_editor->shared()->selectedWay();
        for(size_t i = 0; i < p->wayPoints().size(); ++i)
        {
            blk->lstWayPoints->addItem(this->nameForPoint(p->wayPoints()[i]));
        }
        invoke_blocked(blk->txtWayName, &QLineEdit::setText, STD2QSTRING(p->objectName().c_str()));
        invoke_blocked(blk->dsbWayTotalTime, &QDoubleSpinBox::setValue, p->totalTime());
        invoke_blocked(blk->cbWayClosed, &QCheckBox::setCheckState,  (p->closed()) ? Qt::Checked : Qt::Unchecked);
    }
}
