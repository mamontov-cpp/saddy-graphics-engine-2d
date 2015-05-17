#include "wayswaypointchange.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

history::ways::WayPointChange::WayPointChange(
	sad::p2d::app::Way* w, 
	int pos, 
	const sad::Point2D& oldv, 
	const sad::Point2D& newv
) : m_way(w), m_position(pos), m_old_point(oldv), m_new_point(newv)
{
	m_way->addRef();
}

history::ways::WayPointChange::~WayPointChange()
{
	m_way->delRef();
}

void history::ways::WayPointChange::commit(core::Editor * ob)
{
	m_way->setPoint(m_position, m_new_point);
	if (ob)
	{
		ob->emitClosure(bind(this, &history::ways::WayPointChange::tryUpdateUI, ob, m_new_point));
	}
}

void history::ways::WayPointChange::rollback(core::Editor * ob)
{
	m_way->setPoint(m_position, m_old_point);
	if (ob)
	{
		ob->emitClosure(bind(this, &history::ways::WayPointChange::tryUpdateUI, ob, m_old_point));
	}
}

void history::ways::WayPointChange::tryUpdateUI(core::Editor* ob, const sad::Point2D& p)
{
	if (ob->shared()->selectedWay() == m_way)
	{
		Ui::MainPanelClass* ui = ob->panel()->UI(); 
		ui->lstWayPoints->item(m_position)->setText(ob->panel()->nameForPoint(p));
		if (m_position == ui->lstWayPoints->currentRow())
		{
			invoke_blocked(ui->dsbWayPointX, &QDoubleSpinBox::setValue, p.x());
			invoke_blocked(ui->dsbWayPointY, &QDoubleSpinBox::setValue, p.y());
		}
	}
}