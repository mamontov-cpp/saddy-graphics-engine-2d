#include "changemaximallinewidth.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"
#include "../../gui/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeMaximalLineWidth::ChangeMaximalLineWidth(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "maximallinewidth",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeMaximalLineWidth::~ChangeMaximalLineWidth()
{

}

void history::label::ChangeMaximalLineWidth::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->spbMaximalLineWidth,
            &QSpinBox::setValue,
            value
        )
    );
    e->panel()->sceneNodeActions()->updateRegionForNode();
}
