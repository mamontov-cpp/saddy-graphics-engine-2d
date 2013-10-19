
#define TDEF template<typename Key,typename T>
#pragma once

namespace sad
{
 unsigned long basicHash(const unsigned char * name)
 {
	 unsigned long h=0,g=235;
	 while (*name)
	 {
		 h=(h<<4)+*name++;
		 g=h & 0xF0000000;
		 if (g) h^=g>>24;
		 h&=~g;
	 }
	 return h;
 }
 unsigned long basicHash(const unsigned char * name,unsigned long len)
 {
	 unsigned long h=0;
	 unsigned long i=0;
	 while (i!=len)
	 {
		 h+=name[i];
		 ++i;
	 }
	 return h;
 }
 template<typename T>
 unsigned long getHash(const T & name,unsigned long size)
 {
  return HashFunction<T>::call(name,size);  
 }
 
template<typename T>
 unsigned long HashFunction<T>::call(const T & name,unsigned long size)
 {
  unsigned long ff=basicHash((unsigned char*)&name,sizeof(T));  
  return ff % size;
 }

 TDEF void Hash<Key,T>::init()
 {
	 m_count=0;
	 m_table_size=17;
	 for (unsigned int i=0;i<m_table_size;i++)
		 m_data<<slot();
 }
 TDEF void Hash<Key,T>::extend()
 {
	 sad::Vector<slot> m_tmp=m_data;
     m_data.clear();
	 m_table_size+=19;
	 for (unsigned int i=0;i<m_table_size;i++)
		 m_data<<slot();
	 for (unsigned int i=0;i<m_table_size;i++)
		 assert( m_data[i].correct() );
	 for (unsigned int i=0;i<m_tmp.count();i++)
	 {
		 slot & from=m_tmp[i];
         for(int j=0;j<from.count();j++)
         {
			 sad::Pair<Key,T> & it=from[j];
			 m_data[ getHash( (it).p1(),m_table_size)  ] << it;
		 }
	 }
	 for (unsigned int i=0;i<m_table_size;i++)
		 assert( m_data[i].correct() );
 }
 TDEF bool Hash<Key,T>::checkExtend()
 {
	 return m_count==m_table_size+1;
 }
 TDEF Hash<Key,T>::Hash()                       { init(); }
 TDEF Hash<Key,T>::~Hash()                      {         }
 TDEF Hash<Key,T>::Hash(const Hash<Key,T> & o)
 {
	 m_data=o.m_data;
	 m_count=o.m_count;
	 m_table_size=o.m_table_size;
 }
 TDEF Hash<Key,T> & Hash<Key,T>::operator=(const Hash<Key,T> & o)
 {
	 m_data=o.m_data;
	 m_count=o.m_count;
	 m_table_size=o.m_table_size;
	 return *this;
 }
 TDEF void Hash<Key,T>::clear()
 {
	 m_data.clear();
	 init();
 }
 TDEF unsigned long Hash<Key,T>::count() const { return m_count; }
 TDEF unsigned long Hash<Key,T>::tableSize() const { return m_table_size; }
 TDEF void Hash<Key,T>::insert(const Key & k, const T & v)
 {
    unsigned long ind=getHash(k,m_table_size);
    typename slot::iterator it=m_data[ind].begin();
    bool     foundflag=false; // Whether we are found
	slot & dl=m_data[ind];
   for(int i=0;(i<dl.count()) && !foundflag;i++)
	{
	  if (dl[i].p1()==k)
	  {
	    dl[i].set2(v);
        foundflag=true;
	  }
	  ++it;
	}
	if (!foundflag)
	{
	  m_data[ind]<<(sad::Pair<Key,T>(k,v));
	  ++m_count;
	  if (checkExtend()) extend();
	}
 }
 TDEF const T & Hash<Key,T>::operator[](const Key & k) const
 {
   unsigned long ind=getHash(k,m_table_size);
   const  slot & dl=m_data[ind];
   for(int i=0;i<dl.count();i++)
   {
	  if (dl[i].p1()==k)
	  {
	    return dl[i].p2();
	  }
   }
   return *(new T());
 }
 TDEF  bool Hash<Key,T>::contains(const Key &k) const
 {
  unsigned long ind=getHash(k,m_table_size);
  const slot & dl=m_data[ind];
  for(int i=0;i<dl.count();i++)
  {
	  if (dl[i].p1()==k)
	  {
	    return true;
	  }
  }
  return false;
 }
 TDEF  T & Hash<Key,T>::operator[](const Key & k)
 {
   unsigned long ind=getHash(k,m_table_size);
   slot & dl=m_data[ind];
   for(int i=0;i<dl.count();i++)
   {
	  if (dl[i].p1()==k)
	  {
	    return *(const_cast<T*>( &(dl[i].p2()) ));
	  }
   }
   return *(new T());
 }
 TDEF void Hash<Key,T>::remove(const Key & k)
 {
   unsigned long ind=getHash(k,m_table_size);
   slot & dl=m_data[ind];
   bool notfound=true;
   for (int i=0;(i<dl.count()) && (notfound);i++)
   {
	   if (dl[i].p1()==k)
	   {
		   dl.removeAt(i);
		   notfound=false;
	   }
   }
   if (!notfound) --m_count;
 }
 TDEF Hash<Key,T>::const_iterator::const_iterator(
	                                               Hash<Key,T> * parent, 
	                                               unsigned long slot, 
												    const typename Hash<Key,T>::slot::const_iterator & it
												  )
 {
	 m_parent=parent;
     m_slotposition=slot;
	 m_it=it;
 }
 TDEF Hash<Key,T>::const_iterator::const_iterator()
 {
   m_parent=NULL;
   m_slotposition=0;
 }
 TDEF Hash<Key,T>::const_iterator::~const_iterator()
 {
   m_parent=NULL;
   m_slotposition=0;
 }
 TDEF Hash<Key,T>::const_iterator::const_iterator(const typename Hash<Key,T>::const_iterator & o )
 {
   m_parent=o.m_parent;
   m_slotposition=o.m_slotposition;
   m_it=o.m_it;
 }
 TDEF typename Hash<Key,T>::const_iterator & Hash<Key,T>::const_iterator::operator=( const typename Hash<Key,T>::const_iterator & o )
 {
   m_parent=o.m_parent;
   m_slotposition=o.m_slotposition;
   m_it=o.m_it;
   return *this;
 } 
 TDEF const Key& Hash<Key,T>::const_iterator::key()
 {
   if (m_it.dereferencable()) return (*m_it).p1();
   return *(new Key());
 }
 TDEF const T& Hash<Key,T>::const_iterator::value()
 {
   if (m_it.dereferencable()) return (*m_it).p2();
   return *(new T());
 }
 TDEF bool Hash<Key,T>::const_iterator::operator==( const typename Hash<Key,T>::const_iterator & o) const
 {
	 return  m_it==o.m_it && m_parent==o.m_parent && m_slotposition==o.m_slotposition;
 }
 TDEF bool Hash<Key,T>::const_iterator::operator!=( const typename Hash<Key,T>::const_iterator & o) const
 {
	 return  !(*this==o);
 }
 TDEF void Hash<Key,T>::const_iterator::goNext()
 {
	 if (!m_parent) return ;
	 if (*this!=m_parent->const_end())
	 {
		  if (m_slotposition<m_parent->tableSize())
		  {
			 if (m_it.hasNext())
			 {
				 ++m_it;
			 }
			 else
			 {
				 //Handle if there is one element in deque
				 ++m_slotposition;

				 while (m_slotposition<m_parent->tableSize() && m_parent->m_data[m_slotposition].count()==0)
                       ++m_slotposition;
			     if (m_slotposition!=m_parent->tableSize())
					 m_it= m_parent->m_data[m_slotposition].const_begin();
				 else
					 (*this)=m_parent->const_end();
			 }
		  }
		  else (*this)=m_parent->const_end();
	 }
 }
 TDEF void Hash<Key,T>::const_iterator::goPrevious()
 {
	 if (!m_parent) return ;
	 if (*this!=m_parent->const_begin())
	 {
	    if (*this==m_parent->const_end())
		{
			 --m_slotposition;
			 while ((m_slotposition!=0) && (m_parent->m_data[m_slotposition].count()==0))
			     --m_slotposition;
			 if (m_parent->m_data[m_slotposition].count()!=0)
		     {
			   m_it=m_parent->m_data[m_slotposition].const_end();
			   --m_it;
		     }
		     else
		     {
			    *this=m_parent->const_begin();
		     }
			 return;
		}
		if (!m_it.hasPrevious())
		{
		 --m_slotposition;
		 while ((m_slotposition!=0) && (m_parent->m_data[m_slotposition].count()==0))
			--m_slotposition;
		 if (m_parent->m_data[m_slotposition].count()!=0)
		 {
			 m_it=m_parent->m_data[m_slotposition].const_end();
			 --m_it;
		 }
		 else
		 {
			 *this=m_parent->const_begin();
		 }
		}
		else
		{
			--m_it;
		}
	 }	 
 }
 TDEF typename Hash<Key,T>::const_iterator & Hash<Key,T>::const_iterator::operator++()
 {
	 goNext();
	 return *this;
 }
 TDEF typename Hash<Key,T>::const_iterator & Hash<Key,T>::const_iterator::operator--()
 {
	 goPrevious();
	 return *this;
 }
 TDEF typename Hash<Key,T>::const_iterator  Hash<Key,T>::const_iterator::operator++(int)
 {
	 typename Hash<Key,T>::const_iterator it=*this;
 	 goNext();
	 return it;
 }
 TDEF typename Hash<Key,T>::const_iterator  Hash<Key,T>::const_iterator::operator--(int)
 {
	 typename Hash<Key,T>::const_iterator it=*this;
	 goPrevious();
	 return it;
 }
 TDEF typename Hash<Key,T>::const_iterator Hash<Key,T>::const_begin() const
 {
     unsigned long f=0;
	 while ( (f<m_table_size) && m_data[f].count()==0)
		 ++f;
	 if (f==m_table_size) return const_end();
     //int cc=m_data[f].count();
	 return const_iterator(const_cast<Hash<Key,T>* >(this),f,m_data[f].const_begin());
 }
 TDEF typename Hash<Key,T>::const_iterator Hash<Key,T>::const_end() const
 {
	 return  const_iterator(const_cast<Hash<Key,T>* >(this),m_table_size,typename slot::const_iterator());
 }
 



