#include "../include/sadstring.h"
#include <stdlib.h>
#include <string.h>
#include <algorithm>


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

//------------------------------------------

sad::String & sad::String::operator<<(char c)
{
  this->push_back(c);
  return *this;
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

sad::StringList sad::String::split(
    const char * delimiters,
    sad::String::SplitBehaviour b
) const
{
  sad::StringList result;
  sad::String buffer;
  for(unsigned long i = 0; i < this->length(); i++)
  {
    char cur = (*this)[i];
    if (strrchr(delimiters,cur) != nullptr)
    {
        if (buffer.empty() == false || b == sad::String::KEEP_EMPTY_PARTS)
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
sad::StringList sad::String::split(
    char delimiter,
    sad::String::SplitBehaviour b
) const
{
    char s[2]={delimiter, 0x0};
    return split(s, b);
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
void  sad::String::addExtension(const sad::String & new_extension)
{
    (*this)<<'.'<<new_extension;
}
void  sad::String::changeExtension(const sad::String & new_extension)
{
 removeExtension();
 addExtension(new_extension);
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
   size_t startpos = this->find_first_not_of(" ");
   if(std::string::npos != startpos )
   {
       *this = this->substr( startpos );
   }
   else
   {
       this->clear();
       return;
   }
   size_t endpos = this->find_last_not_of(" ");
   if( std::string::npos != endpos )
   {
        *this = this->substr( 0, endpos+1 );
   }
}
void sad::String::removeSpaces()
{
    this->removeAllOccurrences(sad::String(" "));
}

sad::String & sad::String::removeRange(long beg,long right_length)
{
    long k;
    for (k=0;k<right_length;k++) 
         this->remove(beg);   
    return *this;
}
sad::String & sad::String::insert(const sad::String & o,long i)
{
    if (i < 0) return *this;
    if (static_cast<unsigned long>(i) > this->length()) 
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
bool  sad::String::queryPointer(const sad::String & str, long * address)
{
   long cwt=-32768;
   if (str.empty())
   {
       return 0;
   }
   sscanf(str.data(),"%lX",&cwt);
   *address=0;
   if (cwt>=0)
   {
       *address=cwt; return 1;
   }
   return 0;
}
int sad::String::toInt(const sad::String & str)
{
    int result=0;
    sscanf(str.data(),"%d",&result);
    return result;
}

bool sad::String::parseUInt(const char* begin, const char* end, unsigned int* r)
{    
    bool ok  = true;
    unsigned int rr = 0;
    unsigned int p = 1;
    while((end >= begin) && ok)
    {
        char c = *end;
        if (c >= '0' && c <= '9')
        {
            rr += (c - '0') * p;
        }
        else
        {
            ok = false;
        }
        --end;
        p *= 10;
    }
    if (ok)
    {
        *r = rr;
    }
    return ok;
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
    // Do not throw out of range
    if (beg > static_cast<long>(length()))
    {
        return "";
    }
    return substr(beg, len);
}
sad::String  sad::String::getRightPart(long len) const
{
    return subString(length()-len,len);
}
sad::String  sad::String::getLeftPart(long len) const
{
    return subString(0,len);
}
long sad::String::getOccurrences(const sad::String & sub_string) const
{
    long count = 0;
    size_t pos = this->find(sub_string);
    while(pos != std::string::npos)
    {
        ++count;
        pos = this->find(sub_string, pos + sub_string.length());
    }
    return count;
}
long sad::String::getOccurrence(const sad::String & sub_string,long o_max) const
{
  long result = -1;
  long count = 0;
  size_t pos = this->find(sub_string);
  while(pos != std::string::npos)
  {
    if (count == o_max)
        result = pos;
    ++count;
    pos = this->find(sub_string, pos + sub_string.length());
  }
  return result;
}
long sad::String::getLastOccurrence(const sad::String & sub_string) const
{
  long result = -1;
  size_t pos = this->find(sub_string);
  while(pos != std::string::npos)
  {
    result = pos;
    pos = this->find(sub_string, pos + sub_string.length());
  }
  return result;
}
void sad::String::removeOccurrence(const sad::String & sub_string,long o_max)
{
   long pos=getOccurrence(sub_string,o_max);
   if (pos!=-1) removeRange(pos,sub_string.length());
}

void sad::String::removeAllOccurrences(const sad::String & sub_string)
{
   long pos=getOccurrence(sub_string,0);
   while (pos!=-1)
   {
       removeRange(pos,sub_string.length());
       pos=getOccurrence(sub_string,0);
   }
}
void sad::String::removeLastOccurrence(const sad::String & sub_string)
{
   long pos=getLastOccurrence(sub_string);
   if (pos!=-1) removeRange(pos,sub_string.length());
}
void sad::String::replaceOccurrence(const sad::String & sub_string,const sad::String & to,long o_max)
{
   long pos=getOccurrence(sub_string,o_max);
   if (pos!=-1)
   {
       removeRange(pos,sub_string.length());
       insert(to,pos);
   }
}
void sad::String::replaceAllOccurrences(const sad::String & sub_string,const sad::String & to)
{
   long pos=getOccurrence(sub_string,0);
   while (pos!=-1)
   {
       removeRange(pos,sub_string.length());
       insert(to,pos);
       pos=getOccurrence(sub_string,0);
   }
}

void sad::String::replaceLastOccurrence(const sad::String & sub_string,const sad::String & to)
{
   long pos=getLastOccurrence(sub_string);
   if (pos!=-1)
   {
       removeRange(pos,sub_string.length());
       insert(to,pos);
   }
}

bool sad::String::consistsOfWhitespaceCharacters() const
{
    bool result = true;
    for(size_t i = 0; i < this->size(); i++)
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
    if (!trimLeft())
    {
        trimRight();
    }
}

bool sad::String::trimLeft()
{
    bool is_whitespace = false;
    int length = 0;
    bool found = false;
    for(size_t  i = 0; (i < this->size()) && !found; i++)
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
        is_whitespace = true;
    }
    return is_whitespace;
}

bool sad::String::trimRight()
{
    bool is_whitespace = false;
    bool found = false;
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
    else
    {
        this->clear();
        is_whitespace = true;
    }
    return is_whitespace;
}

bool sad::String::startsWith(const char* s, size_t sz) const
{
    if (!s)
    {
        return false;
    }
    if (this->size() < sz)
    {
        return false;
    }
    return memcmp(this->c_str(), s, sz) == 0;
}

bool sad::String::endsWith(const char* s, size_t sz) const
{
    if (!s)
    {
        return false;
    }
    if (this->size() < sz)
    {
        return false;
    }
    return memcmp(this->c_str() + this->length() - sz, s, sz) == 0;
}

void sad::String::toUpper()
{
    std::transform(this->begin(), this->end(), this->begin(), ::toupper);
}

void sad::String::toLower()
{
    std::transform(this->begin(), this->end(), this->begin(), ::tolower);
}

bool sad::String::isNumeric(bool allowMinus) const
{
    if (this->length() == 0)
    {
        return false;
    }

    char first_char = (*this)[0];
    if ((first_char != '-') || (allowMinus == false))
    {
        if (first_char < '0' || first_char > '9')
        {
            return false;
        }
    }
    for (size_t i = 1; i < this->length(); i++)
    {
        if ((*this)[i] < '0' || (*this)[i] > '9')
        {
            return false;
        }
    }

    return true;
}

sad::String sad::join(const sad::StringList& list, const sad::String & sep)
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
