#include "stdafx.h"
#include "hwstring.h"
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include "hlist.hpp"

using namespace hst;

wstring::~wstring() {}
wstring::wstring() {}


hst::wstring wstring_from_charptr(const char * p)
{
	if (p == NULL) return hst::wstring();
	std::string tmp(p);
	std::wstring tmpres(tmp.begin(), tmp.end());
	return hst::wstring(tmpres);
}


wstring::wstring(const wchar_t * str): std::wstring(str)
{

}



wstring & wstring::operator<<(const wstring & o)
{
  this->append(o.begin(),o.end());
  return *this;
}

wstring & wstring::remove(unsigned int i)
{
  this->erase(i,1);
  return *this;
}
wstring & wstring::insert(unsigned int i,const wstring & str)
{
 this->insert(i,str);
 return *this;
}
wstring::wstring(const std::wstring & str): std::wstring(str)
{

}
wstring wstring::operator+(const wstring &str)
{
	std::wstring s = *this;
	return wstring(s+str);
}
bool wstring::operator==(const wstring & o) const
{
	const std::wstring & t1 = *this;
	const std::wstring & t2 = o;
	return t1 == t2;
}
bool wstring::operator!=(const wstring & o) const
{
	return !(*this==o);
}
hst::list<hst::wstring> wstring::split(const wstring & o)
{
	wchar_t * ptt=new wchar_t[this->length()+1];
	wcscpy(ptt,this->c_str());
	hst::list<hst::wstring> res;
	wchar_t * last=ptt;
	wchar_t * th;
	bool run=false;
	do
	{
		th=wcsstr(last,o.c_str());
		if (th!=NULL)
		{
			*th=0;
			res<<hst::wstring(last);
			run=true;
			last=th+o.length();
		}
	} while (th!=NULL);
	res<<hst::wstring(last);
	for (int i=0;i<res.count();i++)
	{
		if (res[i].empty())
		{
		 res.removeAt(i);
		 --i;
		}
	}
	delete ptt;
	return res;
}
void wstring::removeFirst(const wstring & o)
{
	unsigned int pos=this->find_first_of(o);
	if (pos!=std::wstring::npos) this->erase(pos,o.length());
}
void wstring::removeLast(const wstring & o)
{
	unsigned int pos=this->find_last_of(o);
	if (pos!=std::wstring::npos) this->erase(pos,o.length());
}
