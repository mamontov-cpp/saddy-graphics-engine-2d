#include "db/dberror.h"

DECLARE_SOBJ_INHERITANCE(sad::db::Error, sad::Object )
DECLARE_SOBJ_INHERITANCE(sad::db::NotImplemented, sad::db::Error )
DECLARE_SOBJ_INHERITANCE(sad::db::InvalidPointer, sad::db::Error )

sad::db::Error::~Error()
{

}

sad::db::NotImplemented::~NotImplemented()
{
	
}

sad::db::InvalidPointer::~InvalidPointer()
{
	
}