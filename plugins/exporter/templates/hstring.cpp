#include "stdafx.h"
#include <stdlib.h>
#include "hstringlist.h"
#include <string.h>

#ifdef _CRTMEMORYTEST
	#include <crtdbg.h>
#endif

#ifdef H_USE_LOW_LEVEL_IO
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>
#ifdef __UBUNTU
     #include <unistd.h>
#else
     #include <io.h>
#endif
#endif
#ifndef _MSC_VER
	#define _itoa itoa
#endif

using namespace hst;
string::string()
{
	p=NULL;
	len=0;
}
string::string(const char* _p)
{
  if (_p!=NULL)
  {
	len=(long)strlen(_p);
    p=(char*)malloc((len+1)*sizeof(char));
	strcpy(p,_p);
  }
  else
  {p=NULL;len=0;puts("Empty string copying catched");}
}
string::string(const char* _p,long len)
{
 if (_p!=NULL)
 {
	this->len=len;
    p=(char*)malloc((len+1)*sizeof(char));
	memcpy(p,_p,len*sizeof(char));
	p[len]=0;
	this->removeOddNullsAtEnd();
 }
 else
 {p=NULL;len=0;}
}

string::string(const string & o)
{
 if (o.p!=NULL)
 {
  len=o.len;
  p=(char*)malloc((len+1)*sizeof(char));
  strcpy(p,o.p);
 }
 else
 {p=NULL;len=0;}
}
string::~string()
{
	this->clear();
}
void string::clear()
{
    if (p!=NULL)
	{
		free(p);
	}
	p=NULL;
	len=0;
}
void string::print() const
{
	if (p!=NULL)
	{
		long i;
		for (i=0;i<len;i++)
			printf("%c",p[i]);
		puts("");
	}
}
//----Getters-------------------------------
long   string::length() const {return this->len;}
char * string::data()   const {return this->p;}
bool   string::empty()  const {return (len==0)?true:false;}
//-
void   string::__clean() {this->p=NULL;this->len=0;}
//------------------------------------------
string & string::operator=(const string & o)
{
  if (p!=o.p)
  {
	  if (p!=NULL && len>0) free(p);
      if (o.p!=NULL)
	  {
	   len=o.len;
       p=(char*)malloc((len+1)*sizeof(char));
       strcpy(p,o.p);
	  }
	  else
	   {p=NULL;len=0;}
  }

  return *this;
}
string & string::operator<<(char c)
{
  if (p==NULL)
  {
     p=(char*)malloc(2*sizeof(char));
	 p[0]=c;
	 p[1]=0x00000;
	 len=1;
  }
  else
  {
	  len++;
	  p=(char*)realloc(p,(len+1)*sizeof(char));
	  p[len-1]=c;
      p[len]=0x0000;
  }
  return *this;
}
char & string::operator[](long i)
{
	if ((i<0) || (i>len))
	{
		printf("Out of index assertion. Can't access char %ld",i);
		return *(new char); //Sounds interesting
	}
	else return p[i];
}
char  string::operator[](long i) const
{
	if ((i<0) || (i>len))
	{
		printf("Out of index assertion. Can't access char %ld",i);
		return 0; 
	}
	else return p[i];
}
bool string::operator==(const string & o) const
{
	return (*this==o.data());
}
bool string::operator!=(const string & o) const
{
	return (*this!=o.data());
}
bool string::operator>(const string &o) const
{
	long minlen=(this->length()<o.length())?this->length():o.length();
	long i;
	for (i=0;i<minlen;i++)
	{
	  if ((*this)[i]!=o[i])
		if (cmpchar(this->p[i],o[i]))
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
	long j,jmax=len-1;
	if ((i>-1) && (i<=len))
	{
      for (j=i;j<jmax;j++)
		  p[j]=p[j+1];
	  p[jmax]=0x0000;
	  --len;
	  if (len!=0)
	  p=(char*)realloc(p,(len+1)*sizeof(char));
	  else
	  {
		  free(p);
		  p=NULL;
	  }
	}
	else {printf("Removing of non-existing item %ld catched\n",i);}
}

//Split a string to a list, using specified delimiter
string & string::operator<<(const string & o)
{
	long i;
	for (i=0;i<o.length();i++)
		(*this)<<(o[i]);
	return *this;
}
stringlist string::split(const char * delimiters) const
{
  long    i;           //Counter
  string tmp=*this;   //Copy self to work with it simply. We can replace some delimiter by NULL and work with it
  char * p=tmp.data();          //
  stringlist result;
  p=strtok(p,delimiters);
  while( p )
  {
      result<<hString(p);
	  p=strtok(NULL,delimiters);
  }
  //Scan list
  for (i=0;i<result.length();i++)
	{
		if (result[i].length()==0)
		{
			result.remove(i);
			i--;
		}
		else
		{
		  if (result[i][0]==0x00000)
		  {
			result.remove(i);
			i--;
		  }
		}
	}
	return result;
}
stringlist string::split(char delimiter) const
{
	long    i;           //Counter
	string tmp=*this;   //Copy self to work with it simply. We can replace some delimiter by NULL and work with it
    char * p=tmp.data();          //
	stringlist result;
    for (i=0;i<tmp.len;i++)
		if (tmp[i]==delimiter)
		{
			tmp[i]=0x00000;  //Insert space
			result<<p;
			p=tmp.data()+i+1;
		}
	result<<string(p);              //Adding unused space
	for (i=0;i<result.length();i++)
	{
		if (result[i].length()==0)
		{
			result.remove(i);
			i--;
		}
		else
		{
		  if (result[i][0]==0x00000)
		  {
			result.remove(i);
			i--;
		  }
		}
	}
	return result;
}
void string::readLine(FILE** pf)
{
	char c=0x00;
	this->clear();
	while((c=fgetc(*pf))=='\r' || (c=='\n') && (!feof(*pf))) {};         //Skipping strings
    if ((c!='\r') && (c!='\n') && (c!=EOF)) (*this)<<c;                 //Appending first character
	while((c=fgetc(*pf))!='\r' && (c!='\n') && (!feof(*pf))) (*this)<<c; //Append character
}
bool string::operator==(const char * o) const
{
	if (this->data()==NULL)
	{
		if (o==NULL) return true; else return false;
	}
	if (o==NULL) return false;
	if (strcmp(o,this->data())==0) return true; else return false;
}
bool string::operator!=(const char * o) const
{
 return !(*this==o);
}
string  string::getLastCharacters(long i) const
{
	if (i<0) return *(new string());
	if (i>=this->len) return *(new string(*this));
	string result;
	long j;
	for (j=len-i;j<len;j++)
		result<<(*this)[j];
	return result;
}
string string::getExtension() const
{
   if (len==0) return string();
   char * r=strrchr(p,'.');
   if (r==NULL) return string();
   return string(r+1);
}
void string::removeExtension()
{
	if (!(this->getExtension().empty()))
	{
      char * r=strrchr(p,'.');
	  long index=r-p; //Index of dot
	  this->removeRange(index,len-index);
	}
}
void  string::addExtension(const string & newext)
{
	(*this)<<'.'<<newext;
}
void  string::changeExtension(const string & newext)
{
 if (len==0)
 {
   (*this)<<'.'<<newext;
 }
 else
 {
	 if (this->getExtension().empty()) //if no extension
	 {
		(*this)<<'.'<<newext;
	 }
	 else
	 {
		char * r=strrchr(p,'.');
		long index=(r+1)-p; //Index of dot
		this->removeRange(index,len-index);
		(*this)<<newext;
	 }
 }
}
string  string::operator+(const string & o) const
{
   long i;
   string result(*this);
   for (i=0;i<o.length();i++)
	   result<<o[i];
   return result;
}
void string::removeOddNullsAtEnd()
{
   while ( (this->length()!=0) && (*this)[this->length()-1]==0 )
           this->remove(this->length()-1);

}
string & string::insert(char c,long i)
{
	if (i<0) i=0;
	if (i>=len)  return (*this)<<c;
	len++;
	long j;
	p=(char*)realloc(p,(len+1)*sizeof(char));
	for (j=len-1;j>i;j--)
		p[j]=p[j-1];
	p[i]=c;
    p[len]=0x0000;
	return *this;
}
void string::getLastOccurence(const char * beg, const char * end,long * begi,long * rlen)
{
	*begi=-1;*rlen=-1;
	long i=0,  j=0;
	long beglen=strlen(beg);
	long endlen=strlen(end);
	long lbeg;
	long llen;
	bool flag=false;
	for (i=0;i<=this->len-beglen;i++)
	{
		if (!strncmp(p+i,beg,beglen))
		{
			lbeg=i;
			flag=false;
			for (j=i+beglen;((j<=this->len-endlen) && (!flag));j++)
			{
				if (!strncmp(p+j,end,endlen))
				{
					flag=true;
					llen=j-i+endlen;
					*rlen=llen;
					*begi=lbeg;
				}
			}
		}
	}
}
void string::removeLastOccurence(const char * beg, const char * end)
{
	long i=-1,rlen=-1;
	this->getLastOccurence(beg,end,&i,&rlen);
    if (i!=-1)
	{
		removeRange(i,rlen);
	}
}
void string::replaceLastOccurence(const char * beg, const char * end,const string & to)
{
	long i=-1,rlen=-1;
	this->getLastOccurence(beg,end,&i,&rlen);
    if (i!=-1)
	{
		removeRange(i,rlen);
		insert(to,i);
	}
}
long string::getOccurences(const char * beg, const char * end)
{
   string oldstr=*this,newstr=*this;
   long result=0;
   do
   {
	   oldstr=newstr;
	   newstr.removeLastOccurence(beg,end);
       if (newstr!=oldstr) ++result;
   } while(newstr!=oldstr);
   return result;
}
void string::removeAllOccurences(const char * beg,const char * end)
{
	string oldstr=*this;
	do
	{
       oldstr=*this;
	   removeLastOccurence(beg,end);
	} while (*this!=oldstr);
}
void string::trimSpaces()
{
   long i;
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
void string::oc_Replace_Callback(const char *beg, const char *end, long (*repfunc)(string &, long, long))
{
	long i=0,  j=0;
	long beglen=strlen(beg);
	long endlen=strlen(end);
	long lbeg;
	long llen;
	bool flag=false;
	for (i=0;i<=this->len-beglen;i++)
	{
		if (!strncmp(p+i,beg,beglen))
		{
			lbeg=i;
			flag=false;
			for (j=i+beglen;((j<=this->len-endlen) && (!flag));j++)
			{
				if (!strncmp(p+j,end,endlen))
				{
					flag=true;
					llen=j-i+endlen;
					i=(*repfunc)(*this,lbeg,llen);
				}
			}
		}
	}
}
int string::oc_strictReplace_Callback(const char *beg, const char *end, long (*repfunc)(string &, long, long))
{
	long i=0;
	long beglen=strlen(beg);
	long endlen=strlen(end);
	long llen;
	long searchbflag=0;  //flag, indicates that first subseq found
	long searcheflag=0;  //flag, indicates that second subseq found
    long begpos,endpos;  //Indicates the position of current pos;
    long maxlen=(beglen>endlen)?beglen:endlen;
    long scanlen=this->len-maxlen+1;
    for (i=0;i<scanlen;i++)
    {
        if (!strncmp(p+i,beg,beglen) && !searchbflag)
        {
            ++searchbflag;
            begpos=i;
        }
        if (!strncmp(p+i,end,endlen))
        {
            ++searcheflag;
            if (searchbflag!=0 && searchbflag==searcheflag )     //If this is correct position
            {
				endpos=i;
                llen=endpos-begpos+endlen;
                i=(*repfunc)(*this,begpos,llen);
                searchbflag=0;
                searcheflag=0;
            }
        }
    }
    return (searchbflag==searcheflag);
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
	long j;
    for (j=o.length()-1;j>-1;j--)
		insert(o[j],i);
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
class stringlist string::splitByLength(int maxlen) const
{
   stringlist words=this->split(' ');
   stringlist result;
   string tmp;
   long i=0;
   while (i<words.length())
   {
	   tmp=words[i];
	   ++i;
	   if (i<words.length())
	   {
	    while ((i<words.length()) && ((tmp.length()+1+words[i].length())<=maxlen)  )
	    {
          tmp<<' '<<words[i];
		  ++i;
	    }
	   }
       result<<tmp;
   }
   return result;
}

string  string::subString(long beg,long len) const
{
	string result;
	long i;
	if (beg<0)
	{
		len-=beg;
		beg=0;
	}
	for (i=beg; (i<length()) && (i-beg<len);i++)
	{
		result<<(*this)[i];
	}
	return result;
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
	long result=0;
	long i,maxi=length()-sstr.length();
	if (sstr.length()>length()) return result;
    for (i=0;i<=maxi;i++)
           if (!strncmp(this->p+i,sstr.data(),sstr.length()))
		    {
			 ++result;
	         i+=sstr.length()-1;
		    }
	return result;
}
long string::getOccurence(const string & sstr,long omax)
{
 long result=-1,resulti=-1;
 long i,maxi=length()-sstr.length();
 if (sstr.length()>length()) return result;
    for (i=0;i<=maxi;i++)
           if (!strncmp(this->p+i,sstr.data(),sstr.length()))
		    {
			 ++resulti;
             if (resulti==omax)
				 result=i;
	         i+=sstr.length()-1;
		    }
  return result;
}
long string::getLastOccurence(const string & sstr)
{
 long result=-1;
 long i,maxi=length()-sstr.length();
 if (sstr.length()>length()) return result;
    for (i=0;i<=maxi;i++)
           if (!strncmp(this->p+i,sstr.data(),sstr.length()))
		    {
			 result=i;
	         i+=sstr.length()-1;
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
#ifdef H_USE_LOW_LEVEL_IO

void string::readLine_lowLevel(int fd)
{
  char c=0x00;
  int sz=sizeof(char);
  int _rr=1;
  this->clear();
  _rr=_read(fd,(void *)&c,sz) ;
  while(   (c=='\r') || (c=='\n') && (_rr)) {_rr=_read(fd,(void *)&c,sz) ;};         //Skipping strings
  if ((c!='\r') && (c!='\n') && (c!=EOF)) (*this)<<c;                 //Appending first character
  _rr=_read(fd,(void *)&c,sz);
  while(   (c!='\r') && (c!='\n') && (_rr)) {(*this)<<c;_rr=_read(fd,(void *)&c,sz);} //Append character
  this->removeOddNullsAtEnd();
}

void string::writeToFile_lowLevel(const string & fn)
{
  int	fd = _open( fn.data(), _O_WRONLY | _O_TRUNC | _O_TEXT | _O_CREAT,_S_IREAD | _S_IWRITE);  //Open file
  if( fd != -1 )												  //If succeeded
  {
      _write(fd,(void*)this->data(),this->len*sizeof(char));
      _close( fd );
  }

}
void string::readFile_lowLevel(const string & fn)
{
   char c=0;
   this->clear();
   int	fd = _open( fn.data(), _O_RDONLY  | _O_TEXT,  _S_IREAD | _S_IWRITE );  //Open file
   if (fd!=-1)
   {
      while(_read(fd,(void*)&c,sizeof(char))!=0)
		  (*this)<<c;
	  _close(fd);
   }
}
#endif
