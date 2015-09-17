#include "customobjectchangeschema.h"

#include "../../gui/resourcetreewidget/resourcetreewidget.h"

#include "../../gui/scenenodeactions.h"

#include "../../mainpanel.h"
#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"
#include "../../qstdstring.h"

#include <renderer.h>

history::customobject::ChangeSchema::ChangeSchema(
    sad::db::custom::Object* d,
    const sad::String& oldvalue,
    const sad::String& newvalue
) : history::scenenodes::ChangeProperty<sad::String>(d, "schema", oldvalue, newvalue)
{
    // Save area
    sad::Renderer::ref()->lockRendering();
    m_oldarea = d->area();
    sad::Renderer::ref()->unlockRendering();

    const sad::Hash<sad::String, sad::db::Property*>& props = d->schemaProperties();
    for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = props.const_begin();
        it != props.const_end();
        ++it
       )
    {
        sad::db::Variant* v = new sad::db::Variant();
        d->getObjectProperty(it.key())->get(d, *v);
        m_oldvalues.insert(it.key(), v);
    }
}

history::customobject::ChangeSchema::~ChangeSchema()
{
    
}

void history::customobject::ChangeSchema::rollback(core::Editor * ob)
{
    sad::Renderer::ref()->lockRendering();        
    m_node->setProperty<sad::String>(m_property, m_oldvalue);
    sad::Renderer::ref()->unlockRendering();

    sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(m_node);
    if (o->schema())
    {
        sad::Renderer::ref()->lockRendering();  
        o->setArea(m_oldarea);
        sad::Renderer::ref()->unlockRendering();	
        for(sad::PtrHash<sad::String, sad::db::Variant>::const_iterator it = m_oldvalues.const_begin();
            it != m_oldvalues.const_end();
            ++it
        )
        {
            m_node->getObjectProperty(it.key())->set(o,*(it.value()));
        }
    }

    tryUpdateUI(ob, m_oldvalue);
}


void history::customobject::ChangeSchema::updateUI(
    core::Editor* e, 
    const sad::String& value
)
{
    if (e)
    {
        sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(m_node);
        e->panel()->sceneNodeActions()->updateRegionForNode();
        e->emitClosure(bind(e->panel(), &MainPanel::fillCustomObjectProperties, o));
        e->emitClosure(blocked_bind(
            e->panel()->UI()->rtwCustomObjectSchemas,
            &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName,
            value
        ));
    }
}

