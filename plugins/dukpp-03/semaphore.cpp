#include "dukpp-03/semaphore.h"
#include "db/dbtypename.h"

sad::dukpp03::Semaphore::Semaphore(unsigned int currentvalue, unsigned int maxvalue) : sad::Semaphore(currentvalue, maxvalue)
{
    
}

sad::dukpp03::Semaphore::~Semaphore()
{
    
}


DECLARE_COMMON_TYPE(sad::dukpp03::Semaphore)
