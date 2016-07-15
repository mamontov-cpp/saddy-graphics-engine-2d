#include "newproperty.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>

#include "../../core/editor.h"

#include "../../gui/mainpanelproxy.h"

#include "../../qstdstring.h"

history::database::NewProperty::NewProperty(gui::table::Delegate* d)
{
    m_delegate = d;
    m_delegate->addRef();

    m_property = sad::Renderer::ref()
    ->database("")
    ->propertyByName(Q2STDSTRING(d->propertyName()))
    ->clone();
}

history::database::NewProperty::~NewProperty()
{
    m_delegate->delRef();

    delete m_property;
}

void history::database::NewProperty::commit(core::Editor * ob)
{
    sad::Renderer::ref()
    ->database("")
    ->addProperty(Q2STDSTRING(m_delegate->propertyName()), m_property->clone());

    m_delegate->add();

    ob->panelProxy()->delegatesByName().insert(Q2STDSTRING(m_delegate->propertyName()), m_delegate);
}

void history::database::NewProperty::rollback(core::Editor * ob)
{
    sad::Renderer::ref()
    ->database("")
    ->removeProperty(Q2STDSTRING(m_delegate->propertyName()));
    
    m_delegate->remove();

    ob->panelProxy()->delegatesByName().remove(Q2STDSTRING(m_delegate->propertyName()));
}
