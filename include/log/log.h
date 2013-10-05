/*! \file log.h
	\author HiddenSeeker
	
	Describes a basic log and log parameters
 */
#ifdef QT_CORE_LIB
    #include<QString>
#endif
#include "../templates/svector.h"
#include "../templates/hhash.hpp"
#include "../templates/hpair.hpp"
#include "../templates/hstring.h"
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



inline std::ostream & operator<<(std::ostream & o, const hst::string & o2)
{
	return o << std::string(o2.data());
}

namespace sad
{
	namespace log
	{
		template<typename T>
		class StringCaster
		{
	     public:
			/*! A caster for helping string find their ways
			 */
			static hst::string cast(const T & string)
			{
				std::ostringstream s; 
				s << string;
				return s.str().c_str();
			}
		};
		// Allows to write statements like SL_CRITICAL(fmt::Print("{0}") << 5)
		template<>
		class StringCaster<fmt::internal::ArgInserter<char> >
		{
		 public:
			 /*! This caster performs conversion from format library internal structure
			     to ours
				 \param[in] string an inserter from inner library to make logging simpler
			 */
			static hst::string cast(const fmt::internal::ArgInserter<char> & string)
			{
				// We need to perform this, since operator FormatterProxy<Char>() is non-constant
				fmt::internal::ArgInserter<char> & fmt = const_cast<fmt::internal::ArgInserter<char>&>(string); 
				return str(fmt).c_str();
			}
		};

#ifdef QT_CORE_LIB
		template<>
		class StringCaster<QString>
		{
	     public:
			/*! A caster for helping string find their ways
             */
			inline static hst::string cast(const QString & string)
			{
				return hst::string(string.toStdString().c_str());
			}
		};
#endif

		/*! Priotity of message of log
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
		/*! A colors table for coloring some text in console 
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
		/*! Describes a logging message 
		 */
		class Message
		{
			private:	
				char m_buffer[30];       //!< A buffer part
				hst::string m_subsystem; //!< Subsystem
				hst::string m_message;   //!< Message data
				time_t      m_time;		 //!< Current time
				sad::log::Priority m_priority; //!< Information about message priority
				hst::string        m_user_priority; //!<  User priority, not used if non USER
				const char   *     m_file;          //!<  Compiled file, null if nothing
				int                m_line;          //!<  A line of code, null if nothing
			public:
				/*! Creates a new message
					\param[in] message message text
					\param[in] priority priority type
					\param[in] file file information
					\param[in] line line info
					\param[in] subsystem subsystem info
					\param[in]  user_priority user priority information
				 */
				Message(const hst::string & message,
						sad::log::Priority priority,
						const char * file = NULL,
						int line = 0,
						const hst::string & subsystem = hst::string(),
						const hst::string & user_priority = hst::string()
					   );
					   
