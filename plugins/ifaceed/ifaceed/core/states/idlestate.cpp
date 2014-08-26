#include "idlestate.h"

#include "../ifaceeditor.h"

#include "../../core/editorbehaviour.h"
#include "../../core/editorbehaviourshareddata.h"

#include "../../objects/screentemplate.h"

#include <closure.h>
#include <marshal/serializableobject.h>
#include <keycodes.h>

void IdleState::onMouseDown(const sad::input::MousePressEvent & ev)
{
	if (ev.Button == sad::MouseLeft) {
		IFaceEditor * ed = this->editor();
		CLOSURE
		CLOSURE_DATA( IFaceEditor * e; sad::Point2D m_p; )
		CLOSURE_CODE( this->e->trySelectObject(m_p, true); )
		INITCLOSURE( CLSET(e, ed); CLSET(m_p, ev.pos2D()) );
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void IdleState::enter()
{
	IFaceEditor * ed = this->editor();
	ed->shdata()->setSelectedObject(NULL); // Disable borders
	CLOSURE
	CLOSURE_DATA( IFaceEditor * e; )
	CLOSURE_CODE( this->e->highlightState("Idle"); )
	INITCLOSURE( CLSET(e, ed);  );
	SUBMITCLOSURE( ed->emitClosure );
}
