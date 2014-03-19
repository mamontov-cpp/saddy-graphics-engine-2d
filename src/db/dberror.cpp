#include "db/dberror.h"

DECLARE_SOBJ_INHERITANCE(sad::db::Error, sad::Object )
DECLARE_SOBJ_INHERITANCE(sad::db::NotImplemented, sad::db::Error )

sad::db::Error::~Error()
{

}

sad::db::NotImplemented::~NotImplemented()
{
	
}

