#include "stdafx.h"
#include "hstringlist.h"

using namespace hst;

stringlist::stringlist()
{
	p=NULL;
	len=0;
}
stringlist::stringlist(const stringlist & o)
{
 long i,imax=o.len;
 if (o.p!=NULL)
 {
  len=imax;
  p=(string *)malloc(len*sizeof(string ));
  for (i=0;i<imax;i++)
  {
	  p[i].__clean();
	  p[i]=*(o.p+i);
  }
 }
 else
 {p=NULL;len=0;puts("Empty stringlist copying catched");}
}
stringlist::~stringlist()
{
	for (long i=0;i<this->length();i++)
	     p[i].clear();
	delete p;
	p=NULL;
	len=0;
}
stringlist & stringlist::operator=(const stringlist & o)
{
  long i;
  if (p!=o.p)
  {
	  if (len>0)
	  {
	     for (i=0;i<this->length();i++)
			  p[i].clear();
		 delete p;
	  }
      if (o.p!=NULL)
	  {
	   len=o.len;
       p=(string*)malloc(len*sizeof(string));
       for (i=0;i<len;i++)
       {
	    p[i].__clean();
	    p[i]=o.p[i];
       }
	  }
	  else
	   {p=NULL;len=0;puts("Empty stringlist copying catched");}
  }
  return *this;
}
string & stringlist::operator[](long i)
{
   if (i<0 || i>=len)
   {
	   printf("Out of index assertion. Value: %ld\n",i);
	   return *(new string); //Some interesting
   }
   else return p[i];
}
const string & stringlist::operator[](long i) const
{
   if (i<0 || i>=len)
   {
	   printf("Out of index assertion. Value: %ld\n",i);
	   return *(new string); //Some interesting
   }
   else return p[i];
}
stringlist & stringlist::operator<<(const string & h)
{
  if (p==NULL)
  {
     p=(string*)malloc(sizeof(string));
	 p[0].__clean();
	 p[0]=h;
	 len=1;
  }
  else
  {
	  len++;
	  p=(string*)realloc(p,len*sizeof(string));
	  p[len-1].__clean();
	  p[len-1]=h;
  }
  return *this;
}
void stringlist::print()
{
	long i;
	puts("List:");
	for (i=0;i<len;i++)
		p[i].print();
}
void stringlist::remove(long i)
{
  long j,jmax=len-1;
  if ((i>-1) && (i<=len))
  {
      for (j=i;j<jmax;j++)
		  p[j]=p[j+1];
	  --len;
	  if (len!=0)
	  p=(string*)realloc(p,(len)*sizeof(string));
	  else
	  {
		  free(p);
		  p=NULL;
	  }
  }
  else {printf("Removing of non-existing item %ld catched\n",i);}
}
stringlist & stringlist::operator<<(char * p)
{
	return (*this)<<string(p);
}
//----Getters-------------------------------
long   stringlist::length() const {return len;}
string * stringlist::data() const  {return p;}
//-
void   stringlist::__clean() {this->p=NULL;this->len=0;}
