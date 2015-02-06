#include "wayactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../history/ways/waysnew.h"
#include "../history/ways/waysremove.h"
#include "../history/ways/wayschangename.h"
#include "../history/ways/wayschangetotaltime.h"
#include "../history/ways/wayschangeclosed.h"
#include "../history/ways/wayswaypointnew.h"
#include "../history/ways/wayswaypointremove.h"
#include "../history/ways/wayswaypointchange.h"
#include "../history/ways/wayswaypointswap.h"

#include <renderer.h>
#include <fuzzyequal.h>

#include <p2d/app/way.h>

#include <animations/animationswaymoving.h>
#include <animations/animationswayinstance.h>


#include <QListWidgetItem>

Q_DECLARE_METATYPE(sad::p2d::app::Way*)

gui::WayActions::WayActions(QObject* parent) : QObject(parent)
{
	
}

gui::WayActions::~WayActions()
{
	
}

void gui::WayActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::WayActions::panel() const
{
	return m_panel;
}

void gui::WayActions::moveWayPoint(const sad::input::MouseMoveEvent& e)
{
    core::Shared* s = m_panel->editor()->shared();
    sad::p2d::app::Way* w = s->selectedWay();
    int index = s->wayPointPosition();
    sad::Point2D newpoint = s->oldPoint() + (e.pos2D() - s->pivotPoint());
    w->setPoint(index, newpoint);
    m_panel->editor()->emitClosure(bind(this, &gui::WayActions::updateWayPoint, index, newpoint));
    m_panel->editor()->emitClosure(bind(this, &gui::WayActions::viewPoint, index));
}

void gui::WayActions::commitWayPointMoving(const sad::input::MouseReleaseEvent& e)
{
    sad::input::MouseMoveEvent ev;
    ev.setPoint(e.Point3D);
    this->moveWayPoint(ev);

    core::Shared* s = m_panel->editor()->shared();
    sad::p2d::app::Way* w = s->selectedWay();
    int index = s->wayPointPosition();
    sad::Point2D newpoint = w->wayPoints()[index];

    history::Command* c = new history::ways::WayPointChange(
        w,
        index,
        s->oldPoint(),
        newpoint
    );
    m_panel->editor()->history()->add(c);

    m_panel->editor()->machine()->enterState("ways/selected");
}

void gui::WayActions::updateWayPoint(int row, const sad::Point2D& p)
{
    m_panel->UI()->lstWayPoints->item(row)->setText(m_panel->nameForPoint(p));
}

void gui::WayActions::addWay()
{
    sad::p2d::app::Way* w = new sad::p2d::app::Way();
    w->setObjectName(m_panel->UI()->txtWayName->text().toStdString());
    if (m_panel->UI()->cbWayClosed->checkState() == Qt::Checked) {
        w->makeClosed();
    } else {
        w->makeOpen();
    }
    w->setTotalTime(m_panel->UI()->dsbWayTotalTime->value());
    sad::Renderer::ref()->database("")->table("ways")->add(w);
    history::ways::New* c = new history::ways::New(w);
    c->commit(m_panel->editor());
    m_panel->editor()->history()->add(c);
    m_panel->editor()->machine()->enterState("ways/selected");
    m_panel->editor()->shared()->setSelectedWay(w);
    m_panel->UI()->lstWays->setCurrentRow(m_panel->UI()->lstWays->count() - 1);
}


void gui::WayActions::removeWayFromDatabase(sad::p2d::app::Way* w, bool fromeditor, int row)
{
	sad::Vector<sad::db::Object*> animationlist;
	sad::Renderer::ref()->database("")->table("animations")->objects(animationlist);

	if (row == -1)
	{
		row = this->m_panel->findWayInList(w);
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

	int waymovingcombopos = m_panel->findInComboBox(m_panel->UI()->cmbWayAnimationWay, w);
	int wayinstancecombopos = m_panel->findInComboBox(m_panel->UI()->cmbWayAnimationInstanceWay, w);

    history::ways::Remove* c = new history::ways::Remove(w, row);
    c->setDependencies(dependentanimations, waymovingcombopos, wayinstancecombopos, dependentinstances);
	c->commit(m_panel->editor());
    core::Editor* editor = m_panel->editor();
	if (fromeditor)
	{
		editor->history()->add(c);
	} 
	else
	{
		editor->currentBatchCommand()->add(c);
	}
}
void gui::WayActions::removeWay()
{
    int row = m_panel->UI()->lstWays->currentRow();
    if (row > -1)
    {
        QVariant variant = m_panel->UI()->lstWays->item(row)->data(Qt::UserRole);
        sad::p2d::app::Way* w = variant.value<sad::p2d::app::Way*>();

		this->removeWayFromDatabase(w, true, row);
    }
}

void gui::WayActions::wayChanged(int i)
{
    if (i >= 0)
    {
        QListWidgetItem* item = m_panel->UI()->lstWays->item(i);
        QVariant v = item->data(Qt::UserRole);
        sad::p2d::app::Way* w = v.value<sad::p2d::app::Way*>();
        m_panel->editor()->machine()->enterState("ways/selected");
        m_panel->editor()->shared()->setSelectedWay(w);
        m_panel->updateUIForSelectedWay();
    }
    else
    {
        m_panel->editor()->machine()->enterState("ways/idle");
        m_panel->editor()->shared()->setSelectedWay(NULL);
    }
}

void gui::WayActions::nameEdited(const QString& name)
{
    sad::String newvalue = name.toStdString();
    sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
        sad::String oldvalue =  w->objectName();
        if (newvalue != oldvalue)
        {
            w->setObjectName(newvalue);
            m_panel->updateWayName(w);
            m_panel->editor()->history()->add(new history::ways::ChangeName(w, oldvalue, newvalue));
        }
    }
}


