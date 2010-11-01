#include "hhash.hpp"
using namespace hst;

template<> 
unsigned long hst::getHash(const hst::string & name,unsigned long size)
{
	unsigned long tmp=basicHash((unsigned char*)name.data());
	return tmp % size;
}