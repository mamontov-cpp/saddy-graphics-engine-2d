#include "changetext.h"

#include <QPlainTextEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

history::label::ChangeText::ChangeText(
    sad::SceneNode* d,
    const sad::String& oldvalue,
    const sad::String& newvalue

) : history::scenenodes::ChangeProperty<sad::String>(
    d,
    "text",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeText::~ChangeText()
{

}

void history::label::ChangeText::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->txtLabelText,
            &QPlainTextEdit::setPlainText,
            STD2QSTRING(value)
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
