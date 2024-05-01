#include <log/logscope.h>
#include <log/log.h>
#include "opticksupport.h"


sad::log::Scope::Scope(
    const char * c, 
    const char * f, 
    int l, 
    sad::log::Log * log, 
    bool saddy_internal
)
: m_saddy_internal(saddy_internal)
{
    PROFILER_EVENT;
    if (log == nullptr)
    {
        m_log = sad::log::Log::ref();
    }
    else
    {
        m_log = log;
    }
    if (m_saddy_internal) m_log->setInternalMode(true);
    m_log->pushSubsystem(c, f, l);
    if (m_saddy_internal) m_log->setInternalMode(false);	
}

sad::log::Scope::Scope(
    const sad::String & c, 
    const char * f, 
    int l, 
    sad::log::Log * log,
    bool saddy_internal
)
: m_saddy_internal(saddy_internal)
{
    PROFILER_EVENT;
    if (log == nullptr)
    {
        m_log = sad::log::Log::ref();
    }
    else
    {
        m_log = log;
    }
    if (m_saddy_internal) m_log->setInternalMode(true);
    m_log->pushSubsystem(c, f, l);
    if (m_saddy_internal) m_log->setInternalMode(false);	
}

sad::log::Scope::Scope(
    const std::string & c, 
    const char * f, 
    int l, 
    sad::log::Log * log,
    bool saddy_internal
)
:m_saddy_internal(saddy_internal)
{
    PROFILER_EVENT;
    if (log == nullptr)
    {
        m_log = sad::log::Log::ref();
    }
    else
    {
        m_log = log;
    }
    if (m_saddy_internal) m_log->setInternalMode(true);
    m_log->pushSubsystem(c.c_str(), f, l);
    if (m_saddy_internal) m_log->setInternalMode(false);	
}

sad::log::Scope::Scope(
    const fmt::internal::ArgInserter<char> & c, 
    const char * f, 
    int l, 
    sad::log::Log * log,
    bool saddy_internal
)
:m_saddy_internal(saddy_internal)
{
    PROFILER_EVENT;
    if (log == nullptr)
    {
        m_log = sad::log::Log::ref();
    }
    else
    {
        m_log = log;
    }
    if (m_saddy_internal) m_log->setInternalMode(true);
    fmt::internal::ArgInserter<char> & fmt = const_cast<fmt::internal::ArgInserter<char>&>(c); 
    m_log->pushSubsystem(str(fmt).c_str() , f, l);
    if (m_saddy_internal) m_log->setInternalMode(false);	
}



sad::log::Scope::~Scope()
{
    PROFILER_EVENT;
    if (m_saddy_internal) m_log->setInternalMode(true);
    m_log->popSubsystem();
    if (m_saddy_internal) m_log->setInternalMode(false);
}
