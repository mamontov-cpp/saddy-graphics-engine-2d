/*! \file log.h
	\author HiddenSeeker
	
	Describes a basic log and log parameters
 */
#include "../3rdparty/format/format.h"
#include "../templates/hstring.h"
#include <ctime>
#include <string>
#pragma once

namespace sad
{
	namespace log
	{
		template<typename T>
		class LogStringCaster
		{
			/*! A caster for helping string find their ways
			 */
			static hst::string cast(const T & string);
		};
		template<>
		class LogStringCaster<hst::string>
		{
			/*! A caster for helping string find their ways
			 */
			static hst::string cast(const hst::string> & string);
		};
		template<>
		class LogStringCaster<std::string>
		{
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
	};
	
	
	


};
 