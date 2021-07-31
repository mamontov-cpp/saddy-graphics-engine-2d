#include "customobjectchangeschema.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/resourcetreewidget/resourcetreewidget.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"
#include "../../gui/actions/customobjectactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uicustomobjectblock.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"
#include "../../qstdstring.h"

#include <renderer.h>

history::customobject::ChangeSchema::ChangeSchema(
    sad::db::custom::Object* d,
    const sad::String& old_value,
    const sad::String& new_value
) : history::scenenodes::ChangeProperty<sad::String>(d, "schema", old_value, new_value)
{
    // Save area
    sad::Renderer::ref()->lockRendering();
    m_old_area = d->area();
    sad::Renderer::ref()->unlockRendering();

    const sad::Hash<sad::String, sad::db::Property*>& props = d->schemaProperties();
    for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = props.const_begin();
        it != props.const_end();
        ++it
       )
    {
        sad::db::Variant* v = new sad::db::Variant();
        d->getObjectProperty(it.key())->get(d, *v);
        m_old_values.insert(it.key(), v);
    }

    m_affects_parent_grid = true;
}

history::customobject::ChangeSchema::~ChangeSchema()
{
    
}

void history::customobject::ChangeSchema::rollback(core::Editor * ob)
{
    sad::Renderer::ref()->lockRendering();        
    m_node->setProperty<sad::String>(m_property, m_old_value);
    sad::Renderer::ref()->unlockRendering();

    sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(m_node);
    if (o->schema())
    {
        sad::Renderer::ref()->lockRendering();  
        o->setArea(m_old_area);
        sad::Renderer::ref()->unlockRendering();	
        for(sad::PtrHash<sad::String, sad::db::Variant>::const_iterator it = m_old_values.const_begin();
            it != m_old_values.const_end();
            ++it
        )
        {
            m_node->getObjectProperty(it.key())->set(o,*(it.value()));
        }
    }

    tryUpdateUI(ob, m_old_value);
}


void history::customobject::ChangeSchema::updateUI(
    core::Editor* e, 
    const sad::String& value
)
{
    if (e)
    {
        sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(m_node);
        e->actions()->sceneNodeActions()->updateRegionForNode();
        e->emitClosure(bind(e->actions()->customObjectActions(), &gui::actions::CustomObjectActions::fillCustomObjectProperties, o));
        e->emitClosure(blocked_bind(
            e->uiBlocks()->uiCustomObjectBlock()->rtwCustomObjectSchemas,
            &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName,
            value
        ));
    }
}

