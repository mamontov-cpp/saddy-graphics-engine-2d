/*! \file log.h
    
    
    Describes a basic log and log parameters
 */
#pragma once
#include "priority.h"
#include "stringcaster.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "color.h"
#include "logmessage.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "filetarget.h"
#include "consoletarget.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "logscope.h"

#include "../sadvector.h"
#include "../sadstring.h"
#include "../sadmutex.h"




namespace sad
{

namespace log
{
    
/*! Log class takes all the front work, building a messages 
    and broadcasting them to all targets.

    If you want to register some events, writing it to log, you should use 
    this class.
 */
class Log
{
public:
    /*! Broadcasts a message to all targets
        \param[in] m message
     */
    virtual void broadcast(const sad::log::Message & m);
    /*! Adds a target to a list of targets. A messages, created with 
        sad::log::Log::broadcast() will be broadcast to this target.
        \param[in] t target 
     */
    virtual sad::log::Log & addTarget(sad::log::Target * t);
    /*! Removes a target from a list of targets. 
            
        Note, that memory from target is not freed. You must delete it manually.

        \param[in] t target 
     */
    virtual sad::log::Log & removeTarget(sad::log::Target * t);
    /*! Broadcasts a message of priority FATAL to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */
    template<typename T> 
    void fatal(const T & message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(message, sad::log::Priority::FATAL, file, line);
    }
    /*! Broadcasts a message of priority CRITICAL to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */
    template<typename T> 
    void critical(const T & message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(message, sad::log::Priority::CRITICAL, file, line);
    }
    /*! Broadcasts a message of priority WARNING to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    template<typename T> 
    void warning(const T & message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(message, sad::log::Priority::WARNING, file, line);
    }
    /*! Broadcasts a message of priority MESSAGE to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    template<typename T> 
    void message(const T & message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(message, sad::log::Priority::MESSAGE, file, line);
    }
    /*! Broadcasts a message of priority DEBUG to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    template<typename T> 
    void debug(const T & message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(message, sad::log::Priority::DEBUG, file, line);
    }
    /*! Broadcasts a message of user priority to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
        \param[in] user a user priority name
     */		
    template<typename T> 
    void user(
        const T & message, 
        const char * file = nullptr, 
        int line = 0, 
        const sad::String & user =  sad::String()
    )
    {
        _createAndBroadcast(message, sad::log::Priority::USER, file, line, user);
    }
    /*! Broadcasts a message of SADDY_INTERNAL to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    template<typename T> 
    void saddyInternal(
        const T & message, 
        const char * file = nullptr, 
        int line = 0
    )
    {
        _createAndBroadcast(message, sad::log::Priority::SADDY_INTERNAL, file, line);
    }
    /*! Broadcasts a message of priority FATAL to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */
    void fatal(const char * message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(sad::String(message), sad::log::Priority::FATAL, file, line);
    }
    /*! Broadcasts a message of priority CRITICAL to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */
    void critical(const char * message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(sad::String(message), sad::log::Priority::CRITICAL, file, line);
    }
    /*! Broadcasts a message of priority WARNING to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    void warning(const char * message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(sad::String(message), sad::log::Priority::WARNING, file, line);
    }
    /*! Broadcasts a message of priority MESSAGE to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    void message(const char * message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(sad::String(message), sad::log::Priority::MESSAGE, file, line);
    }
    /*! Broadcasts a message of priority DEBUG to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    void debug(const char  * message, const char * file = nullptr, int line = 0)
    {
        _createAndBroadcast(sad::String(message), sad::log::Priority::DEBUG, file, line);
    }
    /*! Broadcasts a message of user priority to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
        \param[in] user a user priority name
     */		
    void user(const char *  message, const char * file = nullptr, int line = 0, const sad::String & user =  sad::String())
    {
        _createAndBroadcast(sad::String(message), sad::log::Priority::USER, file, line, user);
    }
    /*! Broadcasts a message of SADDY_INTERNAL to all targets
        \param[in] message message
        \param[in] file source file name, where message was created
        \param[in] line source file line, where message was created
     */		
    template<typename T> 
    void saddyInternal(
        const char * message, 
        const char * file = nullptr, 
        int line = 0
    )
    {
        _createAndBroadcast(sad::String(message), sad::log::Priority::SADDY_INTERNAL, file, line);
    }
    /*! Pushes current subsystem name into stack of subsystem names
        \param[in] str string
     */
    virtual void pushSubsystem(const sad::String & str);
    /*! Pushes current subsystem name into stack of subsystem names
        \param[in] str string
        \param[in] file a name of source file, where entering to subsystem occurred
        \param[in] line a source file line, where entering 
     */
    virtual void pushSubsystem(const sad::String & str, const char * file, int line);
    /*! Pops last subsystem name from the stack of subsystem names
     */
    virtual void popSubsystem();
    /*! Frees memory from all targets
     */
    virtual ~Log();
    /*! Returns a global renderer's log instance
     */
    static Log * ref();
    /*! Creates new empty log
     */
    inline Log() 
    {
        m_internal_mode = false;
    }
    /*! Sets internal mode. If true, on sad::log::Log::pushAction, 
        log will generate internal messages
        \param[in] mode a new internal mode
     */
    inline void setInternalMode(bool mode)
    {
        m_internal_mode = mode;
    }
protected:
    /*! Locks, for making this compatible with multi-threading applications
     */
    ::sad::Mutex m_lock;
    /*! Determines, whether log is in internal mode. If internal mode, 
        sad::log::Log::pushAction creates an internal message
     */ 
    bool        m_internal_mode;
    /*! A vector of targets, for broadcasting file
     */
    sad::Vector<sad::log::Target *> m_targets;
    /*! A subsystems in calling-stack order. Pushed with 
        sad::log::Log::pushSubsystem(), popped with sad::log::Log::popSubsystem()
     */
    sad::Vector<sad::String> m_subsystems;
    /*! Returns a current subsystem
        \return name of current subsystem
     */
    virtual sad::String subsystem();
    /*! Creates new message and broadcasts them to all contained targets
        \param[in] message text message, that is being logged
        \param[in] priority  a priority for message
        \param[in] file      a name of file, where message was emitted
        \param[in] line      a line number of line, where message was emitted
        \param[in] user_priority use-defined priority
     */
    virtual void createAndBroadcast(
        const sad::String & message, 
        sad::log::Priority priority,
        const char * file = nullptr, 
        int line = 0,
        const sad::String & user_priority = sad::String()
    );
    /*! Creates new message, converts it to string 
        and broadcasts them to all contained targets
        \param[in] message       message, that is being logged
        \param[in] priority  a priority for message
        \param[in] file      a name of file, where message was emitted
        \param[in] line      a line number of line, where message was emitted
        \param[in] user_priority  user-defined priority
     */
    template<typename T>
    void _createAndBroadcast(
        const T & message, 
        sad::log::Priority priority,
        const char * file = nullptr, 
        int line = 0,
        const sad::String & user_priority = sad::String()
    )
    {
        createAndBroadcast(sad::log::StringCaster<T>::cast(message), priority, file, line, user_priority);
    }
};

}

}

