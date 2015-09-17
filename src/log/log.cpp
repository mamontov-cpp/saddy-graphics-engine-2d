#include "../../include/3rdparty/format/format.h"
#include "../../include/renderer.h"

sad::log::Log::~Log()
{
    for(unsigned int i = 0; i < m_targets.count(); i++)
    {
        delete m_targets[i];
    }
}

void sad::log::Log::broadcast(const sad::log::Message & m)
{
    m_lock.lock();
    for(unsigned int i = 0; i < m_targets.count(); i++)
    {
        m_targets[i]->receive(m);
    }
    m_lock.unlock();
}

sad::String sad::log::Log::subsystem() 
{
    if (m_subsystems.count() != 0)
    {
        return m_subsystems[m_subsystems.count() - 1];
    }
    return "";
}


void sad::log::Log::createAndBroadcast(
    const sad::String & mesg, 
    sad::log::Priority priority,
    const char * file , 
    int line ,
    const sad::String & upriority
)
{
    sad::log::Message * m = new sad::log::Message(
        mesg,
        priority,
        file,
        line ,
        this->subsystem(),
        upriority
    );
    broadcast(*m);
    delete m;
}

sad::log::Log & sad::log::Log::addTarget(sad::log::Target * t)
{
    m_lock.lock();
    m_targets << t;
    m_lock.unlock();
    return *this;
}

sad::log::Log & sad::log::Log::removeTarget(sad::log::Target * t)
{
    m_lock.lock();
    m_targets.removeAll(t);
    m_lock.unlock();
    return *this;
}

sad::log::Log * sad::log::Log::ref()
{
    return sad::Renderer::ref()->log();
}


void sad::log::Log::pushSubsystem(const sad::String & str)
{
    sad::String data="Entering ";
    data += str;
    if (m_internal_mode)
    {
        saddyInternal(data);
    }
    else
    {
        debug(data);
    }
    m_subsystems << str;
}

void sad::log::Log::pushSubsystem(const sad::String & str, const char * file, int line)
{
    sad::String data="Entering ";
    data += str;
    if (m_internal_mode)
    {
        saddyInternal(data, file, line);
    }
    else
    {
        debug(data, file, line);
    }
    m_subsystems << str;
}

void sad::log::Log::popSubsystem()
{
    if (m_subsystems.size()!=0) { 
        sad::String data = "Leaving ";
        unsigned int last_item_index = m_subsystems.count()-1;
        data += m_subsystems[last_item_index];
        if (m_internal_mode)
        {
            saddyInternal(data);
        }
        else
        {
            debug(data);		
        }
        m_subsystems.removeAt(last_item_index);
    }
}
