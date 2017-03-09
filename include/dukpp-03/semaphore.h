/*! \file semaphore.h
    
    Defines a scriptable ref-countable thread
 */
#pragma once
#include "../sadsemaphore.h"
#include "../refcountable.h"

namespace sad
{

namespace dukpp03
{
    
class Semaphore: public sad::Semaphore, public sad::RefCountable
{
public:
    /*! Constructs new semaphore with specified values 
        \param[in] currentvalue currentvalue of semaphore
        \param[in] maxvalue maximum value of semaphore
     */
    Semaphore(unsigned int currentvalue = 0, unsigned int maxvalue = 32767);
    /*! Can be inherited
     */
    virtual ~Semaphore();
};

}

}
