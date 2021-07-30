#include "dukpp-03/semaphore.h"
#include "db/dbtypename.h"

sad::dukpp03::Semaphore::Semaphore(unsigned int current_value, unsigned int max_value) : sad::Semaphore(current_value, max_value)
{
    
}

sad::dukpp03::Semaphore::~Semaphore()
{
    
}


DECLARE_COMMON_TYPE(sad::dukpp03::Semaphore)
