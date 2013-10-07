#include "sadhash.h"

unsigned long sad::HashFunction<sad::String>::call(const sad::String & name,unsigned long size)
{
	unsigned long tmp=basicHash((unsigned char*)name.data());
	return tmp % size;
}
