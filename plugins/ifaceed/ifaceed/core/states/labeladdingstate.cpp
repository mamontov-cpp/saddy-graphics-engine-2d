#include "labeladdingstate.h"
#include "../ifaceeditor.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "../../objects/abstractscreenobject.h"
#include "../../mainpanel.h"
#include "../../history/newcommand.h"

void LabelAddingState::onMouseMove(const sad::Event & ev)
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	hPointF center(0,0);
	hRectF region = o->region();
	for (int i=0;i<4;i++)
		center += region[i];
	center/=4;
	hPointF  pos = o->getProperty("pos")->get(ed->logContext())->get<hPointF>(ed->logContext());
	pos-=center;
	pos.setX(pos.x() + ev.x);
	pos.setY(pos.y() + ev.y);
	o->getProperty("pos")->set(sad::Variant(pos),ed->logContext());
}

void LabelAddingState::enter()
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; IFaceEditor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); ed->highlightState("Place a label");  )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void LabelAddingState::leave()
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


void LabelAddingState::onWheel(const sad::Event & ev)
{
	float dangle = (ev.delta < 0)?-0.07:0.07;
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	MainPanel * p = ed->panel();
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	float a = o->getProperty("angle")->get(ed->logContext())->get<float>(ed->logContext());
	a+=dangle;
	CLOSURE
	CLOSURE_DATA( MainPanel * p; float angle; )
	CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); )
	INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
	SUBMITCLOSURE( ed->emitClosure );
	o->getProperty("angle")->set(sad::Variant((float)a),ed->logContext());
}


void LabelAddingState::onMouseDown(const sad::Event & ev)
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	NewCommand * c = new NewCommand(ed->result(), o);
	ed->history()->add(c);
	c->commit(ed->logContext());
	ed->behaviourSharedData()->setActiveObject(NULL);
	ed->behaviourSharedData()->setSelectedObject(o);
	this->behaviour()->enterState("selected");
}

void LabelAddingState::onKeyDown(const sad::Event & ev)
{
	if (ev.key == KEY_ESC) 
	{
		IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
		AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
		ed->behaviourSharedData()->setActiveObject(NULL);
		InterlockedScene * scene = static_cast<InterlockedScene*>(ed->scene());
		scene->remove(o);
		this->behaviour()->cancelState();
	}
}