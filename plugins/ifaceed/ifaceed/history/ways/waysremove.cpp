#include "waysremove.h"

#include <QComboBox>

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

history::ways::Remove::Remove(sad::p2d::app::Way* w, int pos) 
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
        void (MainPanel::*f)(int) = &MainPanel::removeWayFromWayList;
        ob->emitClosure( bind(ob->panel(), f, m_position) );
        if (ob->shared()->selectedWay() == m_way)
        {
            ob->machine()->enterState("ways/idle");
            ob->shared()->setSelectedWay(NULL);
        }

        // Reset dependent animations
        for(size_t i = 0; i < m_dependent_animations.size(); i++)
        {
            m_dependent_animations[i]->setWayObjectId(0);
            if (m_dependent_animations[i] == ob->shared()->selectedAnimation())
            {
                // Toggle as not set
                ob->emitClosure( blocked_bind(
                    ob->panel()->UI()->cmbWayAnimationWay, 
                    &QComboBox::setCurrentIndex,
                    0
                ));
            }
        }

        // Remove from animation combo
        if (m_position_in_animation_combo >= 0)
        {
            ob->emitClosure( bind(
                ob->panel()->UI()->cmbWayAnimationWay, 
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
                    ob->panel()->UI()->cmbWayAnimationInstanceWay, 
                    &QComboBox::setCurrentIndex, 
                    0
                ));
            }
        }

        // Remove from animation instances combo
        if (m_position_in_animation_instances_combo >= 0)
        {
            ob->emitClosure( bind(
                ob->panel()->UI()->cmbWayAnimationInstanceWay, 
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
        ob->emitClosure( bind(ob->panel(), &MainPanel::insertWayToWayList, m_way, m_position) );

        QString name = ob->panel()->viewableObjectName(m_way);
        QVariant w;
        w.setValue(m_way);

        void (QComboBox::*f)(int, const QString&, const QVariant&) = &QComboBox::insertItem;

        if (m_position_in_animation_combo >= 0)
        {
            ob->emitClosure( bind(
                    ob->panel()->UI()->cmbWayAnimationWay, 
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
                    ob->panel()->UI()->cmbWayAnimationWay, 
                    &QComboBox::setCurrentIndex,
                    m_position_in_animation_combo
                ));
            }
        }


        if (m_position_in_animation_instances_combo >= 0)
        {
            ob->emitClosure( bind(
                    ob->panel()->UI()->cmbWayAnimationInstanceWay, 
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
                    ob->panel()->UI()->cmbWayAnimationInstanceWay, 
                    &QComboBox::setCurrentIndex, 
                    m_position_in_animation_instances_combo
                ));
            }
        }
    }
}

