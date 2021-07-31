#include "changehasformatting.h"

#include <QCheckBox>

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/fontsizewidget/fontsizewidget.h"

#include "../../blockedclosuremethodcall.h"

history::label::ChangeHasFormatting::ChangeHasFormatting(
    sad::SceneNode* d,
    bool old_value,
    bool new_value

) : history::scenenodes::ChangeProperty<bool>(d, "hasformatting", old_value, new_value)
{
    m_affects_parent_grid = true;
}

history::label::ChangeHasFormatting::~ChangeHasFormatting()
{

}

void history::label::ChangeHasFormatting::updateUI(
    core::Editor* e,
    const bool& value)
{
    e->emitClosure(blocked_bind(
        e->uiBlocks()->uiLabelBlock()->cbLabelHasFormatting,
        &QCheckBox::setCheckState,
        (value) ? Qt::Checked : Qt::Unchecked
    )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