				inline const hst::string & subsystem() const
				{ return m_subsystem; } //!< Returns a subsystem
				inline const hst::string & message() const
				{ return m_message; } //!< Returns a message					
				const char * stime() const; //!< A string-alike time
				inline const time_t & time() const
				{ return m_time; }   //!< Returns a time
				inline sad::log::Priority priority() const { return m_priority; } //!< A priority
				inline const hst::string & upriority() const
				{ return m_user_priority; } //!< Returns a user priority
				/*! Returns string like "name of file", "line"				
				 */
				hst::string fileline() const;
				/*! Returns a stringified priority
				 */
				const char * spriority() const;
				/*! A file constant
				 */
				inline const char * file()  const { return m_file; }
			    /*! A line constant
				 */
				inline int line()  const { return m_line; }
		};
		/*! A console class, which implements all console functions
		 */
		class Console
		{
#ifdef WIN32
		 private:
				WORD m_oldattributes;
				HANDLE m_console;
				/*! Inits console information
				 */
				void initConsole();
#endif
		 public:
			 /*! Creates a new console
		          */
			 Console();
			 /*! Used to allocate console on Windows
			  */
		        virtual void createConsole() ;
			 /*! Sets a color mode for console
			  */
			virtual void setColorMode(sad::log::Color foreground, sad::log::Color background) ; 
			/*! Used to restore color modes in console
			 */
			virtual void clearColorMode();
			/*! Prines a text, using color mode
			 */
			virtual void print(const char * text) ;
			/*! Handles console
			 */
			virtual ~Console();
		};
		/*! A log target class is the one, which acts and works with
			output, redirecting it to class or console, depending on implementation 
			
			Note, that this is a basic class, so it is abstract
		 */
		class Target
		{
		public:
			/*! Receives a messages from targetting information
				\param[in] message taken message
			 */
			virtual void receive(const sad::log::Message & message) = 0;
			// Performs nothing
			virtual ~Target();
		};
		/*! A log, that targets a file. 
			File is opened at some other point, because it can fail
		 */
		class FileTarget: public sad::log::Target
		{
			protected:
				FILE * m_file; //!< Inner file handle
				int         m_max_priority; //!< Priority of max message
				hst::string m_format; //!< Format for outputting the message
				/*! Formats a subsystem part, by default adds ": "
					\return format string
				 */
				virtual std::string formatSubsystem(const sad::log::Message & message);
				/*! Formats file and line part, by default adds a space, or empty string
					\return format string
				 */
				virtual std::string formatFileLine(const sad::log::Message & message);
				/*! Closes a handle, if can, sets it to null
				 */
				virtual void close();
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
											Messages with priority maxpriority and bigger this are discarded
				 */
				FileTarget(const hst::string & format = "{0}: [{1}] {3}{2}{4}", int maxpriority = 6);
				/*! Opens a file
					\param[in] filename name of file
					\return true if ok
				 */
				bool open(const hst::string & filename);
				/*! Receives a messages from targetting information
					\param[in] message taken message
				 */
				virtual void receive(const sad::log::Message & message);
				/*! Set maximum level
					\param[in] level level message
				 */ 
				inline void setMaxLevel(int level) { m_max_priority = level; }
				/*! Destructs a file
				 */
				virtual ~FileTarget();
		};
		/*! A targets, which targets a console
		 */
		class ConsoleTarget: public sad::log::Target
		{
		  private:
				sad::log::Console * m_console; //!< A handle-like console
				int         m_max_priority; //!< Priority of max message
				hst::string m_format; //!< Format for outputting the message
				hst::hash<sad::log::Priority, 
				                 hst::pair<sad::log::Color, sad::log::Color>
						 > m_coloring; //!< Color schema (first is back, second is fg)
				/*! Formats a subsystem part, by default adds ": "
					\return format string
				 */
				virtual std::string formatSubsystem(const sad::log::Message & message);
				/*! Formats file and line part, by default adds a space, or empty string
					\return format string
				 */
				virtual std::string formatFileLine(const sad::log::Message & message);
				/*!  Fills colors with priorities
				 */
				void createColoredOutput();
				/*! Fills colors with normal data
		                 */
				void createNormalOutput();
		  public:
				/*! Creates console target  with specified format.
					Format defined as followes
					{0} - current time
					{1} - message priority
					{2} - formatSubsystem() result, by default, subsystem + ": ", nothing if subsystem is not specified. For example: "commit():", ""
					{3} - formatFileLine() result, file and line through ', ', nothing if not specified
					{4} - message text
					\param[in] format format string 
					\param[in] maxpriority Maximum priority for outputting. 
											Messages with priority maxpriority and bigger this are discarded
					\param[in] colored  whether output should be colored
					\param[in] allocate_console whether we should allocate console
				 */
				ConsoleTarget(const hst::string & format = "{0}: [{1}] {3}{2}{4}", int maxpriority = 6,  bool colored =  true, bool allocate_console = false);
				/*! Receives a messages from targetting information
				     \param[in] message  taken message
				 */
				virtual void receive(const sad::log::Message & message);
				/*! Destroys a target
				 */
			        ~ConsoleTarget();
		};
	};
	
	/*! Log class takes frontend work, builds a messages and broadcasts it
		it to all targets
	 */
	class Log: public ActionContext
	{
	 protected:
	        os::mutex m_lock;
	        /*! A vector of targets
		 */
		sad::vector<sad::log::Target *> m_targets;
		/*! Returns a current subsystem
			\return name of current subsystem
		 */
		virtual hst::string subsystem();
		
		virtual void createAndBroadcast(const hst::string & mesg, 
										sad::log::Priority priority,
										const char * file = NULL, 
										int line = 0,
										const hst::string & upriority = hst::string());
		template<typename T>
		void _createAndBroadcast(const T & mesg, 
								sad::log::Priority priority,
								const char * file = NULL, 
								int line = 0,
								const hst::string & upriority = hst::string())
		{
			createAndBroadcast(sad::log::StringCaster<T>::cast(mesg), priority, file, line, upriority);
		}
	 public:
		/*! Broadcasts a message to all targets
			\param[in] m message
		 */
		virtual void broadcast(const sad::log::Message & m);
		/*! Adds a target into a log
			\param[in] t target 
		 */
		virtual sad::Log & addTarget(sad::log::Target * t);
		/*! Removes a target from a log
			\param[in] t target
		 */
		virtual sad::Log & removeTarget(sad::log::Target * t);
		// Here are common interface for messages
		template<typename T> 
		void fatal(const T & mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(mesg, sad::log::FATAL, file, line);
		}
		
