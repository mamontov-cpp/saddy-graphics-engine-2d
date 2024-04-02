#include "waysremove.h"

#include <QComboBox>

#include <animations/animationswaymoving.h>

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

Q_DECLARE_METATYPE(sad::Way*) //-V566

history::ways::Remove::Remove(sad::Way* w, int pos) 
: m_way(w), 
m_position(pos),
m_position_in_animation_combo(-1),
m_position_in_animation_instances_combo(-1)
{
    m_way->addRef();
}

history::ways::Remove::~Remove()
{
    m_way->delRef();
}

void history::ways::Remove::setDependencies(
        const sad::Vector<sad::animations::WayMoving*>& dependent_animations,
        int position_in_animation_combo,
        int position_in_animation_instances_combo,
        const sad::Vector<sad::animations::WayInstance*>& dependent_instances
)
{
    m_dependent_animations = dependent_animations;
    m_position_in_animation_combo = position_in_animation_combo;
    m_position_in_animation_instances_combo = position_in_animation_instances_combo;
    m_dependent_instances = dependent_instances;
}

void history::ways::Remove::commit(core::Editor* ob)
{
    m_way->Active = false;
    if (ob)
    {
        void (gui::actions::WayActions::*f)(int) = &gui::actions::WayActions::removeWayFromWayList;
        ob->emitClosure( bind(ob->actions()->wayActions(), f, m_position) );
        if (ob->shared()->selectedWay() == m_way)
        {
            ob->machine()->enterState("ways/idle");
            ob->shared()->setSelectedWay(nullptr);
        }

        // Reset dependent animations
        for(size_t i = 0; i < m_dependent_animations.size(); i++)
        {
            m_dependent_animations[i]->setWayObjectId(0);
            if (m_dependent_animations[i] == ob->shared()->selectedAnimation())
            {
                // Toggle as not set
                ob->emitClosure( blocked_bind(
                    ob->uiBlocks()->uiAnimationBlock()->cmbWayAnimationWay, 
                    &QComboBox::setCurrentIndex,
                    0
                ));
            }
        }

        // Remove from animation combo
        if (m_position_in_animation_combo >= 0)
        {
            ob->emitClosure( bind(
                ob->uiBlocks()->uiAnimationBlock()->cmbWayAnimationWay, 
                &QComboBox::removeItem,
                m_position_in_animation_combo
            ));
        }

        // Reset way animations major ids
        for(size_t i = 0; i < m_dependent_instances.size(); i++)
        {
            m_dependent_instances[i]->setWayMajorId(0);
            if (ob->shared()->selectedInstance() == m_dependent_instances[i])
            {
                ob->emitClosure( bind(
                    ob->uiBlocks()->uiAnimationInstanceBlock()->cmbWayAnimationInstanceWay, 
                    &QComboBox::setCurrentIndex, 
                    0
                ));
            }
        }

        // Remove from animation instances combo
        if (m_position_in_animation_instances_combo >= 0)
        {
            ob->emitClosure( bind(
                ob->uiBlocks()->uiAnimationInstanceBlock()->cmbWayAnimationInstanceWay, 
                &QComboBox::removeItem, 
                m_position_in_animation_instances_combo
            ));
        }
    }
}

void history::ways::Remove::rollback(core::Editor* ob)
{
    m_way->Active = true;
    if (ob)
    {
        gui::actions::WayActions* w_actions = ob->actions()->wayActions();
        ob->emitClosure( bind(w_actions, &gui::actions::WayActions::insertWayToWayList, m_way, m_position) );

        QString name = w_actions->viewableObjectName(m_way);
        QVariant w;
        w.setValue(m_way);

        void (QComboBox::*f)(int, const QString&, const QVariant&) = &QComboBox::insertItem;

        QComboBox* waw = ob->uiBlocks()->uiAnimationBlock()->cmbWayAnimationWay;
        if (m_position_in_animation_combo >= 0)
        {
            ob->emitClosure( bind(
                    waw, 
                    f,
                    m_position_in_animation_combo,
                    name,
                    w
            ));
        }

        for(size_t i = 0; i < m_dependent_animations.size(); i++)
        {
            m_dependent_animations[i]->setWayObjectId(m_way->MajorId);
            if (m_dependent_animations[i] == ob->shared()->selectedAnimation())
            {
                // Toggle as not set
                ob->emitClosure( blocked_bind(
                    waw, 
                    &QComboBox::setCurrentIndex,
                    m_position_in_animation_combo
                ));
            }
        }

        
        QComboBox* waiw = ob->uiBlocks()->uiAnimationInstanceBlock()->cmbWayAnimationInstanceWay;
        if (m_position_in_animation_instances_combo >= 0)
        {
            ob->emitClosure( bind(
                    waiw, 
                    f,
                    m_position_in_animation_instances_combo,
                    name,
                    w
            ));
        }

        for(size_t i = 0; i < m_dependent_instances.size(); i++)
        {
            m_dependent_instances[i]->setWayMajorId(m_way->MajorId);
            if (ob->shared()->selectedInstance() == m_dependent_instances[i] && m_position_in_animation_instances_combo >= 0)
            {
                ob->emitClosure( bind(
                    waiw, 
                    &QComboBox::setCurrentIndex, 
                    m_position_in_animation_instances_combo
                ));
            }
        }
    }
}

