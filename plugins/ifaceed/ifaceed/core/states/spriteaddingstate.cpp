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
	CLOSURE_CODE( p->setAddingEnabled(false); ed->highlightState("Place a sprite");  )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void SimpleSpriteAddingState::onMouseDown(const sad::Event & ev)
{

}

void DiagonalSpriteAddingState::enter()
{
	IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
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