/*! Creates new message in global log with message X and priority FATAL
 */
#define SL_FATAL(X) sad::log::Log::ref()->fatal(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority CRITICAL
 */
#define SL_CRITICAL(X) sad::log::Log::ref()->critical(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority WARNING
 */
#define SL_WARNING(X) sad::log::Log::ref()->warning(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority MESSAGE
 */
#define SL_MESSAGE(X) sad::log::Log::ref()->message(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority DEBUG
 */
#define SL_DEBUG(X) sad::log::Log::ref()->debug(X, __FILE__, __LINE__)
/*! Creates new user message in global log with message X and priority TYPE
 */
#define SL_USER(X, TYPE) sad::log::Log::ref()->user(X, __FILE__, __LINE__, TYPE)
/*! Creates new message in global log with message X and priority SADDY_INTERNAL
 */
#define SL_INTERNAL(X) sad::log::Log::ref()->saddyInternal(X, __FILE__, __LINE__)

/*! Creates new message in local log R with message X and priority FATAL
 */
#define SL_LOCAL_FATAL(X,R)    (R).log()->fatal(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority CRITICAL
 */
#define SL_LOCAL_CRITICAL(X,R) (R).log()->critical(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority WARNING
 */
#define SL_LOCAL_WARNING(X,R)  (R).log()->warning(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority MESSAGE
 */
#define SL_LOCAL_MESSAGE(X,R)  (R).log()->message(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority DEBUG
 */
#define SL_LOCAL_DEBUG(X,R)    (R).log()->debug(X, __FILE__, __LINE__)
/*! Creates new user message in local log R with message X and priority TYPE
 */
#define SL_LOCAL_USER(X, R, TYPE)  (R).log()->user(X, __FILE__, __LINE__, TYPE)
/*! Creates new message in local log R with message X and priority SADDY_INTERNAL
 */
#define SL_LOCAL_INTERNAL(X, R) (R).log()->saddyInternal(X, __FILE__, __LINE__)


/*! Created new internal message in local log of renderer R 
    with message X and priority SADDY_INTERNAL, if
    pointer to renderer R is not null
 */
#define SL_COND_LOCAL_INTERNAL(X, R)   \
    {                                  \
       if (R)                          \
       {                               \
          (R)->log()->saddyInternal(X, __FILE__, __LINE__);  \
       }                               \
    }


/*! Created new fatal message in local log of renderer R 
    with message X and priority SADDY_INTERNAL, if
    pointer to renderer R is not null
 */
#define SL_COND_LOCAL_FATAL(X, R)      \
    {                                  \
       if (R)                          \
       {                               \
          (R)->log()->fatal(X, __FILE__, __LINE__);  \
       }                               \
    }
