#include "instancesremove.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"

Q_DECLARE_METATYPE(sad::animations::Instance*) //-V566

history::instances::Remove::Remove(
    sad::animations::Instance* d,
    int position,
    const sad::Vector<sad::Pair<sad::animations::Group*, int> >& dependent_groups
) : m_node(d), m_position_in_main_list(position), m_dependent_groups(dependent_groups)
{
    m_node->addRef();
}

history::instances::Remove::~Remove()
{
    m_node->delRef();
}

void history::instances::Remove::commit(core::Editor * ob)
{
    m_node->Active = false;
    for(size_t i = 0 ; i < m_dependent_groups.size(); i++)
    {
        m_dependent_groups[i]._1()->removeAsLink(m_node);
    }

    if (ob)
    {
        ob->emitClosure(bind(
            this,
            &history::instances::Remove::removeItem,			
            ob->uiBlocks()->uiAnimationInstanceBlock()->lstAnimationInstances	
        ));

        ob->emitClosure(bind(
            ob->actions()->instanceActions(),
            &gui::actions::AnimationInstanceActions::updateGroupInstanceList
        ));
    }
}

void history::instances::Remove::rollback(core::Editor * ob)
{
    m_node->Active = false;
    for(size_t i = 0 ; i < m_dependent_groups.size(); i++)
    {
        m_dependent_groups[i]._1()->insertAsLink(m_dependent_groups[i].p2(), m_node);
    }

    if (ob)
    {
        ob->emitClosure(bind(
            this,
            &history::instances::Remove::insertItem,			
            ob->uiBlocks()->uiAnimationInstanceBlock()->lstAnimationInstances,	
            ob->actions()->instanceActions()->nameForInstance(m_node)
        ));

        ob->emitClosure(bind(
            ob->actions()->instanceActions(),
            &gui::actions::AnimationInstanceActions::updateGroupInstanceList
        ));
    }
}

void history::instances::Remove::removeItem(QListWidget* widget)
{
    delete widget->takeItem(m_position_in_main_list);
}

void history::instances::Remove::insertItem(QListWidget* widget, const QString& name)
{
    QVariant v;
    v.setValue(m_node);

    QListWidgetItem* item = new QListWidgetItem(name);
    item->setData(Qt::UserRole, v);
    widget->insertItem(m_position_in_main_list, item);
}