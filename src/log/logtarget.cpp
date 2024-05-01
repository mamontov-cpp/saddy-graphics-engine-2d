#include <log/logtarget.h>
#include <db/dbtypename.h>
#include "opticksupport.h"

sad::log::Target::~Target()
{
    PROFILER_EVENT;

}

DECLARE_COMMON_TYPE(sad::log::Target)
