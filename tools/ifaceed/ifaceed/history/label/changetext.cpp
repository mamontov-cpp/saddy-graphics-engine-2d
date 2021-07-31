#include "changetext.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QPlainTextEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../closuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

history::label::ChangeText::ChangeText(
    sad::SceneNode* d,
    const sad::String& old_value,
    const sad::String& new_value

) : history::scenenodes::ChangeProperty<sad::String>(
    d,
    "text",
    old_value,
    new_value
)
{
    m_affects_parent_grid = true;
}

history::label::ChangeText::~ChangeText()
{

}

void history::label::ChangeText::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( ::bind(
            this,
            &history::Command::blockedSetPlainTextEditText,
            e->uiBlocks()->uiLabelBlock()->txtLabelText,
            STD2QSTRING(value)
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
