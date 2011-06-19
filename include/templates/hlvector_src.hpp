#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#ifdef _CRTMEMORYTEST
	#include <crtdbg.h>
#endif

#define HPTRNULL 0
#define HLVECLEANPOOL( POOL ) (POOL)->p=HPTRNULL; (POOL)->sz=0;

template<class T> void * operator new(size_t sz, struct hst::vector_pool<T> *  _pool, unsigned long  allocated,unsigned long where)
{
    sz=0;
    if (allocated==_pool->sz)
    {pool_growup(_pool);}
    return (void*)((_pool->p)+where);
}
template<typename T>
void  operator delete(void * pp, hst::vector_pool<T> *  _pool, unsigned long allocated,unsigned long where)
{
	pp=NULL;
	allocated=0;
	where=0;
    pool_destroy(_pool);
}


namespace hst
{

template<class T> void pool_growup(vector_pool<T> * _pool)
{
  assert(_pool->sz!=0xFFFFFFFF);       //Overflow checking
  if (_pool->sz!=0)
  {
      _pool->sz=(_pool->sz)<<1;
      T * newptr=(T*)realloc(_pool->p,(size_t)((_pool->sz)*sizeof(T)));
      //printf("New pool size: %ld Pointer: %p %p Size allocate: %u\n",_pool->sz,_pool->p,newptr,(size_t)((_pool->sz)*sizeof(T)));
      if (newptr!=HPTRNULL)            //If realloc not failed
          _pool->p=newptr;
      else                             //If failed
          free(_pool->p);
      assert(newptr!=HPTRNULL);       //Doing assert
  }
  else
  {
      _pool->p=(T*)malloc(2*sizeof(T));
      _pool->sz=2;
	  assert(_pool->p!=HPTRNULL);
  }
}
template<class T> void pool_shrink(vector_pool<T> * _pool,unsigned long newsz)
{
 if (newsz!=0)
 {
     //printf("New size: %ld  %ld %d \n", newsz,(newsz) & (newsz-1),newsz!=1);
     if (( ((newsz) & (newsz-1))==0) && (newsz!=1))  //If power of 2
     {
         _pool->p=(T*)realloc(_pool->p,newsz*sizeof(T));
         _pool->sz=newsz;
     }
 }
 else
 {
   pool_destroy(_pool);
 }
}
template<class T> void pool_destroy(vector_pool<T> * _pool)
{
    if (_pool->p!=HPTRNULL)
        free(_pool->p);
    HLVECLEANPOOL( _pool );
}

template<class T> void vector<T>::makeClear()
{
    if (sz>0)
    {
        unsigned long i;
        for (i=sz-1;i!=0;i--)
		{
			//printf("Calling destructor: %p \n",p+i);
           (m_pool.p+i)->~T();
		}
		m_pool.p->~T();
		m_pool.sz=0;
		sz=0;
		//printf("Cleaning: %p %ld\n",p,this->count());
    }
    pool_destroy(&m_pool);
}
template<class T> void vector<T>::clear()
{
	makeClear();
}
template<class T> vector<T>::vector()
{
  sz=0;
  HLVECLEANPOOL( &m_pool );
}
template<class T> vector<T>::~vector()
{
    makeClear();
}

template<class T> vector<T>::vector(const vector & h)
{
  HLVECLEANPOOL(&m_pool);
  sz=0;
  for (unsigned long i=0;i<h.count();i++)
  {
      this->add(h[i]);
  }
}

template<class T> vector<T> & vector<T>::operator=(const vector<T>&  h)        //Присваивание
{
  if (this->m_pool.p==h.m_pool.p) return *this;
  makeClear();
  sz=0;
  for (unsigned long i=0;i<h.count();i++)
  {
      this->add(h[i]);
  }
  return *this;
}

template<class T> inline unsigned long vector<T>::count() const              {return sz;}
template<class T> inline T*   vector<T>::data()  const              {return (m_pool.p);}
template<class T> inline T&   vector<T>::operator[](unsigned long i)         {return (m_pool.p)[i];}
template<class T> inline const T &   vector<T>::operator[](unsigned long i) const   {return (m_pool.p)[i];}
template<class T> void vector<T>::dbg_ass(const char * cp)         {printf("Size: %ld, Pool size: %ld Pointer %p\n",sz,m_pool.sz,m_pool.p);for (unsigned long i=0;i<sz;i++) printf(cp,(m_pool.p)[i]);printf("%s","\n");}

template<class T> vector<T> & vector<T>::operator<<(const T& obj)
{
    //printf("New params: %p %ld %ld %ld\n" , m_pool.p,m_pool.sz,sz,sz);
	new (&m_pool,sz,sz ) T(obj);
    ++sz;
    return *this;
}

template<class T> vector<T> & vector<T>::operator>>(T& obj)
{
    obj=(*this)[sz-1];
    removeAt(sz-1);
    return *this;
}

template<class T> vector<T>& vector<T>::removeAt(unsigned long i)
{
    if (i>=sz) return *this;
    for (unsigned long j=i+1;j<sz;j++)
    {
        (*this)[j-1]=(*this)[j];
    }
    --sz;
    ((m_pool.p)+sz)->~T();
    pool_shrink(&m_pool,sz);
    return *this;
}

template<class T> vector<T> & vector<T>::add(const T & obj)
{
    return *this<<obj;
}
template<class T> vector<T> & vector<T>::addFront(const T & obj)
{
    return insert(obj,0);
}

template<class T> vector<T> & vector<T>::insert(const T &obj,unsigned long i)
{
  if (i>=sz) return add(obj);
  add(T());
  //puts("Insert: ");
  //this->dbg_ass("%d ");
  for (unsigned long j=sz-1;j!=i;j--)
  {
      //printf("%ld %ld",j,j-1);
      (*this)[j]=(*this)[j-1];
  }
  (*this)[i]=obj;
  return *this;
}

template<class T> vector<T> & vector<T>::remove(const T& obj)
{
    unsigned long i;
    for (i=0;i<sz;i++)
    {
        if ((*this)[i]==obj)
        {
            removeAt(i);
            i--;
        }
    }
   return *this;
}
template<class T> vector<T> & vector<T>::removeFirst(const T& obj)
{
    unsigned long i;
    for (i=0;i<sz;i++)
    {
        if ((*this)[i]==obj)
        {
            return removeAt(i);
        }
    }
   return *this;
}
template<class T> vector<T> & vector<T>::removeLast(const T& obj)
{
    unsigned long i,fnd=0;
    bool flag=false;
    for (i=0;i<sz;i++)
    {
        if ((*this)[i]==obj)
        {
            fnd=i;
            flag=true;
        }
    }
    if (flag) removeAt(fnd);
   return *this;
}
template<class T> vector<T> & vector<T>::removeRange(unsigned long imin,unsigned long imax)
{
    unsigned long i;
    for (i=imin;i<=imax;i++)
       removeAt(imin);
    return *this;
}
template<class T> void  vector<T>::yield(void (*fptr)(T & obj))       //Применение функции на список
{
    unsigned long i;
    for (i=0;i<sz;i++)
     (*fptr)((*this)[i]);
}
template<class T> bool vector<T>::operator==(const vector<T> & o)
{
  unsigned long i;
  if (count()!=o.count()) return 0;
  for (i=0;i<count();i++)
	  if ((*this)[i]!=o[i])
		  return 0;
  return 1;
}
template<class T> bool vector<T>::operator!=(const vector<T> & o)
{
	return !(*this==o);
}

}
