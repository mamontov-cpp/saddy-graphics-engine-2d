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


hst::string sad::log::Message::fileline()
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


hst::string LogStringCaster<hst::string>::cast(const hst::string> & string)
{
	return string;
}

std::string LogStringCaster<std::string>::cast(const std::string> & string)
{
	return string.c_str();
}


