#include "changebreaktext.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QComboBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"


history::label::ChangeBreakText::ChangeBreakText(
    sad::SceneNode* d,
    unsigned int old_value,
    unsigned int new_value

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "breaktext",
    old_value,
    new_value
)
{
    m_affects_parent_grid = true;
}

history::label::ChangeBreakText::~ChangeBreakText()
{

}

void history::label::ChangeBreakText::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->cmbLabelBreakText,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
