#include "changemaximallinescount.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QSpinBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeMaximalLinesCount::ChangeMaximalLinesCount(
    sad::SceneNode* d,
    unsigned int old_value,
    unsigned int new_value

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "maximallinescount",
    old_value,
    new_value
)
{
    m_affects_parent_grid = true;
}

history::label::ChangeMaximalLinesCount::~ChangeMaximalLinesCount()
{

}

void history::label::ChangeMaximalLinesCount::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->spbMaximalLinesCount,
            &QSpinBox::setValue,
            value
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