 TDEF Hash<Key,T>::iterator::iterator(
	                                               Hash<Key,T> * parent, 
	                                               unsigned long slot, 
												    const typename Hash<Key,T>::slot::iterator & it
												  )
 {
	 m_parent=parent;
     m_slotposition=slot;
	 m_it=it;
 }
 TDEF Hash<Key,T>::iterator::iterator()
 {
   m_parent=NULL;
   m_slotposition=0;
 }
 TDEF Hash<Key,T>::iterator::~iterator()
 {
   m_parent=NULL;
   m_slotposition=0;
 }
 TDEF Hash<Key,T>::iterator::iterator( const typename Hash<Key,T>::iterator & o )
 {
   m_parent=o.m_parent;
   m_slotposition=o.m_slotposition;
   m_it=o.m_it;
 }
 TDEF typename Hash<Key,T>::iterator & Hash<Key,T>::iterator::operator=( const typename Hash<Key,T>::iterator & o )
 {
   m_parent=o.m_parent;
   m_slotposition=o.m_slotposition;
   m_it=o.m_it;
   return *this;
 } 
 TDEF const Key& Hash<Key,T>::iterator::key()
 {
   if (m_it.dereferencable()) return (*m_it).p1();
   return *(new Key());
 }
 TDEF T& Hash<Key,T>::iterator::value()
 {
   if (m_it.dereferencable()) return *(const_cast<T*>( &( (*m_it).p2() )));
   return *(new T());
 }
 TDEF bool Hash<Key,T>::iterator::operator==( const typename Hash<Key,T>::iterator & o) const
 {
	 return  m_it==o.m_it && m_parent==o.m_parent && m_slotposition==o.m_slotposition;
 }
 TDEF bool Hash<Key,T>::iterator::operator!=(const  typename Hash<Key,T>::iterator & o) const
 {
	 return  !(*this==o);
 }
 TDEF void Hash<Key,T>::iterator::goNext()
 {
	 if (!m_parent) return ;
	 if (*this!=m_parent->end())
	 {
		  if (m_slotposition<m_parent->tableSize())
		  {
			 if (m_it.hasNext())
			 {
				 ++m_it;
			 }
			 else
			 {
				 ++m_slotposition;
				 while (m_slotposition<m_parent->tableSize() && m_parent->m_data[m_slotposition].count()==0)
                       ++m_slotposition;
				 if (m_slotposition<m_parent->tableSize())
					 m_it= m_parent->m_data[m_slotposition].begin();
				 else
					 (*this)=m_parent->end();
			 }
		  }
		  else (*this)=m_parent->end();
	 }
 }
 TDEF void Hash<Key,T>::iterator::goPrevious()
 {
	 if (!m_parent) return ;
	 if (*this!=m_parent->begin())
	 {
	    if (*this==m_parent->end())
		{
			 --m_slotposition;
			 while ((m_slotposition!=0) && (m_parent->m_data[m_slotposition].count()==0))
			     --m_slotposition;
			 if (m_parent->m_data[m_slotposition].count()!=0)
		     {
			   m_it=m_parent->m_data[m_slotposition].end();
			   --m_it;
		     }
		     else
		     {
			    *this=m_parent->begin();
		     }
			 return;
		}
		if (!m_it.hasPrevious())
		{
		 --m_slotposition;
		 while ((m_slotposition!=0) && (m_parent->m_data[m_slotposition].count()==0))
			--m_slotposition;
		 if (m_parent->m_data[m_slotposition].count()!=0)
		 {
			 m_it=m_parent->m_data[m_slotposition].end();
			 --m_it;
		 }
		 else
		 {
			 *this=m_parent->begin();
		 }
		}
		else
		{
			--m_it;
		}
	 }	 
 }
 TDEF typename Hash<Key,T>::iterator & Hash<Key,T>::iterator::operator++()
 {
	 goNext();
	 return *this;
 }
 TDEF typename Hash<Key,T>::iterator & Hash<Key,T>::iterator::operator--()
 {
	 goPrevious();
	 return *this;
 }
 TDEF typename Hash<Key,T>::iterator  Hash<Key,T>::iterator::operator++(int)
 {
	 typename Hash<Key,T>::iterator it=*this;
 	 goNext();
	 return it;
 }
 TDEF typename Hash<Key,T>::iterator  Hash<Key,T>::iterator::operator--(int)
 {
	 typename Hash<Key,T>::iterator it=*this;
	 goPrevious();
	 return it;
 }
 TDEF typename Hash<Key,T>::iterator Hash<Key,T>::begin()
 {
     unsigned long f=0;
	 while ( (f<m_table_size) && m_data[f].count()==0)
		 ++f;
	 if (f==m_table_size) return end();
	 return iterator(this,f,m_data[f].begin());
 }
 TDEF typename Hash<Key,T>::iterator Hash<Key,T>::end()
 {
	 return iterator(this,m_table_size,typename slot::iterator());
 }
 
}

#undef TDEF
