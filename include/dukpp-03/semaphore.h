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
        \param[in] current_value current_value of semaphore
        \param[in] max_value maximum value of semaphore
     */
    Semaphore(unsigned int current_value = 0, unsigned int max_value = 32767);
    /*! Can be inherited
     */
    virtual ~Semaphore();
};

}

}
