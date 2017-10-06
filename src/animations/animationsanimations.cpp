#include "animations/animationsanimations.h"

// ========================= PUBLIC METHODS =========================

sad::animations::Animations::Animations()
{
    
}

sad::animations::Animations::~Animations()
{
    this->clearNow();
}

sad::animations::SavedObjectStateCache& sad::animations::Animations::cache()
{
    return m_cache;
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToObject(sad::db::Object* o)
{
    m_lock.lock();
    sad::Vector<sad::animations::Process*> result;
    for(size_t i = 0; i < m_command_queue.size(); i++)
    {
        if (m_command_queue[i].Type == CT_ADD)
        {
            if (m_command_queue[i].Added->isRelatedToObject(o))
            {
                result << m_command_queue[i].Added;
            }
        }
    }
    for(size_t i = 0; i < m_list.size(); i++)
    {
        if (m_list[i]->isRelatedToObject(o))
        {
            result << m_list[i];
        }
    }
    m_lock.unlock();
    return result;
}


void sad::animations::Animations::stopProcessesRelatedToObject(sad::db::Object* o)
{
    m_lock.lock();
    sad::Vector<sad::animations::Process*> result;
    for(size_t i = 0; i < m_command_queue.size(); i++)
    {
        if (m_command_queue[i].Type == CT_ADD)
        {
            if (m_command_queue[i].Added->isRelatedToObject(o))
            {
                m_command_queue[i].Added->stopInstancesRelatedToObject(o, this);
            }
        }
    }
    for(size_t i = 0; i < m_list.size(); i++)
    {
        if (m_list[i]->isRelatedToObject(o))
        {
            m_list[i]->stopInstancesRelatedToObject(o, this);
        }
    }
    m_lock.unlock();
}

// ========================= PROTECTED METHODS =========================

void sad::animations::Animations::_process()
{
    performQueuedActions();
    lockChanges();
    m_lock.lock();
    for(size_t i = 0; i < m_list.size(); i++)
    {
        sad::animations::Process * p = m_list[i];
        p->process(this);
        if (p->finished())
        {
            p->removedFromPipeline();
            m_list.removeAt(i);
            --i;
        }
    }
    m_lock.unlock();
    unlockChanges();
    performQueuedActions();
}

void sad::animations::Animations::addNow(sad::animations::Process* o)
{
    m_lock.lock();
    if (o)
    {
        o->addedToPipeline();
        m_list << o;
    }
    m_lock.unlock();
}

void sad::animations::Animations::removeNow(sad::animations::Process* o)
{
    m_lock.lock();
    if (o)
    {
        sad::Vector<sad::animations::Process*>::iterator it = std::find(
            m_list.begin(), 
            m_list.end(),
            o
        );
        if (it != m_list.end())
        {
            o->cancel(this);
            o->removedFromPipeline();
            m_list.erase(it);
        }
    }
    m_lock.unlock();
}

void sad::animations::Animations::clearNow()
{
    m_lock.lock();
    for(size_t i = 0; i < m_list.size(); i++)
    {
        m_list[i]->cancel(this);		    
        m_list[i]->removedFromPipeline();
    }
    m_list.clear();
    m_lock.unlock();
}
