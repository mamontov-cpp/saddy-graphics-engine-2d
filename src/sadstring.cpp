#include "../include/sadstring.h"
#include <stdlib.h>
#include <string.h>


sad::String::String(): std::string() 
{

}
sad::String::String(const char* _p) :  std::string(_p)  
{ 

}
sad::String::String(const char* _p,long len) :  std::string(_p, len)
{

}

sad::String::String(const sad::String & o) :  std::string(o)
{
 
}
sad::String::String(const std::string & o) :  std::string(o)
{
 
}
sad::String::~String()
{

}

//----Getters-------------------------------
char * sad::String::data()   const {return const_cast<char*>(this->c_str());}
bool   sad::String::empty()  const {return this->length() == 0;}
//------------------------------------------

sad::String & sad::String::operator<<(char c)
{
  this->push_back(c);
  return *this;
}

bool sad::String::operator>(const sad::String &o) const
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
bool sad::String::operator>=(const sad::String & o) const
{
	return (*this>o) || (*this==o);
}
bool sad::String::operator<=(const sad::String & o) const
{
	return !(*this>o);
}
bool sad::String::operator<(const sad::String & o) const
{
	return !(*this>=o);
}
void sad::String::remove(long i)
{
	if (i > -1 && i <= (long)(this->length()))
	{
		this->erase(this->begin() + i);
	}
}

//Split a sad::String to a list, using specified delimiter
sad::String & sad::String::operator<<(const sad::String & o)
{
	(*this)+=o;
	return *this;
}

sad::StringList sad::String::split(const char * delimiters) const
{
  sad::StringList result;
  sad::String buffer;
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
sad::StringList sad::String::split(char delimiter) const
{
	char s[2]={delimiter, 0x0};
	return split(s);
}

sad::String  sad::String::getLastCharacters(long i) const
{
	if (i<0) return sad::String();
	if ((unsigned long)i>=this->length()) return sad::String();
	return this->substr(this->length()-i);
}
sad::String sad::String::getExtension() const
{
   size_t pos =  this->rfind('.');
   if (pos == std::string::npos)  return sad::String();
   return this->substr(pos + 1);
}
void sad::String::removeExtension()
{
   size_t pos =  this->rfind('.');
   if (pos == std::string::npos)  return;
   this->erase(this->begin() + pos, this->end());
}
void  sad::String::addExtension(const sad::String & newext)
{
	(*this)<<'.'<<newext;
}
void  sad::String::changeExtension(const sad::String & newext)
{
 removeExtension();
 addExtension(newext);
}
sad::String  sad::String::operator+(const sad::String & o) const
{
   sad::String result(*this);
   result << o;
   return result;
}

sad::String  sad::String::operator+(const std::string & o) const
{
	return *this + sad::String(o);
}

sad::String  sad::String::operator+(const char * o) const
{
	return *this + sad::String(o);
}

sad::String & sad::String::insert(char c,long i)
{
	if (i<0) i=0;
	if ((unsigned long)i>=length())  return (*this)<<c;
	std::string str;
	str.push_back(c);
	this->std::string::insert(i,str);
	return *this;
}

void sad::String::trimSpaces()
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
void sad::String::removeSpaces()
{
	this->removeAllOccurences(sad::String(" "));
}

sad::String & sad::String::removeRange(long beg,long rlen)
{
	long k;
	for (k=0;k<rlen;k++) //Remove old variable definition
		 this->remove(beg);    //Remove
	return *this;
}
sad::String & sad::String::insert(const sad::String & o,long i)
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

sad::String  sad::String::number(int a,int radix)
{
   char buffer[40];
   if (radix==8) {sprintf(buffer,"%o",a);return sad::String(buffer);}
   if (radix==10) {sprintf(buffer,"%d",a);return sad::String(buffer);}
   if (radix==16) {sprintf(buffer,"%X",a);return sad::String(buffer);}
   return sad::String();
}
bool  sad::String::queryPointer(const sad::String & str, long * addr)
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
int sad::String::toInt(const sad::String & str)
{
    int result=0;
    sscanf(str.data(),"%d",&result);
    return result;
}
float sad::String::toFloat(const sad::String & str)
{
    float result=0;
    sscanf(str.data(),"%f",&result);
    return result;
}

double sad::String::toDouble(const sad::String & str)
{
	double result = 0;
	sscanf(str.data(), "%lf", &result);
	return result;
}

sad::String  sad::String::subString(long beg,long len) const
{
	return substr(beg, len);
}
sad::String  sad::String::getRightPart(long len)
{
 return subString(length()-len,len);
}
sad::String  sad::String::getLeftPart(long len)
{
 return subString(0,len);
}
long sad::String::getOccurences(const sad::String & sstr) const
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
long sad::String::getOccurence(const sad::String & sstr,long omax)
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
long sad::String::getLastOccurence(const sad::String & sstr)
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
void sad::String::removeOccurence(const sad::String & sstr,long omax)
{
   long pos=getOccurence(sstr,omax);
   if (pos!=-1) removeRange(pos,sstr.length());
}
void sad::String::removeAllOccurences(const sad::String & sstr)
{
   long pos=getOccurence(sstr,0);
   while (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   pos=getOccurence(sstr,0);
   }
}
void sad::String::removeLastOccurence(const sad::String & sstr)
{
   long pos=getLastOccurence(sstr);
   if (pos!=-1) removeRange(pos,sstr.length());
}
void sad::String::replaceOccurence(const sad::String & sstr,const sad::String & to,long omax)
{
   long pos=getOccurence(sstr,omax);
   if (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   insert(to,pos);
   }
}
void sad::String::replaceAllOccurences(const sad::String & sstr,const sad::String & to)
{
   long pos=getOccurence(sstr,0);
   while (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   insert(to,pos);
	   pos=getOccurence(sstr,0);
   }
}

void sad::String::replaceLastOccurence(const sad::String & sstr,const sad::String & to)
{
   long pos=getLastOccurence(sstr);
   if (pos!=-1)
   {
	   removeRange(pos,sstr.length());
	   insert(to,pos);
   }
}

bool sad::String::consistsOfWhitespaceCharacters() const
{
	bool result = true;
	for(int i = 0; i < this->size(); i++)
	{
		char c = (*this)[i];
		if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
		{
			result = false;
		}
	}
	return result;
}

void sad::String::trim()
{
	int length = 0;
	bool found = false;
	for(int  i = 0; (i < this->size()) && !found; i++)
	{
		char c = (*this)[i];
		if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
		{
			found = true;
			length = i;
		}
	}
	if (found)
	{
		this->erase(0, length);
	}
	else
	{		
		this->clear();
		return;
	}

	found = false;
	int pos = 0;
	for(int  i = this->size() - 1; (i > -1) && !found; i--)
	{
		char c = (*this)[i];
		if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
		{
			found = true;
			pos = i;
		}
	}
	if (found)
	{
		this->erase(this->begin() + pos + 1, this->end());
	}
}

bool sad::String::cmpchar(char c1,char c2) const //Return false if c1 is bigger than c2
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

sad::String sad::join(const sad::StringList list, const sad::String & sep)
{
	if (list.count() == 0)
		return sad::String();
	sad::String result = list[0];
	for(unsigned int i = 1; i < list.count(); i++)
	{
		result << sep << list[i];
	}
	return result;
}
