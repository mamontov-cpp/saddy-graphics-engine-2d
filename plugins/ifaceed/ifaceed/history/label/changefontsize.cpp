#include "changefontsize.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeFontSize::ChangeFontSize(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

) : m_node(d), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
    m_node->addRef();
}

history::label::ChangeFontSize::~ChangeFontSize()
{
    m_node->delRef();
}

 void history::label::ChangeFontSize::commit(core::Editor * ob)
{
    m_node->setProperty("fontsize", m_newvalue);
    tryUpdateUI(ob, m_newvalue);
}

 void history::label::ChangeFontSize::rollback(core::Editor * ob)
{
    m_node->setProperty("fontsize", m_oldvalue);
    tryUpdateUI(ob, m_oldvalue);
}


void history::label::ChangeFontSize::tryUpdateUI(core::Editor* e, unsigned int value)
{
    if (m_node  == e->shared()->selectedObject() && e->machine()->isInState("selected"))
    {
        e->emitClosure( blocked_bind(
                e->panel()->UI()->fswLabelFontSize,
                &gui::fontsizewidget::FontSizeWidget::setValue,
                value
            )
        );
        e->panel()->labelActions()->updateRegionForLabel();
    }
}
