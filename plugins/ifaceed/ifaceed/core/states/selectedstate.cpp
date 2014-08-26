#include "selectedstate.h"

#include "../objectborders.h"

#include "../../core/editor.h"
#include "../../core/editorbehaviour.h"
#include "../../core/shared.h"

#include "../../objects/screentemplate.h"
#include "../../objects/abstractscreenobject.h"

#include "../../history/editorhistory.h"
#include "../../history/movecommand.h"

#include <closure.h>

#include <db/load.h>
#include <db/save.h>

#include <marshal/serializableobject.h>

#include <p2d/vector.h>

SelectedState::SelectedState()
{
	m_substate = SSSS_SIMPLESELECTED;
	m_movement_substate = SSMSS_NOMOVEMENT;
	m_navigationstart = 0;
	m_navposition = 0;
}


void SelectedState::enterNavigation(const std::vector<sad::String> & chain)
{
	m_substate = SSSS_SELECTEDNAVIGATION;
	m_movement_substate = SSMSS_NOMOVEMENT;
	m_navigationstart = clock();
	m_chain = chain;
	m_navposition = 0;
}

void SelectedState::navigateOffset(bool next)
{
	if (m_chain.size() == 0) 
		return;
	if (next) 
	{
		m_navposition += 1;
        if ((unsigned int)m_navposition == m_chain.size())
			m_navposition = 0;
	}
	else 
	{
		m_navposition -= 1;
		if (m_navposition == -1)
			m_navposition = m_chain.size() -1;
	}
	core::Editor * ed = this->editor();
	ScreenTemplate * t = ed->result();
	SerializableObject * so = t->object(m_chain[m_navposition]);
	if (so != NULL) 
	{
		AbstractScreenObject * o = static_cast<AbstractScreenObject *>(so);
		CLOSURE
		CLOSURE_DATA( core::Editor * m_e; AbstractScreenObject * m_o; )
		CLOSURE_CODE( 
			this->m_e->behaviourSharedData()->setSelectedObject(this->m_o);
			this->m_e->showObjectStats(this->m_o); 
		)
		INITCLOSURE( CLSET(m_e, ed); CLSET(m_o, o) );
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void SelectedState::onWheel(const sad::input::MouseWheelEvent & ev)
{
	if (m_substate == SSSS_SELECTEDNAVIGATION) 
	{
		clock_t cur = clock();
		if ((cur-m_navigationstart) / CLOCKS_PER_SEC < SSSS_NAVIGATION_COOLDOWN)
		{
			m_navigationstart = clock();
			this->navigateOffset(ev.Delta > 0);
		}
		else 
		{
			m_substate = SSSS_SIMPLESELECTED;
		}
	}
	core::Editor * ed = this->editor();	
	AbstractScreenObject * o =	this->shdata()->selectedObject();
	if (m_substate == SSSS_SIMPLESELECTED && o->rotatable()) 
	{
		float dangle = (ev.Delta < 0)? (- ROTATION_ANGLE_STEP ) : ROTATION_ANGLE_STEP;
		MainPanel * p = ed->panel();
		//float a = o->getProperty("angle")->get<float>().value();
		//a+=dangle;
		/*
		TODO: Reimplement
		CLOSURE
		CLOSURE_DATA( MainPanel * p; float angle; )
		CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); p->setRegionParameters(); )
		INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
		SUBMITCLOSURE( ed->emitClosure );
		*/
	}
}

void SelectedState::onMouseDown(const sad::input::MousePressEvent & ev)
{
	SL_SCOPE(QString("SelectedState::onMouseDown(%1,%2)")
		     .arg(ev.pos2D().x(), ev.pos2D().y())
			 .toStdString()
			);
	if (ev.Button == sad::MouseLeft) {
		core::Editor * ed = this->editor();
		sad::Point2D p = ev.pos2D();
		AbstractScreenObject * o = this->shdata()->selectedObject();
		sad::Vector<BorderHotSpots> r = ed->selectionBorder()->isWithin(p);
		if (r.count() != 0) 
		{
			BorderHotSpots bhs = r[r.count()-1]; 
			if (bhs == BHS_REMOVE) 
			{
				SL_DEBUG("Hit remove hotspot on selected object");
				ed->tryEraseObject();
			}
			else
			{
				m_movement_substate = SSMSS_RESIZING;
				m_resizingsubstate.oldRect = o->region();
				m_resizingsubstate.oldPoint = p;
				if (bhs == BHS_LEFT)  m_resizingsubstate.action   = ResizingStateAction(0,3,1,2);
				if (bhs == BHS_TOP)  m_resizingsubstate.action = ResizingStateAction(0,1,3,2);
				if (bhs == BHS_RIGHT)  m_resizingsubstate.action  = ResizingStateAction(1,2,0,3);
				if (bhs == BHS_BOTTOM)  m_resizingsubstate.action    = ResizingStateAction(2,3,1,0);
			}
		}
		else
		{
			if (ed->result()->isObjectInPicked(p,o) 
				&& ed->result()->pickedIsBackground(p,o) == false) 
			{
				m_substate = SSSS_SIMPLESELECTED;
				sad::Rect2D region = o->region();
				m_picked_old_center = (region[0] + region[2]) / 2;
				m_picked_point = p;
				m_movement_substate = SSMSS_MOVING;
			} 
			else 
			{
				CLOSURE
				CLOSURE_DATA( core::Editor * e; sad::Point2D m_p; )
				CLOSURE_CODE( this->e->trySelectObject(m_p, false); )
				INITCLOSURE( CLSET(e, ed); CLSET(m_p, p) );
				SUBMITCLOSURE( ed->emitClosure );
			}
		}
	}
}


void SelectedState::onMouseMove(const sad::input::MouseMoveEvent & ev)
{
	core::Editor * ed = this->editor();
	sad::Point2D p = ev.pos2D();
	AbstractScreenObject * o = this->shdata()->selectedObject();
	sad::log::Log * log = sad::log::Log::ref();
	if (m_movement_substate == SSMSS_MOVING)
	{	
		o->moveCenterTo(m_picked_old_center + (p - m_picked_point));
		CLOSURE
		CLOSURE_DATA( core::Editor * e;  )
		CLOSURE_CODE( this->e->panel()->setRegionParameters(); )
		INITCLOSURE( CLSET(e, ed);  );
		SUBMITCLOSURE( ed->emitClosure );
	}
	if (m_movement_substate == SSMSS_RESIZING)
	{
		sad::Point2D d = p - m_resizingsubstate.oldPoint;
		sad::Rect2D nr  = m_resizingsubstate.action.apply(m_resizingsubstate.oldRect, d);
		o->setRotatedRectangle(nr, o->prop<float>("angle", log));
		CLOSURE
		CLOSURE_DATA( core::Editor * e;  )
		CLOSURE_CODE( this->e->panel()->setRegionParameters(); )
		INITCLOSURE( CLSET(e, ed);  );
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void SelectedState::onMouseUp(const sad::input::MouseReleaseEvent & ev)
{
	core::Editor * ed = this->editor();
	AbstractScreenObject * o = this->shdata()->selectedObject();
	sad::input::MouseMoveEvent movingevent;
	movingevent.Point3D = ev.Point3D;
	sad::log::Log * log = sad::log::Log::ref();
	if (m_movement_substate == SSMSS_MOVING)
	{		
		this->onMouseMove(movingevent);		
		sad::Rect2D region = o->region();
		sad::Point2D newcenter = (region[0] + region[2]) / 2;
		ed->history()->add(new MoveCommand(o, m_picked_old_center, newcenter));
		m_movement_substate = SSMSS_NOMOVEMENT;
	}
	if (m_movement_substate == SSMSS_RESIZING)
	{
		this->onMouseMove(movingevent);
		ResizeCommand * r = new ResizeCommand(o, m_resizingsubstate.oldRect, o->region(), o->prop<float>("angle", log));
		ed->history()->add(r);
		m_movement_substate = SSMSS_NOMOVEMENT;
	}
}


void SelectedState::enter()
{
	core::Editor * ed = this->editor();
    //AbstractScreenObject * o = this->shdata()->selectedObject();
	ed->submitEvent("selected_enter", sad::db::Variant(0));
	m_movement_substate = SSMSS_NOMOVEMENT;
	CLOSURE
	CLOSURE_DATA( core::Editor * e; )
	CLOSURE_CODE( this->e->highlightState("Selected"); )
	INITCLOSURE( CLSET(e, ed); );
	SUBMITCLOSURE( ed->emitClosure );
}

void SelectedState::leave()
{
	core::Editor * ed = this->editor();
    //AbstractScreenObject * o = this->shdata()->selectedObject();
	ed->submitEvent("selected_enter", sad::db::Variant(0));
}


void SelectedState::onKeyDown(const sad::input::KeyPressEvent & ev)
{
	if (ev.Key == sad::Esc)
	{
		core::Editor * ed = this->editor();
		this->shdata()->setSelectedObject(NULL);
		ed->currentBehaviour()->enterState("idle");
	}
}

sad::Rect2D ResizingStateAction::apply(const sad::Rect2D & x, const sad::Point2D & xd)
{
	sad::Rect2D xs  = x;
	sad::Point2D xc = (x[p0] + x[p1]) / 2;
	sad::Point2D xcp = (x[p2] + x[p3]) / 2;
	sad::p2d::Vector e = sad::p2d::unit(xc - xcp);
	float t = sad::p2d::scalar(e, xd );
	xs[p0] +=  e * t;
	xs[p1] +=  e * t;
	return xs;
}
