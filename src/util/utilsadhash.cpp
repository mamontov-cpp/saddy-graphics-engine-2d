#include "sadhash.h"

#ifdef USE_OLD_SAD_HASH_IMPLEMENTATION 

unsigned long sad::HashFunction<sad::String>::call(const sad::String & name,unsigned long size)
{
    unsigned long tmp=basicHash((unsigned char*)name.data());
    return tmp % size;
}

#endif

// Disable warnings about unreachable parts for this compilation unit for util sad::Hash
namespace sad
{
	
namespace util
{
	
namespace hash
{

void ___dummy()
{
	
}

}

}

}
