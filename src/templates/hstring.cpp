#include "hstring.h"
#include <stdlib.h>
#include <string.h>

using namespace hst;

string::string(): std::string() 
{

}
string::string(const char* _p) : std::string(_p)  
{ 

}
string::string(const char* _p,long len) : std::string(_p, len)
{

}

string::string(const string & o) : std::string(o)
{
 
}
string::string(const std::string & o) : std::string(o)
{
 
}
string::~string()
{

}

void string::print() const
{
	puts(this->c_str());
}
//----Getters-------------------------------
char * string::data()   const {return const_cast<char*>(this->c_str());}
bool   string::empty()  const {return this->length() == 0;}
//------------------------------------------

string & string::operator<<(char c)
{
  this->push_back(c);
  return *this;
}

bool hst::string::operator==(const hst::string & o) const
{
	const std::string & p1 = (const std::string&)(*this);
	const std::string & p2 = (const std::string&)(o);	
	return p1.compare(p2) == 0;
}

bool  hst::string::operator!=(const hst::string & o) const
{
	return !(*this == o);
}

bool hst::string::operator==(const char * o) const
{
	return (*this == hst::string(o));
}

bool hst::string::operator!=(const char * o) const
{
	return (*this == hst::string(o));
}

bool string::operator>(const string &o) const
{
	unsigned long minlen=(this->length()<o.length())?this->length():o.length();
	unsigned long i;
	for (i=0;i<minlen;i++)
	{
	  if ((*this)[i]!=o[i])
		if (cmpchar((*this)[i],o[i]))
			return false;
		else
			return true;
	}
	if (this->length()>o.length())
		return true;
	return false;
}
bool string::operator>=(const string & o) const
{
	return (*this>o) || (*this==o);
}
bool string::operator<=(const string & o) const
{
	return !(*this>o);
}
bool string::operator<(const string & o) const
{
	return !(*this>=o);
}
void string::remove(long i)
{
	if (i > -1 && i <= (long)(this->length()))
	{
		this->erase(this->begin() + i);
	}
}

//Split a string to a list, using specified delimiter
string & string::operator<<(const string & o)
{
	(*this)+=o;
	return *this;
}
stringlist string::split(const char * delimiters) const
{
  hst::stringlist result;
  hst::string buffer;
  for(unsigned long i = 0; i < this->length(); i++)
  {
	char cur = (*this)[i];
	if (strrchr(delimiters,cur) != NULL)
	{
		if (buffer.empty() == false)
			result << buffer;
		buffer.clear();
	}
	else 
	{
		buffer << cur;
	}
  }
  if (buffer.empty() == false)
      result << buffer;
  return result;
}
stringlist string::split(char delimiter) const
{
	char s[2]={delimiter, 0x0};
	return split(s);
}
void string::readLine(FILE** pf)
{
	char c=0x00;
	this->clear();
	while((c=fgetc(*pf))=='\r' || (c=='\n') && (!feof(*pf))) {};         //Skipping strings
    if ((c!='\r') && (c!='\n') && (c!=EOF)) (*this)<<c;                 //Appending first character
	while((c=fgetc(*pf))!='\r' && (c!='\n') && (!feof(*pf))) (*this)<<c; //Append character
}

string  string::getLastCharacters(long i) const
{
	if (i<0) return string();
	if ((unsigned long)i>=this->length()) return string();
	return this->substr(this->length()-i);
}
string string::getExtension() const
{
   size_t pos =  this->rfind('.');
   if (pos == std::string::npos)  return string();
   return this->substr(pos + 1);
}
void string::removeExtension()
{
   size_t pos =  this->rfind('.');
   if (pos == std::string::npos)  return;
   this->erase(this->begin() + pos, this->end());
}
void  string::addExtension(const string & newext)
{
	(*this)<<'.'<<newext;
}
void  string::changeExtension(const string & newext)
{
 removeExtension();
 addExtension(newext);
}
string  string::operator+(const string & o) const
{
   string result(*this);
   result << o;
   return result;
}

string  string::operator+(const std::string & o) const
{
	return *this + hst::string(o);
}

string  string::operator+(const char * o) const
{
	return *this + hst::string(o);
}

string & string::insert(char c,long i)
{
	if (i<0) i=0;
	if ((unsigned long)i>=length())  return (*this)<<c;
	std::string str;
	str.push_back(c);
	this->std::string::insert(i,str);
	return *this;
}

void string::trimSpaces()
{
   unsigned long i;
   int flag=1;
   for (i=0;(i<length()) && (flag);i++)
   {
		if ((*this)[i]!=' ')
		{
			flag=0;
		}
		else
		{
			remove(i);
			--i;
		}
   }
   flag=1;
   for (i=length()-1;(i>-1) && (flag);i--)
   {
	if ((*this)[i]!=' ')
	{
			flag=0;
	}
	else
	{
			remove(i);
			++i;
	}
   }
}
void string::removeSpaces()
{
	this->removeAllOccurences(string(" "));
}

