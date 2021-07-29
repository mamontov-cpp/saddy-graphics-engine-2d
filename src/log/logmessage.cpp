#include <log/logmessage.h>

static const char * logmesgs[7] = {
    "SADDY",
    "USER",
    "DEBUG",
    "MESSAGE",
    "WARNING",
    "CRITICAL",
    "FATAL",
};

const char * sad::log::Message::spriority() const
{
    if (m_priority != sad::log::USER || m_user_priority.length() == 0)
    {
        return logmesgs[(int)m_priority];
    }
    return m_user_priority.data();
}


const char * sad::log::Message::stime() const
{
    tm * time = localtime(&m_time);
    sad::log::Message * m = const_cast<sad::log::Message *>(this);
    strftime((char *)m->m_buffer, 30, "%Y-%m-%d %H:%M:%S", time);
    return m_buffer;
}


sad::String sad::log::Message::fileline() const
{
    if (!m_file)
        return sad::String();
    sad::String o;
    const char * chslash = strrchr(m_file, '/');
    const char * chbslash = strrchr(m_file, '\\');
    const char * chk = nullptr;
    if (chslash)
    {
        if (chbslash)
        {
            chk = (chbslash > chslash) ? chbslash : chslash;
        }
        else chk = chslash;
    } else chk = chbslash;

    o =  (chk ? chk + 1 : m_file);
    o << ",";
    o << sad::String::number(m_line);
    return o;
}
