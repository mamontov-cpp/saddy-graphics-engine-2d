/*! \file log.h
	\author HiddenSeeker
	
	Describes a basic log and log parameters
 */
#ifdef QT_CORE_LIB
    #include<QString>
#endif
#include "../sadvector.h"
#include "../sadhash.h"
#include "../sadpair.h"
#include "../sadstring.h"
#include "../marshal/actioncontext.h"
#include "../os/mutex.h"
#include "../3rdparty/format/format.h"
#include <ctime>
#include <sstream>
#include <string>
#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif	
	#include <windows.h>
#endif
#pragma once



inline std::ostream & operator<<(std::ostream & o, const sad::String & o2)
{
	return o << std::string(o2.data());
}

namespace sad
{

namespace log
{

/*! Converts any object to string, using std::ostringstream, for a substitution
	it to log message. 

	Instantiate a template of this class, to make output of your class possible
 */
template<typename T>
class StringCaster
{
public:
	/*! Casts object to string, for passing it to a log message
		\param[in] object an objecct
		\return string representation of object
	 */
	static std::string cast(const T & object)
	{
		std::ostringstream s; 
		s << object;
		return s.str().c_str();
	}
};

/*! This is special type of caster, which allows writing statementss, 
	like SL_CRITICAL(fmt::Print("{0}") << 5)
 */
template<>
class StringCaster<fmt::internal::ArgInserter<char> >
{
public:
	/*! This caster performs conversion from format library internal structure
	    to string, allowing to pass it to a log message
		\param[in] string an inserter from inner formatlibrary
		\return string
	  */
	static std::string cast(const fmt::internal::ArgInserter<char> & string)
	{
		// We need to perform this, since operator FormatterProxy<Char>() is non-constant
		fmt::internal::ArgInserter<char> & fmt = const_cast<fmt::internal::ArgInserter<char>&>(string); 
		return str(fmt);
	}
};

#ifdef QT_CORE_LIB

/*! Converts QString to a string, allowing to pass it to a log message
 */
template<>
class StringCaster<QString>
{
public:
	/*! Converts QString to std::string, making possible to pass it to a log
		message
     */
	inline static std::string cast(const QString & string)
	{
		return string.toStdString();
	}
};

#endif

/*! Defines a priority of messages in log
	Messages can be disabled by it's priority.
	Also, when possible, they can be represented on screen by different colors
 */
enum Priority
{
	FATAL = 0,
	CRITICAL = 1,
	WARNING = 2,
	MESSAGE = 3,
	DEBUG = 4,
	USER = 5
};
/*! Defines a color, for color scheme, which text will be colored, when log message
	is represented on screen
 */
enum Color
{
	NONE =  0,
	LIGHT_RED = 1,
	LIGHT_GREEN = 2,
	LIGHT_BLUE = 3,
	DARK_RED = 4,
	DARK_GREEN = 5,
	DARK_BLUE = 6,
	WHITE = 7,
	LIGHT_GREY = 8,
	DARK_GREY = 9,
	BLACK = 10,
	LIGHT_YELLOW = 11,
	DARK_YELLOW = 12,
	LIGHT_MAGENTA = 13,
	DARK_MAGENTA = 14,
	LIGHT_CYAN = 15,
	DARK_CYAN = 16
};
/*! Describes a logging message, which is broadcasted by sad::log::Log to all targets
	and contains information of all data
 */
class Message
{
public:
	/*! Creates a new message
		\param[in] message message text
		\param[in] priority priority type
		\param[in] file file information
		\param[in] line line info
		\param[in] subsystem subsystem info
		\param[in]  user_priority user priority information
	 */
	inline Message(const sad::String & message,
				   sad::log::Priority priority,
				   const char * file = NULL,
				   int line = 0,
				   const sad::String & subsystem = sad::String(),
				   const sad::String & user_priority = sad::String()
				  )
	: m_message(message), m_priority(priority), m_file(file),
	  m_line(line), m_subsystem(subsystem), m_user_priority(user_priority)
	{
		m_time = ::time(NULL);
	}
	/*! Returns subsystem, passed into constructor.
		\return subsystem
	 */
	inline const sad::String & subsystem() const
	{ 
		return m_subsystem; 
	} 
	/*! Returns message string, passed into constructor.
		\return message string
	 */
	inline const sad::String & message() const
	{ 
		return m_message; 
	} 				
	/*! Returns string representation of message creation time
		\return string representation of message creation time
	 */
	const char * stime() const; 
	/*! Returns message creation time
		\return message creation time
	 */
	inline const time_t & time() const
	{ 
		return m_time; 
	}   
	/*! Returns basic priority of message
		\return basic priority of message
	 */
	inline sad::log::Priority priority() const 
	{ 
		return m_priority; 
	} 
	/*! Returns user priority of message
		\return usage priority message
	 */
	inline const sad::String & upriority() const
	{ 
		return m_user_priority; 
	} 
	/*! Returns string with name and line, formatted like 
		"name of file", "line"			
		\return formatted file name and line
	 */
	sad::String fileline() const;
	/*! Returns a string representation of priority.
		If user representation of priority is set, than it's returned, otherwise
		a built-in representation of priority
		\return string representation of priority
	 */
	const char * spriority() const;
	/*! Returns a source code file name, where log function is called
		\return source code file name
	 */
	inline const char * file()  const 
	{ 
		return m_file; 
	}
	/*! Returns a source code line, where log function is called
	 */
	inline int line()  const 
	{ 
		return m_line; 
	}
private:	
	/*! A buffer part for converting time to string, using strftime.
	 */
	char m_buffer[30];    
	sad::String m_subsystem;            //!< Subsystem
	sad::String m_message;              //!< Message data
	time_t      m_time;		            //!< Current time stamp
	sad::log::Priority m_priority;      //!< Information about message priority
	sad::String        m_user_priority; //!<  User priority, not used if non USER
	const char   *     m_file;          //!<  Compiled file, null if nothing
	int                m_line;          //!<  A line of code, null if nothing
};

/*! A console class, which implements crossplatform support for low-level console IO
 */
class Console
{
public:
	/*! Connects to console and initializates support for console
     */
	Console();
	/*! Used to allocate console on Windows
	 */
	virtual void createConsole() ;
	/*! Sets a color mode for console
	 */
	virtual void setColorMode(sad::log::Color foreground, sad::log::Color background) ; 
	/*! Used to restore default color mode in console
	 */
	virtual void clearColorMode();
	/*! Outputs a text, using color, set previously
	 */
	virtual void print(const char * text) ;
	/*! Disconnects from console and restores default color scheme
	 */
	virtual ~Console();
#ifdef WIN32
private:
	WORD m_oldattributes;
	HANDLE m_console;
	/*! Inits console information
	 */
	void initConsole();
#endif

};
/*! A log target class is the one, which works with
	output log messages, redirecting it to 
	file or console or other output device, depending on it's subclass

	To implement writing to your output device, you must subclass it.
			
	Note, that this is a basic class, so it is abstract
  */
class Target
{
public:
	/*! Receives a messages from targetting information
		\param[in] message taken message
	 */
	virtual void receive(const sad::log::Message & message) = 0;
	/*! Kept only for inheritance conformance. Does nothing
	 */
	virtual ~Target();
};
/*! A log, that writes formatted messages to a file. 
	You must call sad::log::FileTarget::open() before doing any output, because 
	OS could fail to opena file. Otherwise, or when OS fails to open FILE no output
	is performed
 */
class FileTarget: public sad::log::Target
{
public:
	/*! Creates a new file with specified format.
		Format defined as followes
		{0} - current time
		{1} - message priority
		{2} - formatSubsystem() result, by default, subsystem + ": ", nothing if subsystem is not specified. For example: "commit():", ""
		{3} - formatFileLine() result, file and line through ', ', nothing if not specified
		{4} - message text
		\param[in] format format string 
		\param[in] maxpriority Maximum priority for outputting. 
							   Messages with priority  value, 
							   bigger than maximum priority are discarded
	 */
	FileTarget(const sad::String & format = "{0}: [{1}] {3}{2}{4}", int maxpriority = 6);
	/*! Tries to open file for writing. Previous file, if opened with object
		of this target is closed and new file is opened, with destroying it's content
		\param[in] filename name of file
		\return true if opened successfully
	 */
	bool open(const sad::String & filename);
	/*! Formats message and writes it to file
		\param[in] message received message
	 */
	virtual void receive(const sad::log::Message & message);
	/*! Set maximum priority, which can be output
		\param[in] priority level message
	 */ 
	inline void setMaxPriority(int priority) { m_max_priority = priority; }
	/*! Closes a file if opened it before
	 */
	virtual ~FileTarget();
protected:
	FILE * m_file;         //!< Inner file handle
	/*! Maximum priority of output message. 
		Messages of bigger priority will not be printed
	 */
	int    m_max_priority; 
	sad::String m_format;  //!< Format for outputting the message
	/*! Formats a subsystem part, by default adds ": "
		\param[in] message a logged message
		\return format string
	 */
	virtual std::string formatSubsystem(const sad::log::Message & message);
	/*! Formats file and line part, by default adds a space, or empty string
		\param[in] message a logged message
		\return format string
	 */
	virtual std::string formatFileLine(const sad::log::Message & message);
	/*! Closes a to opened file, if can. Also resets it to null
	 */
	virtual void close();
};
/*! A special kind of target, which prints output to console.
	This kind of target, supports colored output and allocating console on Windows.
 */
class ConsoleTarget: public sad::log::Target
{
public:
	/*! Creates console target  with specified format.
		Format defined as followes
		{0} - current time
		{1} - message priority
		{2} - formatSubsystem() result, by default, subsystem + ": ", 
		      nothing if subsystem is not specified. For example: "commit():", ""
		{3} - formatFileLine() result, file and line 
		      through ', ', nothing if not specified
		{4} - message text
		\param[in] format format string 
		\param[in] maxpriority Maximum priority for outputting. 
							   Messages with priority  value, 
							   bigger than maximum priority are discarded
		\param[in] colored  whether output should be colored
		\param[in] allocate_console forces allocating new console, 
									if current platform is Windows. The console
									will be allocated immediately after creation
	 */
	ConsoleTarget(const sad::String & format = "{0}: [{1}] {3}{2}{4}", 
				  int maxpriority = 6,  
				  bool colored =  true, 
				  bool allocate_console = false);
	/*! Formats message and outputs it to console
		\param[in] message received message
	 */
	virtual void receive(const sad::log::Message & message);
	/*! Restores default color mode for console, saving problems
	  */
	~ConsoleTarget();
protected:
	/*! An inner implementation of console, used to support low-level console 
		operations, like setting color and printing already formatted output
	 */
	sad::log::Console * m_console; //!< A handle-like console
	/*! Maximum priority of output message. 
		Messages of bigger priority will not be printed
	 */
	int         m_max_priority;  
	sad::String m_format; //!< Format for converting message to string
	/*! Color schema, as map from priority to pair of colors, where first is
		background color and second is foreground color
	 */
	sad::Hash<sad::log::Priority, 
			  sad::Pair<sad::log::Color, sad::log::Color>
			 > m_coloring; 
	/*! Formats a subsystem message part. By default adds ": " to and of name of
		current subsystem, if it's specified
		\return format string
	 */
	virtual std::string formatSubsystem(const sad::log::Message & message);
	/*! Formats file and line message part. 
		If file and line are specified,
		encloses them in brackets and places between them
		a sad::log::Message::fileline() result. Adds a trailing space after result.
		If file and line are not specified, returns empty string 
		\return format string
	  */
	virtual std::string formatFileLine(const sad::log::Message & message);
	/*! Initializes default coloring schema
	 */
	void createColoredOutput();
	/*! Initializes schema. The console output, initalized with
		this method won't be colored at all.
     */
	void createNormalOutput();
};
	
/*! Log class takes all the front work, building a messages 
	and broadcasting them to all targets.

	If you want to register some events, writing it to log, you should use 
	this class.
 */
class Log: public ActionContext
{
public:
	/*! Broadcasts a message to all targets
		\param[in] m message
	 */
	virtual void broadcast(const sad::log::Message & m);
	/*! Adds a target to a list of targets. A messages, created with 
		sad::log::Log::broadcast() will be broadcasted to this target.
		\param[in] t target 
	 */
	virtual sad::log::Log & addTarget(sad::log::Target * t);
	/*! Removes a target to a list of targets. A messages, created with 
		sad::log::Log::broadcast() will be broadcasted to this target.
			
		Note, that memory from target is not freed. You must delete it manually.

		\param[in] t target 
	 */
	virtual sad::log::Log & removeTarget(sad::log::Target * t);
	/*! Broadcasts a message of priority FATAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	template<typename T> 
	void fatal(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::FATAL, file, line);
	}
	/*! Broadcasts a message of priority CRITICAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	template<typename T> 
	void critical(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::CRITICAL, file, line);
	}
	/*! Broadcasts a message of priority WARNING to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void warning(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::WARNING, file, line);
	}
	/*! Broadcasts a message of priority MESSAGE to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void message(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::MESSAGE, file, line);
	}
	/*! Broadcasts a message of priority DEBUG to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void debug(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::DEBUG, file, line);
	}
	/*! Broadcasts a message of user priority to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
		\param[in] user a user priority name
	 */		
	template<typename T> 
	void user(
		const T & mesg, 
		const char * file = NULL, 
		int line = 0, 
		const sad::String & user =  sad::String()
	)
	{
		_createAndBroadcast(mesg, sad::log::USER, file, line, user);
	}
	/*! Broadcasts a message of priority FATAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	void fatal(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::FATAL, file, line);
	}
	/*! Broadcasts a message of priority CRITICAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	void critical(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::CRITICAL, file, line);
	}
	/*! Broadcasts a message of priority WARNING to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	void warning(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::WARNING, file, line);
	}
	/*! Broadcasts a message of priority MESSAGE to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	void message(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::MESSAGE, file, line);
	}
	/*! Broadcasts a message of priority DEBUG to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	void debug(const char  * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::DEBUG, file, line);
	}
	/*! Broadcasts a message of user priority to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
		\param[in] user a user priority name
	 */		
	void user(const char *  mesg, const char * file = NULL, int line = 0, const sad::String & user =  sad::String())
	{
		_createAndBroadcast(sad::String(mesg), sad::log::USER, file, line, user);
	}
	/*! Pushes current subsystem name into stack of subsystem names
		\param[in] str string
	 */
	virtual void pushAction(const sad::String & str);
	/*! Pushes current subsystem name into stack of subsystem names
		\param[in] str string
		\param[in] file a name of source file, where entering to subsystem occured
		\param[in] line a source file line, where entering 
	 */
	virtual void pushAction(const sad::String & str, const char * file, int line);
	/*! Pops last subsystem name from the stack of subsystem names
	 */
	virtual void popAction();
	/*! Frees memory from all targets
	 */
	virtual ~Log();
	/*! Returns a global renderer's log instance
	 */
	static Log * ref();
protected:
	/*! Locks, for making this compatible with multithreading applications
	 */
	os::mutex m_lock;
	/*! A vector of targets, for broadcasting file
	 */
	sad::Vector<sad::log::Target *> m_targets;
	/*! Returns a current subsystem
		\return name of current subsystem
	 */
	virtual sad::String subsystem();
	/*! Creates new message and broadcasts them to all contained targets
		\param[in] mesg text message, that is being logged
		\param[in] priority  a priority for message
		\param[in] file      a name of file, where message was emitted
		\param[in] line		 a lnumber of line, where message was emitted
		\param[in] upriority 
	 */
	virtual void createAndBroadcast(
		const sad::String & mesg, 
		sad::log::Priority priority,
		const char * file = NULL, 
		int line = 0,
		const sad::String & upriority = sad::String()
	);
	/*! Creates new message, converts it to string 
		and broadcasts them to all contained targets
		\param[in] mesg       message, that is being logged
		\param[in] priority  a priority for message
		\param[in] file      a name of file, where message was emitted
		\param[in] line		 a lnumber of line, where message was emitted
		\param[in] upriority 
	 */
	template<typename T>
	void _createAndBroadcast(
		const T & mesg, 
		sad::log::Priority priority,
		const char * file = NULL, 
		int line = 0,
		const sad::String & upriority = sad::String()
	)
	{
		createAndBroadcast(sad::log::StringCaster<T>::cast(mesg), priority, file, line, upriority);
	}
};
/*! Scopes define a current scope for log
	It pushes subsystem name to it when are created, and  
	pops it, when destroyed.
 */
class Scope
{
public:
	/*! Pushes a new subsystem name into a log
		\param[in] c subsystem name
		\param[in] file source file name, where scope was created
		\param[in] line source code line, where scope was created
		\param[in] log a log, where changes will be registered
	 */
	Scope(
		const char * c, 
		const char * file = NULL, 
		int line = 0, 
		sad::log::Log * log = sad::log::Log::ref()
	);
	/*! Pushes a new subsystem name into a log
		\param[in] c subsystem name
		\param[in] file source file name, where scope was created
		\param[in] line source code line, where scope was created
		\param[in] log a log, where changes will be registered
	 */
	Scope(
		const sad::String & c, 
		const char * file = NULL, 
		int line = 0, 
		sad::log::Log * log = sad::log::Log::ref()
	);
	/*! Pushes a new subsystem name into a log
		\param[in] c subsystem name
		\param[in] file source file name, where scope was created
		\param[in] line source code line, where scope was created
		\param[in] log a log, where changes will be registered
	 */
	Scope(
		const std::string & c, 
		const char * file = NULL, 
		int line  = 0, 
		sad::log::Log * log = sad::log::Log::ref()
	);
	/*! Pushes a new subsystem name into a log
		\param[in] c subsystem name
		\param[in] file source file name, where scope was created
		\param[in] line source code line, where scope was created
		\param[in] log a log, where changes will be registered
	 */
	Scope(
		const fmt::internal::ArgInserter<char> & c, 
		const char * file = NULL, 
		int line  = 0, 
		sad::log::Log * log = sad::log::Log::ref()
	);
	/*! Pops  current subsystem, specified on creation of scope from subsystem names
		stack of linked logger
	 */
	~Scope();
private:
	/*! A linked log, where all state changes will be registered
	 */
	sad::log::Log * m_log;
};

}

}
 
