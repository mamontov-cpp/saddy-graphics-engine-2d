#include "changelinespacing.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeLineSpacing::ChangeLineSpacing(
    sad::SceneNode* d,
    float oldvalue,
    float newvalue
) : m_node(d), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
    m_node->addRef();
}

history::label::ChangeLineSpacing::~ChangeLineSpacing()
{
    m_node->delRef();
}

 void history::label::ChangeLineSpacing::commit(core::Editor * ob)
{
    m_node->setProperty("linespacing", m_newvalue);
    tryUpdateUI(ob, m_newvalue);
}

 void history::label::ChangeLineSpacing::rollback(core::Editor * ob)
{
    m_node->setProperty("linespacing", m_oldvalue);
    tryUpdateUI(ob, m_oldvalue);
}


void history::label::ChangeLineSpacing::tryUpdateUI(core::Editor* e, float value)
{
    if (m_node  == e->shared()->selectedObject() && e->machine()->isInState("selected"))
    {
        e->emitClosure( blocked_bind(
                e->panel()->UI()->dsbLineSpacingRatio,
                &QDoubleSpinBox::setValue,
                static_cast<float>(value)
            )
        );
        e->panel()->labelActions()->updateRegionForLabel();
    }
}
