#include "changeoptions.h"

#include <sprite2d.h>

#include "../../gui/scenenodeactions.h"

#include "../../gui/resourcetreewidget/resourcetreewidget.h"

#include "../../blockedclosuremethodcall.h"

#include "../../mainpanel.h"

history::sprite2d::ChangeOptions::ChangeOptions(
        sad::SceneNode* d, 
        const sad::Rect2D& oldarea,
        const sad::String& oldvalue, 
        const sad::String& newvalue
) 
: history::scenenodes::ChangeProperty<sad::String>(d, "options", oldvalue, newvalue),
m_oldarea(oldarea)
{
    
}

history::sprite2d::ChangeOptions::~ChangeOptions()
{
    
}

void history::sprite2d::ChangeOptions::rollback(core::Editor* e)
{
    this->history::scenenodes::ChangeProperty<sad::String>::rollback(e);
    m_node->setProperty("area", m_oldarea);
    if (e)
    {
        if (e->isNodeSelected(m_node))
        {
            e->panel()->sceneNodeActions()->updateRegionForNode();
        }
    }
}

void history::sprite2d::ChangeOptions::updateUI(
    core::Editor* e, 
    const sad::String& value
)
{
    if (!e)
        return;
    e->emitClosure( blocked_bind(e->panel()->UI()->rtwSpriteSprite,
        &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName,
        value
    ));
    sad::Renderer::ref()->lockRendering();
    // Force updating region immediately
    m_node->rendererChanged();
    if (e)
    {
        if (e->isNodeSelected(m_node))
        {
            e->panel()->sceneNodeActions()->updateRegionForNode();
        }
    }
    sad::Renderer::ref()->unlockRendering();
}