/*! Creates new message in global log with message X and piority FATAL
 */
#define SL_FATAL(X) sad::log::Log::ref()->fatal(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and piority CRITICAL
 */
#define SL_CRITICAL(X) sad::log::Log::ref()->critical(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and piority WARNING
 */
#define SL_WARNING(X) sad::log::Log::ref()->warning(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and piority MESSAGE
 */
#define SL_MESSAGE(X) sad::log::Log::ref()->message(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and piority DEBUG
 */
#define SL_DEBUG(X) sad::log::Log::ref()->debug(X, __FILE__, __LINE__)
/*! Creates new user message in global log with message X and piority TYPE
 */
#define SL_USER(X, TYPE) sad::log::Log::ref()->user(X, __FILE__, __LINE__, TYPE)

/*! Creates new message in local log R with message X and piority FATAL
 */
#define SL_LOCAL_FATAL(X,R)    (R).log()->fatal(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and piority CRITICAL
 */
#define SL_LOCAL_CRITICAL(X,R) (R).log()->critical(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and piority WARNING
 */
#define SL_LOCAL_WARNING(X,R)  (R).log()->warning(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and piority MESSAGE
 */
#define SL_LOCAL_MESSAGE(X,R)  (R).log()->message(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and piority DEBUG
 */
#define SL_LOCAL_DEBUG(X,R)    (R).log()->debug(X, __FILE__, __LINE__)
/*! Creates new user message in local log R with message X and piority TYPE
 */
#define SL_LOCAL_USER(X, R, TYPE)  (R).log()->user(X, __FILE__, __LINE__, TYPE)

/*! Creates new scope with subsystem name X
 */
#define SL_SCOPE(X)  sad::log::Scope  _____1_____(X, __FILE__, __LINE__)
/*! Creates new scope in local log R with subsystem name X
 */
#define SL_LOCAL_SCOPE(X,R)  sad::log::Scope  _____1_____(X, __FILE__, __LINE__, (R).log())

