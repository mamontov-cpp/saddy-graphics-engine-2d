#include "spriteaddingstate.h"
#include "../ifaceeditor.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "../../objects/abstractscreenobject.h"
#include "../../mainpanel.h"
#include "../../history/newcommand.h"

void SimpleSpriteAddingState::enter()
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; IFaceEditor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  ed->highlightState("Place a sprite"); 
				  p->setRegionParameters();  
				 )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void SimpleSpriteAddingState::onMouseMove(const sad::Event & ev)
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	o->moveCenterTo(hPointF(ev.x,ev.y));

	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; IFaceEditor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  ed->highlightState("Place a sprite"); 
				  p->setRegionParameters();  
				 )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void SimpleSpriteAddingState::onWheel(const sad::Event & ev)
{
	float dangle = (ev.delta < 0)? (- ROTATION_ANGLE_STEP ) : ROTATION_ANGLE_STEP;
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	MainPanel * p = ed->panel();
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	float a = o->getProperty("angle")->get(ed->log())->get<float>(ed->log());
	a+=dangle;
	CLOSURE
	CLOSURE_DATA( MainPanel * p; float angle; )
	CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); p->setRegionParameters();   )
	INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
	SUBMITCLOSURE( ed->emitClosure );
	o->getProperty("angle")->set(sad::Variant((float)a),ed->log());
}

void SimpleSpriteAddingState::onMouseDown(const sad::Event & ev)
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	NewCommand * c = new NewCommand(ed->result(), o);
	ed->history()->add(c);
	c->commit(ed->log(), ed);
	ed->behaviourSharedData()->setActiveObject(NULL);
	ed->behaviourSharedData()->setSelectedObject(o);
	this->behaviour()->enterState("selected");
}

void SimpleSpriteAddingState::onKeyDown(const sad::Event & ev)
{
	if (ev.key == KEY_ESC) 
	{
		IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
		ed->tryEraseObject();
	}
}

void SimpleSpriteAddingState::leave()
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	ed->behaviourSharedData()->setActiveObject(NULL);
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; )
	CLOSURE_CODE( p->setAddingEnabled(true); )
	INITCLOSURE( CLSET(p,p) )
	SUBMITCLOSURE( ed->emitClosure );
}

void DiagonalSpriteAddingState::enter()
{
	m_substate = DSAS_INITIAL;
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	ed->behaviourSharedData()->activeObject()->setProp<float>("angle", 0.0f, ed->log());
	ed->behaviourSharedData()->activeObject()->setProp<bool>("visibility", false, ed->log());

	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; IFaceEditor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  p->setAngleChangingEnabled(false); 
				  ed->highlightState("Place a first point");  
				)
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}


void DiagonalSpriteAddingState::onMouseDown(const sad::Event & ev)
{
	
}

void DiagonalSpriteAddingState::leave()
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	ed->behaviourSharedData()->setActiveObject(NULL);
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; )
	CLOSURE_CODE( p->setAddingEnabled(true); p->setAngleChangingEnabled(true); )
	INITCLOSURE( CLSET(p,p) )
	SUBMITCLOSURE( ed->emitClosure );
}


void DiagonalSpriteAddingState::onMouseMove(const sad::Event & ev)
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	//o->moveCenterTo(hPointF(ev.x,ev.y));
}

void DiagonalSpriteAddingState::onKeyDown(const sad::Event & ev)
{
	if (ev.key == KEY_ESC) 
	{
		IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
		ed->tryEraseObject();
	}
}