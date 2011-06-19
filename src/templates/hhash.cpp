#include "hhash.hpp"
using namespace hst;


unsigned long hst::HashFunction<hst::string>::call(const hst::string & name,unsigned long size)
{
	unsigned long tmp=basicHash((unsigned char*)name.data());
	return tmp % size;
}
