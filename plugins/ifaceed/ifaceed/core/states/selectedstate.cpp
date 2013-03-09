#include "selectedstate.h"
#include "../ifaceeditor.h"
#include "../objectborders.h"
#include "../../utils/closure.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "../../objects/screentemplate.h"
#include "../../objects/abstractscreenobject.h"
#include <marshal/serializableobject.h>


SelectedState::SelectedState()
{
	m_substate = SSSS_SIMPLESELECTED;
	m_navigationstart = 0;
	m_navposition = 0;
}


void SelectedState::enterNavigation(const std::vector<hst::string> & chain)
{
	m_substate = SSSS_SELECTEDNAVIGATION;
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
		if (m_navposition == m_chain.size())
			m_navposition = 0;
	}
	else 
	{
		m_navposition -= 1;
		if (m_navposition == -1)
			m_navposition = m_chain.size() -1;
	}
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
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
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());	
	AbstractScreenObject * o =	ed->behaviourSharedData()->selectedObject();
	if (m_substate == SSSS_SIMPLESELECTED && o->rotatable()) 
	{
		float dangle = (ev.delta < 0)? (- ROTATION_ANGLE_STEP ) : ROTATION_ANGLE_STEP;
		MainPanel * p = ed->panel();
		float a = o->getProperty("angle")->get(ed->log())->get<float>(ed->log());
		a+=dangle;
		CLOSURE
		CLOSURE_DATA( MainPanel * p; float angle; )
		CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); )
		INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void SelectedState::onMouseDown(const sad::Event & ev)
{
	if (ev.key == MOUSE_BUTTON_LEFT) {
		IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
		hPointF p(ev.x, ev.y);
		AbstractScreenObject * o = ed->behaviourSharedData()->selectedObject();
		hst::vector<BorderHotSpots> r = ed->selectionBorder()->isWithin(p);
		if (r.count() != 0) 
		{
			ed->log()->debug(QString::number(r[0]));
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