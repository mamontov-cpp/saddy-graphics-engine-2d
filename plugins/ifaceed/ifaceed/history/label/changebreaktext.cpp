#include "changebreaktext.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"
#include "../../gui/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeBreakText::ChangeBreakText(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "breaktext",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeBreakText::~ChangeBreakText()
{

}

void history::label::ChangeBreakText::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->cmbLabelBreakText,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->panel()->sceneNodeActions()->updateRegionForNode();
}
