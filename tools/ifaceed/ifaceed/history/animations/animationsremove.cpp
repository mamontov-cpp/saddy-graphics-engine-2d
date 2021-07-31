#include "animationsremove.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include <QListWidgetItem>

Q_DECLARE_METATYPE(sad::animations::Animation*) //-V566

history::animations::Remove::Remove(sad::animations::Animation* a)
: m_animation(a), m_position_in_animation_list(-1), m_position_in_animation_instance_list(-1)
{
    m_animation->addRef();
}

history::animations::Remove::~Remove()
{
    m_animation->delRef();
}

void history::animations::Remove::set(
    int position_in_animation_list,
    int position_in_animation_instance_list,
    const sad::Vector< sad::Pair<sad::animations::Composite*, sad::Vector<int> > >& list
)
{
    m_position_in_animation_list = position_in_animation_list;
    m_position_in_animation_instance_list = position_in_animation_instance_list;
    m_composites = list;
}

void history::animations::Remove::set(
    const sad::Vector< sad::animations::Instance* >& list
)
{
    m_dependent_instances = list;
}

void history::animations::Remove::commit(core::Editor * ob)
{
    m_animation->Active = false;
    for(size_t i = 0; i < m_composites.size(); i++)
    {
        const sad::Vector<int>& positions_vector = m_composites[i]._2();
        for(int j = positions_vector.size() - 1; j > -1; j--) 
        {
            m_composites[i]._1()->remove(positions_vector[j]);
        }
    }
    for(size_t i = 0; i < m_dependent_instances.size(); i++)
    {
        m_dependent_instances[i]->setAnimationMajorId(0);		
    }
    if (ob)
    {
        ob->emitClosure( bind(ob->actions()->animationActions(), &gui::actions::AnimationActions::removeAnimationFromViewingLists, m_animation) );
        ob->emitClosure( bind(ob->actions()->animationActions(), &gui::actions::AnimationActions::updateCompositeList));
        for(size_t i = 0; i < m_dependent_instances.size(); i++)
        {
            if (m_dependent_instances[i] == ob->shared()->selectedInstance())
            {
                ob->emitClosure( bind(ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceAnimationFromDatabase, &QComboBox::setCurrentIndex, 0));
            }
        }
    }
}

void history::animations::Remove::rollback(core::Editor * ob)
{
    m_animation->Active = true;
    for(size_t i = 0; i < m_composites.size(); i++)
    {
        const sad::Vector<int>& positions_vector = m_composites[i]._2();
        for(size_t j = 0; j < positions_vector.size(); j++) 
        {
            m_composites[i]._1()->insert(m_animation->MajorId, positions_vector[j]);
        }		
    }
    for(size_t i = 0; i < m_dependent_instances.size(); i++)
    {
        m_dependent_instances[i]->setAnimationMajorId(m_animation->MajorId);		
    }
    if (ob)
    {
        if (ob->panel())
        {
            ob->emitClosure( bind(this, &history::animations::Remove::insertAnimationIntoUI, ob) );
            ob->emitClosure( bind(ob->actions()->animationActions(), &gui::actions::AnimationActions::updateCompositeList));
            for(size_t i = 0; i < m_dependent_instances.size(); i++)
            {
                if (m_dependent_instances[i] == ob->shared()->selectedInstance())
                {
                    ob->emitClosure( bind(ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceAnimationFromDatabase, &QComboBox::setCurrentIndex, m_position_in_animation_instance_list));
                }
            }
        }
    }
}

void history::animations::Remove::insertAnimationIntoUI(core::Editor* editor)
{
    QString name =	editor->actions()->animationActions()->nameForAnimation(m_animation);
    
    QVariant v;
    v.setValue(m_animation);
    
    if (m_position_in_animation_list > -1)
    {
        QListWidgetItem* item  = new QListWidgetItem(name);
        item->setData(Qt::UserRole, v);
        editor->uiBlocks()->uiAnimationBlock()->lstAnimations->insertItem(m_position_in_animation_list, item);
    }

    if (m_position_in_animation_instance_list > -1)
    {
        editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceAnimationFromDatabase->insertItem(m_position_in_animation_instance_list, name, v);
    }
}
