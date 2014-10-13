#include "wayactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/ways/waysnew.h"

#include <renderer.h>

#include <p2d/app/way.h>

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
