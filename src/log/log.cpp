#include "../../include/log/log.h"

static const char * logmesgs[6] = {
	"FATAL",
	"CRITICAL",
	"WARNING",
	"MESSAGE",
	"DEBUG",
	"USER"
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
	return asctime(localtime(&m_time));
}

sad::log::Message::Message(const hst::string & message,
					   	   sad::log::Priority priority,
						   const char * file,
						   int line ,
						   const hst::string & subsystem ,
						   const hst::string & user_priority 
					      )
{
	m_message = message;
	m_priority = priority;
	m_file = file;
	m_line = line;
	m_subsystem = subsystem;
	m_user_priority = user_priority;
	m_time = ::time(NULL);
}


hst::string sad::log::Message::fileline() const
{
	if (!m_file)
		return hst::string();
	hst::string o;
	char ch = '/';
#ifndef _WIN32
	ch = '//';
#endif	
	o =  (strrchr(m_file, ch) ? strrchr(m_file, ch) + 1 : m_file);
	o << ", ";
	o << hst::string::number(m_line);
	return o;
}

hst::string sad::log::StringCaster<hst::string>::cast(const hst::string & string)
{
	return string;
}


hst::string sad::log::StringCaster<std::string>::cast(const std::string & string)
{
	return string.c_str();
}


sad::log::Target::~Target()
{

}

sad::Log::~Log()
{
	for(int i = 0; i < m_targets.count(); i++)
	{
		delete m_targets[i];
	}
}

void sad::Log::broadcast(const sad::log::Message & m)
{
	for(int i = 0; i < m_targets.count(); i++)
	{
		m_targets[i]->receive(m);
	}
}

hst::string sad::Log::subsystem() 
{
	if (m_actions_stack.count() != 0)
	{
		return m_actions_stack[m_actions_stack.count() - 1];
	}
	return "";
}


void sad::Log::createAndBroadcast(const hst::string & mesg, 
				            	  sad::log::Priority priority,
						          const char * file , 
								  int line ,
								  const hst::string & upriority)
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

sad::Log & sad::Log::addTarget(sad::log::Target * t)
{
	m_targets << t;
	return *this;
}

sad::Log & sad::Log::removeTarget(sad::log::Target * t)
{
	m_targets.remove(t);
	return *this;
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
	std::string result = message.fileline().data();
	result += " ";
	return result;
}

sad::log::FileTarget::~FileTarget()
{
	close();
}


void sad::log::FileTarget::close()
{
	if (m_file) 
		fclose(m_file);
	m_file = NULL;
}


sad::log::FileTarget::FileTarget(const hst::string & format, int maxpriority)
{
	m_format = format;
	m_max_priority = maxpriority;
	m_file = NULL;
}


bool sad::log::FileTarget::open(const hst::string & filename)
{
	close();
	m_file = fopen(filename.data(), "wt");
	return m_file != NULL;
}

void sad::log::FileTarget::receive(const sad::log::Message & message)
{
	if (((int)message.priority()) >= m_max_priority || m_file == NULL)
		return;
	std::string mesg = str(fmt::Format(m_format.data()) 
						   << message.stime() 
						   << message.spriority()
						   << formatSubsystem(message).data()
						   << formatFileLine(message).data()
						   << message.message().data()
						  );
	fputs(mesg.c_str(), m_file);
}


sad::log::Console::~Console()
{

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
	std::string result = message.fileline().data();
	result += " ";
	return result;
}

sad::log::ConsoleTarget::~ConsoleTarget()
{
	m_console->clearColorMode();
	delete m_console;
}


sad::log::ConsoleTarget::ConsoleTarget(const hst::string & format, int maxpriority,  bool colored , bool allocate_console )
{
	m_format = format;
	m_max_priority = maxpriority;
	m_console = new sad::log::Console();
	if (colored)	this->createColoredOutput(); 
	else                  this->createNormalOutput();
	if (allocate_console) 
		m_console->createConsole();
}


void sad::log::ConsoleTarget::createColoredOutput()
{
	m_coloring.insert(sad::log::FATAL, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_RED));
	m_coloring.insert(sad::log::CRITICAL, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_MAGENTA));
	m_coloring.insert(sad::log::WARNING, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_YELLOW));
	m_coloring.insert(sad::log::MESSAGE, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::WHITE));
	m_coloring.insert(sad::log::DEBUG, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_BLUE));
	m_coloring.insert(sad::log::USER, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_CYAN));
}


void sad::log::ConsoleTarget::createNormalOutput()
{
	m_coloring.insert(sad::log::FATAL, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::CRITICAL, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::WARNING, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::MESSAGE, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::DEBUG, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::USER, hst::pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
}

void sad::log::ConsoleTarget::receive(const sad::log::Message & message)
{
	if (((int)message.priority()) >= m_max_priority)
		return;
	std::string mesg = str(fmt::Format(m_format.data()) 
						   << message.stime() 
						   << message.spriority()
						   << formatSubsystem(message).data()
						   << formatFileLine(message).data()
						   << message.message().data()
						  );
	m_console->setColorMode(m_coloring[message.priority()].p2(),  m_coloring[message.priority()].p1());
	m_console->print(mesg.c_str());
}



void sad::log::Console::print(const char * text)
{
	puts(text);
}


void sad::log::Console::createConsole()
{
#ifdef  WIN32
#endif	
}

sad::log::Console::~Console()
{
	this->clearColorMode();
}

sad::log::Console::Console()
{
#ifdef WIN32
#endif	
}


void sad::log::Console::clearColorMode()
{
#ifdef WIN32
#else
	puts("\033[00m");
#endif	
}


#ifdef WIN32

#else

static const char * fg[] = { "01;", "01;31;", "01;32;", "01;34;", "31;", "32;", "34;", "01;37;", "37;", "02;37;", "30;", "01;33;", "33;", "01;35;", "35;", "01;36;", "36;"};
static const char * bg[] = { ""    , "41"      , "42"      , "44"      , "41" , "42" , "44" , "47"      , "47" , "47"      ,  "40", "43"      , "43" , "45" ,      "45",  "46"      , "46;"};		
#endif


void sad::log::Console::setColorMode(sad::log::Color foreground, sad::log::Color background)
{
#ifdef WIN32
#else
	this->clearColorMode();
	printf("\033[%s%s%s", fg[(int)foreground], bg[(int)background], "m");
#endif
}