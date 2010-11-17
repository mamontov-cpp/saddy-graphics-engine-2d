#include "stdafx.h"
#include "hwstring.h"
#include <wchar.h>
#include <stdlib.h>
#include "hlist.hpp"

using namespace hst;

wstring::~wstring() {}
wstring::wstring() {}


wstring::wstring(const wchar_t * str)
{
  m_str=std::wstring(str);
}

wstring::wstring(const char * str)
{
  size_t length=strlen(str)+1;
  wchar_t * s=(wchar_t*)malloc(length*sizeof(wchar_t));
  mbtowc(s,str,length);
  m_str=std::wstring(s);
  free(s);
}

wstring & wstring::operator=(const wstring & o)
{
  m_str=o.m_str;
  return *this;
}
wstring & wstring::operator<<(const wstring & o)
{
  m_str.append(o.m_str.begin(),o.m_str.end());
  return *this;
}
wstring & wstring::remove(unsigned int i)
{
  m_str.erase(i,1);
  return *this;
}
wstring & wstring::insert(unsigned int i,const wstring & str)
{
 m_str.insert(i,str.m_str);
 return *this;
}
wstring::wstring(const std::wstring & str)
{
	m_str=str;
}
wstring wstring::operator+(const wstring &str)
{
	return wstring(m_str+str.m_str);
}
bool wstring::operator==(const wstring & o) const
{
	return m_str==o.m_str;
}
bool wstring::operator!=(const wstring & o) const
{
	return m_str!=o.m_str;
}
hst::list<hst::wstring> wstring::split(const wstring & o)
{
	wchar_t * ptt=new wchar_t[m_str.length()+1];
	wcscpy(ptt,m_str.c_str());
	hst::list<hst::wstring> res;
	wchar_t * last=ptt;
	wchar_t * th;
	bool run=false;
	do
	{
		th=wcsstr(last,o.m_str.c_str());
		if (th!=NULL)
		{
			*th=0;
			res<<hst::wstring(last);
			run=true;
			last=th+o.m_str.length();
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
	unsigned int pos=m_str.find_first_of(o.m_str);
	if (pos!=std::wstring::npos) m_str.erase(pos,o.m_str.length());
}
void wstring::removeLast(const wstring & o)
{
	unsigned int pos=m_str.find_last_of(o.m_str);
	if (pos!=std::wstring::npos) m_str.erase(pos,o.m_str.length());
}