#include "selectedstate.h"
#include "../ifaceeditor.h"
#include "../objectborders.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "../../objects/screentemplate.h"
#include "../../objects/abstractscreenobject.h"
#include "../../history/editorhistory.h"
#include "../../history/movecommand.h"

#include <closure.h>
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
	IFaceEditor * ed = this->editor();
	ScreenTemplate * t = ed->result();
	SerializableObject * so = t->object(m_chain[m_navposition]);
	if (so != NULL) 
	{
		AbstractScreenObject * o = static_cast<AbstractScreenObject *>(so);
		CLOSURE
		CLOSURE_DATA( IFaceEditor * m_e; AbstractScreenObject * m_o; )
		CLOSURE_CODE( 
			this->m_e->behaviourSharedData()->setSelectedObject(this->m_o);
			this->m_e->showObjectStats(this->m_o); 
		)
		INITCLOSURE( CLSET(m_e, ed); CLSET(m_o, o) );
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void SelectedState::onWheel(const sad::Event & ev)
{
	if (m_substate == SSSS_SELECTEDNAVIGATION) 
	{
		clock_t cur = clock();
		if ((cur-m_navigationstart) / CLOCKS_PER_SEC < SSSS_NAVIGATION_COOLDOWN)
		{
			m_navigationstart = clock();
			this->navigateOffset(ev.delta > 0);
		}
		else 
		{
			m_substate = SSSS_SIMPLESELECTED;
		}
	}
	IFaceEditor * ed = this->editor();	
	AbstractScreenObject * o =	this->shdata()->selectedObject();
	if (m_substate == SSSS_SIMPLESELECTED && o->rotatable()) 
	{
		float dangle = (ev.delta < 0)? (- ROTATION_ANGLE_STEP ) : ROTATION_ANGLE_STEP;
		MainPanel * p = ed->panel();
		float a = o->getProperty("angle")->get(ed->log())->get<float>(ed->log());
		a+=dangle;
		CLOSURE
		CLOSURE_DATA( MainPanel * p; float angle; )
		CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); p->setRegionParameters(); )
		INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void SelectedState::onMouseDown(const sad::Event & ev)
{
	SL_SCOPE(QString("SelectedState::onMouseDown(%1,%2)").arg(ev.x, ev.y).toStdString());
	if (ev.key == MOUSE_BUTTON_LEFT) {
		IFaceEditor * ed = this->editor();
		hPointF p(ev.x, ev.y);
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
				hRectF region = o->region();
				m_picked_old_center = (region[0] + region[2]) / 2;
				m_picked_point = p;
				m_movement_substate = SSMSS_MOVING;
			} 
			else 
			{
				CLOSURE
				CLOSURE_DATA( IFaceEditor * e; hPointF m_p; )
				CLOSURE_CODE( this->e->trySelectObject(m_p, false); )
				INITCLOSURE( CLSET(e, ed); CLSET(m_p, p) );
				SUBMITCLOSURE( ed->emitClosure );
			}
		}
	}
}


void SelectedState::onMouseMove(const sad::Event & ev)
{
	IFaceEditor * ed = this->editor();
	hPointF p(ev.x, ev.y);
	AbstractScreenObject * o = this->shdata()->selectedObject();
	if (m_movement_substate == SSMSS_MOVING)
	{	
		o->moveCenterTo(m_picked_old_center + (p - m_picked_point));
		CLOSURE
		CLOSURE_DATA( IFaceEditor * e;  )
		CLOSURE_CODE( this->e->panel()->setRegionParameters(); )
		INITCLOSURE( CLSET(e, ed);  );
		SUBMITCLOSURE( ed->emitClosure );
	}
	if (m_movement_substate == SSMSS_RESIZING)
	{
		hPointF d = p - m_resizingsubstate.oldPoint;
		hRectF nr  = m_resizingsubstate.action.apply(m_resizingsubstate.oldRect, d);
		o->setRotatedRectangle(nr, o->prop<float>("angle", ed->log()));
		CLOSURE
		CLOSURE_DATA( IFaceEditor * e;  )
		CLOSURE_CODE( this->e->panel()->setRegionParameters(); )
		INITCLOSURE( CLSET(e, ed);  );
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void SelectedState::onMouseUp(const sad::Event & ev)
{
	IFaceEditor * ed = this->editor();
	AbstractScreenObject * o = this->shdata()->selectedObject();
	if (m_movement_substate == SSMSS_MOVING)
	{
		this->onMouseMove(ev);		
		hRectF region = o->region();
		hPointF newcenter = (region[0] + region[2]) / 2;
		ed->history()->add(new MoveCommand(o, m_picked_old_center, newcenter));
		m_movement_substate = SSMSS_NOMOVEMENT;
	}
	if (m_movement_substate == SSMSS_RESIZING)
	{
		this->onMouseMove(ev);
		ResizeCommand * r = new ResizeCommand(o, m_resizingsubstate.oldRect, o->region(), o->prop<float>("angle", ed->log()));
		ed->history()->add(r);
		m_movement_substate = SSMSS_NOMOVEMENT;
	}
}


void SelectedState::enter()
{
	IFaceEditor * ed = this->editor();
    //AbstractScreenObject * o = this->shdata()->selectedObject();
	ed->submitEvent("selected_enter", sad::Variant(0));
	m_movement_substate = SSMSS_NOMOVEMENT;
	CLOSURE
	CLOSURE_DATA( IFaceEditor * e; )
	CLOSURE_CODE( this->e->highlightState("Selected"); )
	INITCLOSURE( CLSET(e, ed); );
	SUBMITCLOSURE( ed->emitClosure );
}

void SelectedState::leave()
{
	IFaceEditor * ed = this->editor();
    //AbstractScreenObject * o = this->shdata()->selectedObject();
	ed->submitEvent("selected_enter", sad::Variant(0));
}


void SelectedState::onKeyDown(const sad::Event & ev)
{
	if (ev.key == KEY_ESC)
	{
		IFaceEditor * ed = this->editor();
		this->shdata()->setSelectedObject(NULL);
		ed->currentBehaviour()->enterState("idle");
	}
}

hRectF ResizingStateAction::apply(const hRectF & x, const hPointF & xd)
{
	hRectF xs  = x;
	hPointF xc = (x[p0] + x[p1]) / 2;
	hPointF xcp = (x[p2] + x[p3]) / 2;
	sad::p2d::Vector e = sad::p2d::unit(xc - xcp);
	float t = sad::p2d::scalar(e, xd );
	xs[p0] +=  e * t;
	xs[p1] +=  e * t;
	return xs;
}
