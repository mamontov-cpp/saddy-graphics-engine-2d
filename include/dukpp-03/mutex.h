/*! \file mutex.h
    
    Defines a scriptable ref-countable mutex
 */
#pragma once
#include "../sadmutex.h"
#include "../refcountable.h"

namespace sad
{

namespace dukpp03
{
    
class Mutex: public sad::Mutex, public sad::RefCountable
{
public:
    /*! Constructs new mutex
     */
    Mutex();
    /*! Can be inherited
     */
    virtual ~Mutex();
};

}

}
