#include "sadwstring.h"
#include <wchar.h>
#include <stdlib.h>
#include <string.h>



sad::WString::~WString() {}
sad::WString::WString() {}


sad::WString sad::widestring_from_charpointer(const char * p)
{
    if (p == nullptr) return sad::WString();
    std::string tmp(p);
    std::wstring tmpres(tmp.begin(), tmp.end());
    return sad::WString(tmpres);
}


sad::WString::WString(const wchar_t * str): std::wstring(str)
{

}



sad::WString & sad::WString::operator<<(const sad::WString & o)
{
  this->append(o.begin(),o.end());
  return *this;
}

sad::WString & sad::WString::remove(unsigned int i)
{
  this->erase(i,1);
  return *this;
}
sad::WString & sad::WString::insert(unsigned int i,const sad::WString & str)
{
 this->std::wstring::insert(i,str);
 return *this;
}
sad::WString::WString(const std::wstring & str): std::wstring(str)
{

}
sad::WString sad::WString::operator+(const sad::WString &str)
{
    std::wstring s = *this;
    return sad::WString(s+str);
}
bool sad::WString::operator==(const sad::WString & o) const
{
    const std::wstring & t1 = *this;
    const std::wstring & t2 = o;
    return t1 == t2;
}
bool sad::WString::operator!=(const sad::WString & o) const
{
    return !(*this==o);
}
sad::Vector<sad::WString> sad::WString::split(const sad::WString & o,  sad::String::SplitBehaviour b)
{
    wchar_t * ptt=new wchar_t[this->length() + 1];
    wcscpy(ptt,this->c_str());
    sad::Vector<sad::WString>  res;
    wchar_t * last=ptt;
    wchar_t * th;
    bool run=false;
    do
    {
        th=wcsstr(last,o.c_str());
        if (th!=nullptr)
        {
            *th=0;
            res<<sad::WString(last);
            run=true;
            last=th+o.length();
        }
    } while (th!=nullptr);
    res<<sad::WString(last);
    for (size_t i=0;i<res.count();i++)
    {
        if (res[i].empty() || b != sad::String::SplitBehaviour::KEEP_EMPTY_PARTS)
        {
         res.removeAt(i);
         --i;
        }
    }
    delete[] ptt;
    return res;
}
void sad::WString::removeFirst(const sad::WString & o)
{
    unsigned int pos=this->find_first_of(o);
    if (pos!=std::wstring::npos) this->erase(pos,o.length());
}
void sad::WString::removeLast(const sad::WString & o)
{
    unsigned int pos=this->find_last_of(o);
    if (pos!=std::wstring::npos) this->erase(pos,o.length());
}
