/*! \file log.h
	\author HiddenSeeker
	
	Describes a basic log and log parameters
 */
#include "../3rdparty/format/format.h"
#include "../templates/hlvector.hpp"
#include "../templates/hstring.h"
#include "../marshal/actioncontext.h"
#include <ctime>
#include <string>
#pragma once

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
			static hst::string cast(const T & string);
		};
		template<>
		class StringCaster<hst::string>
		{
		 public:
			/*! A caster for helping string find their ways
			 */
			static inline hst::string cast(const hst::string & string);
		};
		template<>
		class StringCaster<std::string>
		{
		 public:
			/*! A caster for helping string find their ways
			 */
			static hst::string cast(const std::string & string);
		};
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
				hst::string fileline();
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
		/*! A log target class is the one, which acts and works with
			output, redirecting it to class or console, depending on implementation 
			
			Note, that this is a basic class, so it is abstract
		 */
		class Target
		{
		public:
			/*! Receives a messages from targetting information
				\param[in] message, taken message
			 */
			virtual void receive(const sad::log::Message & message) = 0;
			// Performs nothing
			virtual ~Target();
		};
		
	};
	
	/*! Log class takes frontend work, builds a messages and broadcasts it
		it to all targets
	 */
	class Log: public ActionContext
	{
	 protected:
	    /*! A vector of targets
		 */
		hst::vector<sad::log::Target *> m_targets;
		/*! Returns a current subsystem
			\retrun subsystem
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
		
		virtual ~Log();
	};
	


};
 