#include "changetext.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/labelactions.h"

history::label::ChangeText::ChangeText(
    sad::SceneNode* d,
    const sad::String& oldvalue,
    const sad::String& newvalue

) : m_node(d), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
    m_node->addRef();
}

history::label::ChangeText::~ChangeText()
{
    m_node->delRef();
}

 void history::label::ChangeText::commit(core::Editor * ob)
{
    m_node->setProperty<sad::String>("text", m_newvalue);
    tryUpdateUI(ob, m_newvalue);
}

 void history::label::ChangeText::rollback(core::Editor * ob)
{
    m_node->setProperty<sad::String>("text", m_oldvalue);
    tryUpdateUI(ob, m_oldvalue);
}


void history::label::ChangeText::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    if (m_node  == e->shared()->selectedObject() && e->machine()->isInState("selected"))
    {
        e->emitClosure( blocked_bind(
                e->panel()->UI()->txtLabelText,
                &QPlainTextEdit::setPlainText,
                QString(value.c_str())
            )
        );
        e->panel()->labelActions()->updateRegionForLabel();
    }
}
