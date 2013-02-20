#include "idlestate.h"
#include "../ifaceeditor.h"
#include "../../utils/closure.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "../../objects/screentemplate.h"
#include <marshal/serializableobject.h>


void IdleState::onMouseDown(const sad::Event & ev)
{
	if (ev.key == MOUSE_BUTTON_LEFT) {
		IFaceEditor * ed = static_cast<IFaceEditor *>(this->behaviour()->parent());
		hPointF p(ev.x, ev.y);
		CLOSURE
		CLOSURE_DATA( IFaceEditor * e; hPointF m_p; )
		CLOSURE_CODE( this->e->trySelectObject(m_p, true); )
		INITCLOSURE( CLSET(e, ed); CLSET(m_p, p) );
		SUBMITCLOSURE( ed->emitClosure );
	}
}
