#include <log/filetarget.h>
#include <3rdparty/format/format.h>
#include <db/dbtypename.h>


sad::log::FileTarget::FileTarget(const sad::String & format, int min_priority)
: m_file(nullptr), m_min_priority(min_priority), m_format(format)
{
}


bool sad::log::FileTarget::open(const sad::String & filename)
{
    close();
    m_file = fopen(filename.data(), "wt");
    return m_file != nullptr;
}

void sad::log::FileTarget::receive(const sad::log::Message & message)
{
    if (((int)message.priority()) < m_min_priority || m_file == nullptr)
        return;
    std::string mesg = str(fmt::Format(m_format) 
                           << message.timeAsString() 
                           << message.priorityAsString()
                           << formatSubsystem(message)
                           << formatFileLine(message)
                           << message.message()
                          );
    fputs(mesg.c_str(), m_file);
    fputs("\n", m_file);
}

sad::log::FileTarget::~FileTarget()
{
    close();
}

std::string sad::log::FileTarget::formatSubsystem(const sad::log::Message & message)
{
    if (message.subsystem().length() == 0)
        return "";
    std::string result = message.subsystem().data();
    result += ": ";
    return result;
}

std::string sad::log::FileTarget::formatFileLine(const sad::log::Message & message)
{
    if (message.fileline().length() == 0)
        return "";
    std::string result = "[";
    result += message.fileline().data();
    result += "] ";
    return result;
}

void sad::log::FileTarget::close()
{
    if (m_file) 
        fclose(m_file);
    m_file = nullptr;
}

DECLARE_COMMON_TYPE(sad::log::FileTarget);