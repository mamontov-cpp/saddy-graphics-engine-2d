/*! \file priority.h
    

    Defines a priority for every message in log
 */
#pragma once

namespace sad
{

namespace log
{

/*! Defines a priority of messages in log
    Messages can be disabled by it's priority.
    Also, when possible, they can be represented on screen by different colors
 */
enum class Priority: int
{
    FATAL = 6,    //!< A fatal message, is a message of which app cannot recover
    CRITICAL = 5, //!< A critical message marks serious error
    WARNING = 4,  //!< A warning for user
    MESSAGE = 3,  //!< A message, which we need to told to user
    DEBUG = 2,    //!< A simple debug message
    USER = 1,     //!< A user-generated message
    /** A special type of message, used by saddy to mark it's own inner messages, which
     *  you can capture in order to work with large debugs
     */
    SADDY_INTERNAL = 0  
};

}

}
