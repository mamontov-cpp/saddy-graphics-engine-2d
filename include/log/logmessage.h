/*! \file logmessage.h
    

    Defines a message, that can be logged
 */
#pragma once
#include "priority.h"
#include "../sadstring.h"
#include <ctime>

namespace sad
{

namespace log
{

/*! Describes a logging message, which is broadcast by sad::log::Log to all targets
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
    inline Message(const sad::String & message, //-V730
                   sad::log::Priority priority,
                   const char * file = nullptr,
                   int line = 0,
                   const sad::String & subsystem = sad::String(),
                   const sad::String & user_priority = sad::String()
                  )
    : m_buffer{0}, m_subsystem(subsystem), m_message(message), m_priority(priority),
      m_user_priority(user_priority), m_file(file), m_line(line)
    {
        m_time = ::time(nullptr);
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
    const char * timeAsString() const; 
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
    inline const sad::String & userPriority() const
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
    const char * priorityAsString() const;
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

}

}
