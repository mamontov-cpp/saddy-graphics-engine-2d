#include "db/dberror.h"
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::db::Error, sad::Object )
DECLARE_SOBJ_INHERITANCE(sad::db::NotImplemented, sad::db::Error )
DECLARE_SOBJ_INHERITANCE(sad::db::InvalidPointer, sad::db::Error )

sad::db::Error::~Error() throw()
{
    PROFILER_EVENT;

}

sad::db::NotImplemented::~NotImplemented() throw()
{
    PROFILER_EVENT;
    
}

sad::db::InvalidPointer::~InvalidPointer() throw()
{
    PROFILER_EVENT;
    
}
