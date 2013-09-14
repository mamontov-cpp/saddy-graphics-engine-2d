#include "labeladdingstate.h"
#include "../ifaceeditor.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "../../objects/abstractscreenobject.h"
#include "../../mainpanel.h"
#include "../../history/newcommand.h"

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

void LabelAddingState::onMouseMove(const sad::Event & ev)
{
	this->shdata()->activeObject()->moveCenterTo(hPointF(ev.x,ev.y));
}

void LabelAddingState::enter()
{
	IFaceEditor * ed = this->editor();
	CLOSURE
	CLOSURE_DATA( IFaceEditor * ed; )
	CLOSURE_CODE( ed->panel()->setAddingEnabled(false); ed->highlightState("Place a label");  )
	INITCLOSURE(  CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void LabelAddingState::leave()
{
	IFaceEditor * ed = this->editor();
	ed->behaviourSharedData()->setActiveObject(NULL);
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; )
	CLOSURE_CODE( p->setAddingEnabled(true); )
	INITCLOSURE( CLSET(p,p) )
	SUBMITCLOSURE( ed->emitClosure );
}


void LabelAddingState::onWheel(const sad::Event & ev)
{
	float dangle = (ev.delta < 0)? (- ROTATION_ANGLE_STEP ) : ROTATION_ANGLE_STEP;
	IFaceEditor * ed = this->editor();
	MainPanel * p = ed->panel();
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	float a = o->getProperty("angle")->get(ed->log())->get<float>(ed->log());
	a+=dangle;
	CLOSURE
	CLOSURE_DATA( MainPanel * p; float angle; )
	CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); )
	INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
	SUBMITCLOSURE( ed->emitClosure );
	o->getProperty("angle")->set(sad::Variant((float)a),ed->log());
}


void LabelAddingState::onMouseDown(UNUSED const sad::Event & ev)
{
	IFaceEditor * ed = this->editor();
	AbstractScreenObject * o =	this->shdata()->activeObject();
	NewCommand * c = new NewCommand(ed->result(), o);
	ed->history()->add(c);
	c->commit(ed->log(), ed);
	ed->behaviourSharedData()->setActiveObject(NULL);
	ed->behaviourSharedData()->setSelectedObject(o);
	this->behaviour()->enterState("selected");
}

void LabelAddingState::onKeyDown(const sad::Event & ev)
{
	if (ev.key == KEY_ESC) 
	{
		IFaceEditor * ed = this->editor();
		ed->tryEraseObject();
	}
}
