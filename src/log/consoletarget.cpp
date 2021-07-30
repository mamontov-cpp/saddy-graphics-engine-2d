#include <log/consoletarget.h>
#include <os/consoleimpl.h>
#include <3rdparty/format/format.h>
#include <db/dbtypename.h>

sad::log::ConsoleTarget::ConsoleTarget(const sad::String & format, int min_priority,  bool colored , bool allocate_console )
: m_console(new sad::os::ConsoleImpl())
{
    m_format = format;
    m_min_priority = min_priority;
    if (colored)    this->createColoredOutput(); 
    else            this->createNormalOutput();
    if (allocate_console) 
        m_console->createConsole();
}

void sad::log::ConsoleTarget::receive(const sad::log::Message & message)
{
    if (((int)message.priority()) < m_min_priority)
        return;
    std::string mesg = str(fmt::Format(m_format) 
                           << message.timeAsString() 
                           << message.priorityAsString()
                           << formatSubsystem(message)
                           << formatFileLine(message)
                           << message.message()
                          );
    m_console->setColorMode(m_coloring[message.priority()].p2(),  m_coloring[message.priority()].p1());
    m_console->print(mesg.c_str());
}

void sad::log::ConsoleTarget::setColorForPriorityAndColoredOutput(sad::log::Priority p, sad::log::Color c)
{
    m_coloring[p].set2(c);
}

sad::log::ConsoleTarget::~ConsoleTarget()
{
    m_console->clearColorMode();
    delete m_console;
}


std::string sad::log::ConsoleTarget::formatSubsystem(const sad::log::Message & message)
{
    if (message.subsystem().length() == 0)
        return "";
    std::string result = message.subsystem().data();
    result += ": ";
    return result;
}

std::string sad::log::ConsoleTarget::formatFileLine(const sad::log::Message & message)
{
    if (message.fileline().length() == 0)
        return "";
    std::string result = "[";
    result += message.fileline().data();
    result += "] ";
    return result;
}

void sad::log::ConsoleTarget::createColoredOutput()
{
    m_coloring.insert(sad::log::Priority::FATAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::LIGHT_RED));
    m_coloring.insert(sad::log::Priority::CRITICAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::LIGHT_MAGENTA));
    m_coloring.insert(sad::log::Priority::WARNING, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::LIGHT_YELLOW));
    m_coloring.insert(sad::log::Priority::MESSAGE, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::WHITE));
    m_coloring.insert(sad::log::Priority::DEBUG, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::LIGHT_BLUE));
    m_coloring.insert(sad::log::Priority::USER, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::LIGHT_CYAN));
    m_coloring.insert(sad::log::Priority::SADDY_INTERNAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::DARK_MAGENTA));
}


void sad::log::ConsoleTarget::createNormalOutput()
{
    m_coloring.insert(sad::log::Priority::FATAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::NONE));
    m_coloring.insert(sad::log::Priority::CRITICAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::NONE));
    m_coloring.insert(sad::log::Priority::WARNING, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::NONE));
    m_coloring.insert(sad::log::Priority::MESSAGE, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::NONE));
    m_coloring.insert(sad::log::Priority::DEBUG, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::NONE));
    m_coloring.insert(sad::log::Priority::USER, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::NONE));
    m_coloring.insert(sad::log::Priority::SADDY_INTERNAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::Color::NONE, sad::log::Color::NONE));
}

DECLARE_COMMON_TYPE(sad::log::ConsoleTarget)
