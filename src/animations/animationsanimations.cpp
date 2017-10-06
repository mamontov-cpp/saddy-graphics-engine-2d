#include "animations/animationsanimations.h"

// ========================= PUBLIC METHODS =========================

sad::animations::Animations::Animations()
{
    
}

sad::animations::Animations::~Animations()
{
    this->clearNow();
}

void sad::animations::Animations::add(sad::animations::Process * o)
{
    if (!o)
    {
        return;
    }
    if (containerLocked())
    {
        m_command_queue_lock.lock();
        QueueCommand  c;
        c.Type = QueueCommand::CT_ADD;
        c.Process = o;
        o->addRef();
        pushCommand(c);
        m_command_queue_lock.unlock();
    }
    else
    {
        m_lock.lock();
        addNow(o);
        m_lock.unlock();
    }
}

void sad::animations::Animations::remove(sad::animations::Process *o)
{
    if (!o)
    {
        return;
    }
    if (containerLocked())
    {
        m_command_queue_lock.lock();
        QueueCommand  c;
        c.Type = QueueCommand::CT_REMOVE;
        c.Process = o;
        o->addRef();
        pushCommand(c);
        m_command_queue_lock.unlock();
    }
    else
    {
        m_lock.lock();
        removeNow(o);
        m_lock.unlock();
    }
}

void sad::animations::Animations::clear()
{
    if (containerLocked())
    {
        m_command_queue_lock.lock();
        QueueCommand  c;
        c.Type = QueueCommand::CT_CLEAR;
        pushCommand(c);
        m_command_queue_lock.unlock();
    }
    else
    {
        m_lock.lock();
        clearNow();
        m_lock.unlock();
    }
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
        if (m_command_queue[i].Type == QueueCommand::CT_ADD)
        {
            if (m_command_queue[i].Process->isRelatedToObject(o))
            {
                result << m_command_queue[i].Process;
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
        if (m_command_queue[i].Type == QueueCommand::CT_ADD)
        {
            if (m_command_queue[i].Process->isRelatedToObject(o))
            {
                m_command_queue[i].Process->stopInstancesRelatedToObject(o, this);
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
            p->delRef();
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
    // If process is already in animation, don't do stuff
    sad::Vector<sad::animations::Process*>::iterator it = std::find(
        m_list.begin(),
        m_list.end(),
        o
    );
    if (it != m_list.end())
    {
        return;
    }

    if (o)
    {
        o->addRef();
        o->addedToPipeline();
        m_list.push_back(o);
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
            o->delRef();
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
        m_list[i]->delRef();
    }
    m_list.clear();
    m_lock.unlock();
}

void sad::animations::Animations::pushCommand(const sad::animations::Animations::QueueCommand& c)
{
    m_command_queue_lock.lock();
    m_command_queue.push_back(c);
    m_command_queue_lock.unlock();
}

void sad::animations::Animations::lockChanges()
{
    m_lock_changes_lock.lock();
    m_lock_changes = true;
    m_lock_changes_lock.unlock();
}

void sad::animations::Animations::unlockChanges()
{
    m_lock_changes_lock.lock();
    m_lock_changes = false;
    m_lock_changes_lock.unlock();
}

bool sad::animations::Animations::containerLocked()
{
    bool result = false;
    m_lock_changes_lock.lock();
    result = m_lock_changes;
    m_lock_changes_lock.unlock();
    return result;
}


void sad::animations::Animations::performQueuedActions()
{
   m_command_queue_lock.lock();
   for(size_t i = 0; i < m_command_queue.count(); i++)
   {
       QueueCommand & c = m_command_queue[i];
       switch(c.Type)
       {
           case QueueCommand::CT_ADD :    { addNow(c.Process); c.Process->delRef(); break; }
           case QueueCommand::CT_REMOVE:  { removeNow(c.Process); c.Process->delRef(); break; }
           case QueueCommand::CT_CLEAR:   { clearNow(); break; }
       };
   }
   m_command_queue.clear();
   m_command_queue_lock.unlock();
}


