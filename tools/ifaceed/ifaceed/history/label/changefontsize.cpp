#include "changefontsize.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/fontsizewidget/fontsizewidget.h"

#include "../../blockedclosuremethodcall.h"

history::label::ChangeFontSize::ChangeFontSize(
    sad::SceneNode* d,
    unsigned int old_value,
    unsigned int new_value

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "fontsize",
    old_value,
    new_value
)
{
    m_affects_parent_grid = true;
}

history::label::ChangeFontSize::~ChangeFontSize()
{

}

void history::label::ChangeFontSize::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->fswLabelFontSize,
            &gui::fontsizewidget::FontSizeWidget::setValue,
            value
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