		// Here are common interface for messages
		template<typename T> 
		void critical(const T & mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(mesg, sad::log::CRITICAL, file, line);
		}
		
		template<typename T> 
		void warning(const T & mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(mesg, sad::log::WARNING, file, line);
		}
		
		template<typename T> 
		void message(const T & mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(mesg, sad::log::MESSAGE, file, line);
		}
		
		template<typename T> 
		void debug(const T & mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(mesg, sad::log::DEBUG, file, line);
		}
		
		template<typename T> 
		void user(const T & mesg, const char * file = NULL, int line = 0, const hst::string & user =  hst::string())
		{
			_createAndBroadcast(mesg, sad::log::USER, file, line, user);
		}


		// Overloads for const char *
		void fatal(const char * mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(hst::string(mesg), sad::log::FATAL, file, line);
		}
		
		// Here are common interface for messages

		void critical(const char * mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(hst::string(mesg), sad::log::CRITICAL, file, line);
		}
		
		void warning(const char * mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(hst::string(mesg), sad::log::WARNING, file, line);
		}
		
		void message(const char * mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(hst::string(mesg), sad::log::MESSAGE, file, line);
		}
		
		void debug(const char  * mesg, const char * file = NULL, int line = 0)
		{
			_createAndBroadcast(hst::string(mesg), sad::log::DEBUG, file, line);
		}
		
		void user(const char *  mesg, const char * file = NULL, int line = 0, const hst::string & user =  hst::string())
		{
			_createAndBroadcast(hst::string(mesg), sad::log::USER, file, line, user);
		}
		
		/*! Sets current action
			\param[in] str string
		 */
		virtual void pushAction(const hst::string & str);
		/*! Sets current action
			\param[in] str string
			\param[in] file file data
			\param[in] line line data
		 */
		virtual void pushAction(const hst::string & str, const char * file, int line);
		/*!  Pops an actions
		 */
		virtual void popAction();

		virtual ~Log();
		/*! Returns a renderer's log instance
		 */
		static Log * ref();
	};
	namespace log 
	{
	    /*! Scopes defines a current scope for log
			It pushes action to it when creating, and 
			pops it, when destroyed
		 */
		class Scope
		{
			private:
				sad::Log * m_log;
			public:
				/*! Pushes a new state
				 */
				Scope(const char * c, const char * file = NULL, int line = 0, sad::Log * log = sad::Log::ref());
				/*! Pushes a new state
				 */
				Scope(const hst::string & c, const char * file = NULL, int line = 0, sad::Log * log = sad::Log::ref());
				/*! Pushes a new state
				 */
				Scope(const std::string & c, const char * file = NULL, int line  = 0, sad::Log * log = sad::Log::ref());
				/*! Pushes a new state
				 */
				Scope(const fmt::internal::ArgInserter<char> & c, const char * file = NULL, int line  = 0, sad::Log * log = sad::Log::ref());
				
				~Scope();
		};
	}
};
 

#define SL_FATAL(X) sad::Log::ref()->fatal(X, __FILE__, __LINE__)
#define SL_CRITICAL(X) sad::Log::ref()->critical(X, __FILE__, __LINE__)
#define SL_WARNING(X) sad::Log::ref()->warning(X, __FILE__, __LINE__)
#define SL_MESSAGE(X) sad::Log::ref()->message(X, __FILE__, __LINE__)
#define SL_DEBUG(X) sad::Log::ref()->debug(X, __FILE__, __LINE__)
#define SL_USER(X, TYPE) sad::Log::ref()->user(X, __FILE__, __LINE__, TYPE)

#define SL_LOCAL_FATAL(X,R)    (R).log()->fatal(X, __FILE__, __LINE__)
#define SL_LOCAL_CRITICAL(X,R) (R).log()->critical(X, __FILE__, __LINE__)
#define SL_LOCAL_WARNING(X,R)  (R).log()->warning(X, __FILE__, __LINE__)
#define SL_LOCAL_MESSAGE(X,R)  (R).log()->message(X, __FILE__, __LINE__)
#define SL_LOCAL_DEBUG(X,R)    (R).log()->debug(X, __FILE__, __LINE__)
#define SL_LOCAL_USER(X, R, TYPE)  (R).log()->user(X, __FILE__, __LINE__, TYPE)


#define SL_SCOPE(X)  sad::log::Scope  _____1_____(X, __FILE__, __LINE__)
#define SL_LOCAL_SCOPE(X,R)  sad::log::Scope  _____1_____(X, __FILE__, __LINE__, (R).log())

