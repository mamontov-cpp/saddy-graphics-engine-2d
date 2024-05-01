#include "../../include/3rdparty/format/format.h"
#include "../../include/renderer.h"

#include "../../include/db/dbtypename.h"
#include "opticksupport.h"

sad::log::Log::~Log()
{
    PROFILER_EVENT;
    for(unsigned int i = 0; i < m_targets.count(); i++)
    {
        m_targets[i]->delRef();
    }
}

void sad::log::Log::broadcast(const sad::log::Message & m)
{
    PROFILER_EVENT;
    m_lock.lock();
    for(unsigned int i = 0; i < m_targets.count(); i++)
    {
        m_targets[i]->receive(m);
    }
    m_lock.unlock();
}

sad::String sad::log::Log::subsystem() 
{
    PROFILER_EVENT;
    if (m_subsystems.count() != 0)
    {
        return m_subsystems[m_subsystems.count() - 1];
    }
    return "";
}


void sad::log::Log::createAndBroadcast(
    const sad::String & message, 
    sad::log::Priority priority,
    const char * file , 
    int line ,
    const sad::String & user_priority
)
{
    PROFILER_EVENT;
    sad::log::Message * m = new sad::log::Message(
        message,
        priority,
        file,
        line ,
        this->subsystem(),
        user_priority
    );
    broadcast(*m);
    delete m;
}

sad::log::Log & sad::log::Log::addTarget(sad::log::Target * t)
{
    PROFILER_EVENT;
    m_lock.lock();
    t->addRef();
    m_targets << t;
    m_lock.unlock();
    return *this;
}

sad::log::Log & sad::log::Log::removeTarget(sad::log::Target * t)
{
    PROFILER_EVENT;
    m_lock.lock();
    m_targets.removeAll(t);
    t->delRef();
    m_lock.unlock();
    return *this;
}

sad::log::Log * sad::log::Log::ref()
{
    PROFILER_EVENT;
    return sad::Renderer::ref()->log();
}


void sad::log::Log::pushSubsystem(const sad::String & str)
{
    PROFILER_EVENT;
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
    PROFILER_EVENT;
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
    PROFILER_EVENT;
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

DECLARE_COMMON_TYPE(sad::log::Log);
