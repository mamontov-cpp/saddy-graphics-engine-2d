/*! \file logtarget.h
    

    Describes a  basic class, for all of objects, which can receive messages from the log,
    performing writing them to file or console
 */
#pragma once
#include "../refcountable.h"

namespace sad
{

namespace log
{
class Message;
/*! A log target class is the one, which works with
    output log messages, redirecting it to 
    file or console or other output device, depending on it's subclass

    To implement writing to your output device, you must subclass it.
            
    Note, that this is a basic class, so it is abstract
  */
class Target: public RefCountable
{
public:
    /*! Receives a messages from target information
        \param[in] message taken message
     */
    virtual void receive(const sad::log::Message & message) = 0;
    /*! Kept only for inheritance conformance. Does nothing
     */
    virtual ~Target();
};

}

}
