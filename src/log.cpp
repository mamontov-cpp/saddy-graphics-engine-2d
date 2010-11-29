#include "log.h"
#include "hstring.h"
#include <stdio.h>
#include <stdlib.h>

using namespace hst;

log * hst::log::m_instance=NULL;

hst::log::log()
{
}

void hst::log::freeInst()
{
	delete m_instance;
}
hst::log * hst::log::inst()
{
	if (!m_instance)
	{
		m_instance=new hst::log();
		atexit(&(log::freeInst));
	}
    return m_instance;
}

std::string  hst::log::str()
{
	return inst()->m_lg.str();
}

void hst::log::clear()
{
	inst()->m_lg.clear();
}

void hst::log::save(const char * fn )
{
	if (fn)
	{
	 FILE * file =fopen(fn,"wt+");
	 if (file!=NULL)
	 {
		fputs(inst()->m_lg.str().c_str(),file);
		fclose(file);
	 }
	}
}

std::ostringstream & operator<<(std::ostringstream & o, const hst::string & s)
{
	o<<std::string(s.data());
	return o;
}
std::ostream & operator<<(std::ostream & o, const hst::string & s)
{
	o<<std::string(s.data());
	return o;
}