void gui::WayActions::closednessChanged(bool state)
{
    sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
        bool oldvalue =  w->closed();
        if (state != oldvalue)
        {
            w->setClosed(state);
            m_panel->editor()->history()->add(new history::ways::ChangeClosed(w, oldvalue, state));
        }
    }
}


void gui::WayActions::totalTimeChanged(double value)
{
    sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
        double oldvalue =  w->totalTime();
        if (value != oldvalue)
        {
            w->setTotalTime(value);
            m_panel->editor()->history()->add(new history::ways::ChangeTotalTime(w, oldvalue, value));
        }
    }
}

void gui::WayActions::addWayPoint()
{
    sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
        history::Command* c = new history::ways::WayPointNew(w);
        c->commit(m_panel->editor());
        m_panel->editor()->history()->add(c);

        m_panel->UI()->lstWayPoints->setCurrentRow(m_panel->UI()->lstWayPoints->count() - 1);
        this->viewPoint(w->wayPoints().size() - 1);
    }
}

void gui::WayActions::removeWayPoint()
{
	sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
		int row = m_panel->UI()->lstWayPoints->currentRow();
		if (row >= 0 && row < m_panel->UI()->lstWayPoints->count())
		{
			history::Command* c = new history::ways::WayPointRemove(w, row);
			c->commit(m_panel->editor());
		    m_panel->editor()->history()->add(c);
		}
	}
}

void gui::WayActions::viewPoint(int i)
{
    sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
        if (i >= 0 && i <= w->wayPoints().count())
        {
            sad::Point2D p = w->wayPoints()[i];
            invoke_blocked(m_panel->UI()->dsbWayPointX, &QDoubleSpinBox::setValue, p.x());
            invoke_blocked(m_panel->UI()->dsbWayPointY, &QDoubleSpinBox::setValue, p.y());
        }
    }
}


void gui::WayActions::wayPointXChanged(double value)
{
	sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
		int row = m_panel->UI()->lstWayPoints->currentRow();
		if (row >= 0 && row < m_panel->UI()->lstWayPoints->count())
		{
			sad::Point2D oldvalue = w->wayPoints()[row];
			sad::Point2D newvalue(value, m_panel->UI()->dsbWayPointY->value());
			if (sad::is_fuzzy_equal(newvalue.x(), oldvalue.x()) == false)
			{
				history::Command* c = new history::ways::WayPointChange(w, row, oldvalue, newvalue);
				c->commit(m_panel->editor());
			    m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::WayActions::wayPointYChanged(double value)
{
	sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
		int row = m_panel->UI()->lstWayPoints->currentRow();
		if (row >= 0 && row < m_panel->UI()->lstWayPoints->count())
		{
			sad::Point2D oldvalue = w->wayPoints()[row];
			sad::Point2D newvalue(m_panel->UI()->dsbWayPointX->value(), value);
			if (sad::is_fuzzy_equal(newvalue.y(), oldvalue.y()) == false)
			{
				history::Command* c = new history::ways::WayPointChange(w, row, oldvalue, newvalue);
				c->commit(m_panel->editor());
			    m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::WayActions::wayPointMoveBack()
{
	sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
		int row = m_panel->UI()->lstWayPoints->currentRow();
		if (row > 0 && row < m_panel->UI()->lstWayPoints->count())
		{
			history::Command* c = new history::ways::WayPointSwap(w, row - 1, row);
			c->commit(m_panel->editor());
		    m_panel->editor()->history()->add(c);
		}
	}
}

void gui::WayActions::wayPointMoveFront()
{
	sad::p2d::app::Way* w = m_panel->editor()->shared()->selectedWay();
    if (w)
    {
		int row = m_panel->UI()->lstWayPoints->currentRow();
		if (row >= 0 && row < m_panel->UI()->lstWayPoints->count() - 1)
		{
			history::Command* c = new history::ways::WayPointSwap(w, row, row + 1);
			c->commit(m_panel->editor());
		    m_panel->editor()->history()->add(c);
		}
	}
}
