#include "removeproperty.h"

#include "../../qstdstring.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../gui/mainpanelproxy.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uicommonblock.h"

// ================================= PUBLIC METHODS =================================

history::database::RemoveProperty::RemoveProperty(gui::table::Delegate* d)
{
    m_delegate = d;
    m_delegate->addRef();

    m_property = sad::Renderer::ref()
    ->database("")
    ->propertyByName(Q2STDSTRING(d->propertyName()))
    ->clone();

    m_row = -1;    
}

history::database::RemoveProperty::~RemoveProperty()
{
    m_delegate->delRef();

    delete m_property;
}

void history::database::RemoveProperty::commit(core::Editor * ob)
{
    if (m_row == -1 && ob)
    {
        m_row = findProperty(ob);
    }

    sad::Renderer::ref()
    ->database("")
    ->removeProperty(Q2STDSTRING(m_delegate->propertyName()));
    
    m_delegate->remove();
    if (ob)
    {
        ob->panelProxy()->delegatesByName().remove(Q2STDSTRING(m_delegate->propertyName()));
    }
}

void history::database::RemoveProperty::rollback(core::Editor * ob)
{
    sad::Renderer::ref()
    ->database("")
    ->addProperty(Q2STDSTRING(m_delegate->propertyName()), m_property->clone());
    if (m_row == -1)
    {
        m_delegate->add();
    }
    else
    {
        m_delegate->insert(m_row);
    }
    ob->panelProxy()->delegatesByName().insert(Q2STDSTRING(m_delegate->propertyName()), m_delegate);
}


// ================================= PROTECTED METHODS =================================

int history::database::RemoveProperty::findProperty(core::Editor* ob) const
{
    int row = -1;
    QTableWidget* props = ob->uiBlocks()->uiCommonBlock()->twDatabaseProperties;
    for(size_t i = 0; i < props->rowCount() && m_row == -1; i++)
    {
        if (props->item(i, 0)->text() == m_delegate->propertyName())
        {
            row = i;
        }
    }
    return row;
}