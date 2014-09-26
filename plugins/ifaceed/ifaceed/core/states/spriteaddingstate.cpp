#include <unused.h>

#include "spriteaddingstate.h"

#include "../../core/editor.h"
#include "../../core/editorbehaviour.h"
#include "../../core/shared.h"

#include "../../objects/abstractscreenobject.h"

#include "../../mainpanel.h"

#include "../../history/newcommand.h"
#include "../../objects/screensprite.h"



void SimpleSpriteAddingState::enter()
{
	core::Editor * ed = this->editor();
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; core::Editor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  p->setRegionParameters();  
				  p->setSpriteChangingEnabled(false);
				 )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void SimpleSpriteAddingState::onMouseMove(const sad::input::MouseMoveEvent & ev)
{
	core::Editor * ed = this->editor();
	//AbstractScreenObject * o =	ed->shared()->activeObject();
	//o->moveCenterTo(ev.pos2D());

	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; core::Editor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  p->setRegionParameters();  
				 )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void SimpleSpriteAddingState::onWheel(const sad::input::MouseWheelEvent & ev)
{
	float dangle = (ev.Delta < 0)? (- ROTATION_ANGLE_STEP ) : ROTATION_ANGLE_STEP;
	core::Editor * ed = this->editor();
	MainPanel * p = ed->panel();
	//AbstractScreenObject * o =	ed->shared()->activeObject();
	//float a = o->SerializableObject::getProperty("angle")->get<float>().value();
	//a+=dangle;
	/*
	// TODO: Reimplement
	CLOSURE
	CLOSURE_DATA( MainPanel * p; float angle; )
	CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); p->setRegionParameters();   )
	INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
	SUBMITCLOSURE( ed->emitClosure );
	o->getProperty("angle")->set(sad::db::Variant((float)a));
	*/
}

void SimpleSpriteAddingState::onMouseDown(UNUSED const sad::input::MousePressEvent & ev)
{
	/*
	core::Editor * ed = this->editor();
	AbstractScreenObject * o =	ed->shared()->activeObject();
	NewCommand * c = new NewCommand(ed->result(), o);
	ed->history()->add(c);
	c->commit(ed);

	sad::Scene * scene = ed->scene();
	scene->add(o);

	ed->shared()->setActiveObject(NULL);
	ed->shared()->setSelectedObject(o);
	this->behaviour()->enterState("selected");
	*/
}

void SimpleSpriteAddingState::onKeyDown(const sad::input::KeyPressEvent & ev)
{
	if (ev.Key == sad::Esc) 
	{
        //this->editor()->tryEraseObject();
	}
}

void SimpleSpriteAddingState::leave()
{
	core::Editor * ed = this->editor();
	this->shdata()->setActiveObject(NULL);
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; )
	CLOSURE_CODE( p->setAddingEnabled(true); p->setSpriteChangingEnabled(true); )
	INITCLOSURE( CLSET(p,p) )
	SUBMITCLOSURE( ed->emitClosure );
}

void DiagonalSpriteAddingState::enter()
{
	m_substate = DSAS_INITIAL;
	core::Editor * ed = this->editor();
	sad::log::Log * log = sad::log::Log::ref();
	//ed->shared()->activeObject()->setProp<float>("angle", 0.0f, log);
	//ed->shared()->activeObject()->setProp<bool>("visibility", false, log);
	ed->shared()->toggleActiveBorder(false);

	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; core::Editor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  p->setAngleChangingEnabled(false); 
				  p->setSpriteChangingEnabled(false);
				)
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}


void DiagonalSpriteAddingState::onMouseDown(const sad::input::MousePressEvent & ev)
{
	/*
	DiagonalSpriteAddingSubState ss = m_substate;
	core::Editor * ed = this->editor();
	AbstractScreenObject * o = ed->shared()->activeObject();
	ScreenSprite * oo = static_cast<ScreenSprite *>(o);
	sad::Point2D p = ev.pos2D();
	bool highlight = false;
	sad::String highlights;
	sad::log::Log * log = sad::log::Log::ref();
	if (ss == DSAS_INITIAL)
	{
		ed->shared()->toggleActiveBorder(true);

		sad::Rect2D brect = oo->rect();
		sad::Point2D ddelta = p - brect[3];
		for(int i = 0; i < 4; i++)
		{
			brect[i] += ddelta;
		}
		oo->setProp<bool>("visibility", true, log);
		oo->setRotatedRectangle(brect, 0.0f);
		m_substate = DSAS_FIRSTCLICK;
		highlight = true;
		highlights = "Now place the bottom right point";
	}
	if (ss == DSAS_FIRSTCLICK)
	{
		sad::input::MouseMoveEvent mousemoveevent;
		mousemoveevent.Point3D = ev.Point3D;
		this->onMouseMove(mousemoveevent);
		NewCommand * c = new NewCommand(ed->result(), o);
		ed->history()->add(c);

		sad::Scene * scene = ed->scene();
		scene->add(o);

		c->commit(ed);
		ed->shared()->setActiveObject(NULL);
		ed->shared()->setSelectedObject(o);
		this->behaviour()->enterState("selected");
	}
	CLOSURE
	CLOSURE_DATA( MainPanel * p; bool h; sad::String hs; )
	CLOSURE_CODE(
		p->setRegionParameters();   
		if (h)
			p->highlightState(hs);
	)
	INITCLOSURE( CLSET(p,ed->panel()); CLSET(h, highlight); CLSET(hs, highlights);  )
	SUBMITCLOSURE( ed->emitClosure );
	*/
}

void DiagonalSpriteAddingState::leave()
{
	core::Editor * ed = this->editor();
	ed->shared()->toggleActiveBorder(true);
	ed->shared()->setActiveObject(NULL);
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; )
	CLOSURE_CODE( p->setAddingEnabled(true); 
	              p->setAngleChangingEnabled(true); 
				  p->setSpriteChangingEnabled(true);
				)
	INITCLOSURE( CLSET(p,p) )
	SUBMITCLOSURE( ed->emitClosure );
}


void DiagonalSpriteAddingState::onMouseMove(const sad::input::MouseMoveEvent & ev)
{
	/*
	core::Editor * ed = this->editor();
	AbstractScreenObject * o = ed->shared()->activeObject();
	ScreenSprite * oo = static_cast<ScreenSprite *>(o);
	if (m_substate == DSAS_FIRSTCLICK)
	{
		sad::Point2D newpoint = ev.pos2D();
		sad::Rect2D nrect = oo->rect();
		nrect[1] = newpoint;
		nrect[0].setX(nrect[3].x());
		nrect[0].setY(nrect[1].y());
		nrect[2].setX(nrect[1].x());
		nrect[2].setY(nrect[3].y());
		oo->setRotatedRectangle(nrect, 0.0f);


		CLOSURE
		CLOSURE_DATA( MainPanel * p; )
		CLOSURE_CODE(p->setRegionParameters();   )
		INITCLOSURE( CLSET(p,ed->panel());  )
		SUBMITCLOSURE( ed->emitClosure );
	}
	*/
}

void DiagonalSpriteAddingState::onKeyDown(const sad::input::KeyPressEvent & ev)
{
	if (ev.Key == sad::Esc) 
	{
        //this->editor()->tryEraseObject();
	}
}
