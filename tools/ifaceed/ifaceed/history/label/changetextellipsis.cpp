#include "changetextellipsis.h"

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

// ReSharper disable once CppUnusedIncludeDirective
#include "../../closuremethodcall.h"

history::label::ChangeTextEllipsis::ChangeTextEllipsis(
    sad::SceneNode* d,
    unsigned int old_value,
    unsigned int new_value

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "textellipsisposition",
    old_value,
    new_value
)
{
    m_affects_parent_grid = true;
}

history::label::ChangeTextEllipsis::~ChangeTextEllipsis()
{

}

void history::label::ChangeTextEllipsis::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->cmbLabelTextEllipsis,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