string & string::removeRange(long beg,long rlen)
{
	long k;
	for (k=0;k<rlen;k++) //Remove old variable definition
		 this->remove(beg);    //Remove
	return *this;
}
string & string::insert(const string & o,long i)
{
	if (i < 0) return *this;
	if ((unsigned long)i > this->length()) 
	{
		*this << o;
		return *this;
	}
	this->std::string::insert(i, o);
	return *this;
}

string  string::number(int a,int radix)
{
char buffer[40];
#ifdef __HAS_A_ITOA_CONVERSION
 _itoa(a,buffer,radix);
 string result((const char*)buffer);
 return result;
#else
   if (radix==8) {sprintf(buffer,"%o",a);return string(buffer);}
   if (radix==10) {sprintf(buffer,"%d",a);return string(buffer);}
   if (radix==16) {sprintf(buffer,"%X",a);return string(buffer);}
   return string();
#endif
}
bool  string::queryPointer(const string & str, long * addr)
{
   long cwt=-32768;
   if (str.empty())
   {
	   return 0;
   }
   sscanf(str.data(),"%lX",&cwt);
   *addr=0;
   if (cwt>=0)
   {
       *addr=cwt; return 1;
   }
   return 0;
}
int string::toInt(const string & str)
{
    int result=0;
    sscanf(str.data(),"%d",&result);
    return result;
}
float string::toFloat(const string & str)
{
    float result=0;
    sscanf(str.data(),"%f",&result);
    return result;
}
string  string::subString(long beg,long len) const
{
	return substr(beg, len);
}
string  string::getRightPart(long len)
{
 return subString(length()-len,len);
}
string  string::getLeftPart(long len)
{
 return subString(0,len);
}
long string::getOccurences(const string & sstr)
{
	long count = 0;
	size_t pos = this->find(sstr);
	while(pos != std::string::npos)
	{
		++count;
		pos = this->find(sstr, pos + sstr.length());
	}
	return count;
}
long string::getOccurence(const string & sstr,long omax)
{
  long result = -1;
  long count = 0;
  size_t pos = this->find(sstr);
  while(pos != std::string::npos)
  {
    if (count == omax)
		result = pos;
    ++count;
	pos = this->find(sstr, pos + sstr.length());
  }
  return result;
}
long string::getLastOccurence(const string & sstr)
{
  long result = -1;
  size_t pos = this->find(sstr);
  while(pos != std::string::npos)
  {
	result = pos;
	pos = this->find(sstr, pos + sstr.length());
  }
  return result;
}
void string::removeOccurence(const string & sstr,long omax)
{
   long pos=getOccurence(sstr,omax);
   if (pos!=-1) removeRange(pos,sstr.length());
}
void string::removeAllOccurences(const string & sstr)
{
   long pos=getOccurence(sstr,0);
   while (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   pos=getOccurence(sstr,0);
   }
}
void string::removeLastOccurence(const string & sstr)
{
   long pos=getLastOccurence(sstr);
   if (pos!=-1) removeRange(pos,sstr.length());
}
void string::replaceOccurence(const string & sstr,const string & to,long omax)
{
   long pos=getOccurence(sstr,omax);
   if (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   insert(to,pos);
   }
}
void string::replaceAllOccurences(const string & sstr,const string & to)
{
   long pos=getOccurence(sstr,0);
   while (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   insert(to,pos);
	   pos=getOccurence(sstr,0);
   }
}
void string::replaceLastOccurence(const string & sstr,const string & to)
{
   long pos=getLastOccurence(sstr);
   if (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   insert(to,pos);
   }
}
bool string::cmpchar(char c1,char c2) const //Return false if c1 is bigger than c2
{
	int priority1,priority2;
	if (c1>=65 && c1<=90)
		priority1=8;
	else if (c1>=97 && c1<=122)
		priority1=7;
	else if (c1<=-123)
		priority1=6;
	else if (c1==-16)
		priority1=5;
	else if (c1>=-122 && c1<=-97)
		priority1=4;
	else if (c1>=-96 && c1<=-91)
		priority1=3;
	else if (c1==-15)
		priority1=2;
	else if ((c1>=-90 && c1<=-81) || (c1>=-32 && c1<=-17))
		priority1=1;
	else priority1=0;

    if (c2>=65 && c2<=90)
		priority2=8;
	else if (c2>=97 && c2<=122)
		priority2=7;
	else if (c2<=-123)
		priority2=6;
	else if (c2==-16)
		priority2=5;
	else if (c2>=-122 && c2<=-97)
		priority2=4;
	else if (c2>=-96 && c2<=-91)
		priority2=3;
	else if (c2==-15)
		priority2=2;
	else if ((c2>=-90 && c2<=-81) || (c2>=-32 && c2<=-17))
		priority2=1;
	else priority2=0;
    if (priority1>priority2) return true;
	else if (priority1<priority2) return false;
	return c1<c2;
}

hst::string join(const hst::stringlist list, const hst::string & sep)
{
	if (list.count() == 0)
		return hst::string();
	hst::string result = list[0];
	for(unsigned int i = 0; i < list.count(); i++)
	{
		result << sep << list[i];
	}
	return result;
}

