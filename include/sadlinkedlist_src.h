#include <math.h>
#include <malloc.h>
#include <cassert>
#include <stdio.h>


namespace sad
{
template<class T> LinkedList<T>::node::node(typename LinkedList<T>::node *previous, typename LinkedList<T>::node *next, T *me)
{
  this->previous= previous;
  this->next    = next;
  this->me      = me;
}
template<class T> LinkedList<T>::node::~node()
{
	delete me;
	me=NULL;
}

template<class T> LinkedList<T>::LinkedList()
{
	firstnode=NULL;
	lastnode=NULL;
	currentnode=NULL;
	currentindex=0;
	sz=0;
	
}
template<class T> LinkedList<T>::LinkedList(const LinkedList & h)
{
	firstnode=NULL;
	lastnode=NULL;
	currentnode=NULL;
	currentindex=0;
	sz=0;
	add(h);
	
}
template<class T> LinkedList<T> & LinkedList<T>::operator=(const LinkedList & h)
{
	
	this->makeClear();
	this->add(h);
	
	return *this;
}
template<class T> void LinkedList<T>::clear()
{
  this->makeClear();
}
template<class T> void LinkedList<T>::makeClear()
{
 
 node * curr=firstnode;
 node * next;
 long   i=0;
 if (sz>0 && firstnode!=0)
 {
  while (i<sz)
  {
    assert(curr);
    next=curr->next;
    delete curr;
	curr=next;    //Moving next
	++i;
  }
 }
 firstnode=NULL;
 lastnode=NULL;
 currentnode=NULL;
 currentindex=0;
 sz=0;
}
template<class T> LinkedList<T>::~LinkedList()
{
   
   makeClear();
}

template<class T> T * LinkedList<T>::data()   const {return firstnode;}
template<class T> long LinkedList<T>::count() const {return sz;}
template<class T> bool LinkedList<T>::empty() const {return count()==0;}
template<class T> LinkedList<T> & LinkedList<T>::addFront(const T & obj) {return insert(obj,0);}
template<class T> LinkedList<T> & LinkedList<T>::add(const T & obj)
{
 
 if (sz==0)
 {
    firstnode=new node(NULL, NULL, new T(obj));
	lastnode=firstnode;
	currentnode=firstnode;
	currentindex=0;
	sz=1;
 }
 else
 {
  if (sz==1)
  {
	lastnode=new node(firstnode,NULL,new T(obj));
	firstnode->next=lastnode;
    currentnode=lastnode;
	currentindex=1;
	sz=2;
  }
  else
  {
	node * oldlastnode=lastnode;
    lastnode=new node(oldlastnode,NULL,new T(obj));
    oldlastnode->next=lastnode;
	currentnode=lastnode;
	currentindex=sz;
	++sz;
  }
 }
 
 return *this;
}

template<class T> LinkedList<T> & LinkedList<T>::operator<<(const T & obj)
{
	
	return add(obj);
}
template<class T> void LinkedList<T>::dump(const char * cp) const
{
 node * curr=firstnode;
 long   i=0;
 printf("Inner properties: first node: %p last node: %p current node: %p current index: %ld\n",firstnode,lastnode,currentnode,currentindex);

  while (i<sz)
  {
	printf("Index: %ld:\n",i);
	printf("            Previous: %p\n",curr->previous);
	printf("            Me:%p, Value:",curr->me); printf(cp,*(curr->me));
    printf("\n            Next: %p\n",curr->next);
	curr=curr->next;    //Moving next
	++i;
  }
}
template<class T> T&   LinkedList<T>::operator[](long i)
{
	return *(  getNode(i)->me );
}
template<class T> const T &  LinkedList<T>::operator[](long i) const
{
	return *( getNode(i)->me );
}

template<class T> void * LinkedList<T>::jumpPrevious(class LinkedList<T>::node * from,long jumps) const
{
	
    long i;
    node * res=from;
    for (i=0;i<jumps;i++)
        res=res->previous;
	
    return (void*)res;
}
template<class T> void * LinkedList<T>::jumpNext(class LinkedList<T>::node * from,long jumps) const
{
	
    long i;
    node * res=from;
    for (i=0;i<jumps;i++)
        res=res->next;
	
    return (void*)res;
}
template<class T> typename LinkedList<T>::node *   LinkedList<T>::getNode(long i) const
{
	
	if (i<0 || i>=sz) return new node(NULL,NULL,new T());
	long fdist=i,ldist=sz-1-i; //Distances from first nodes and last nodes
	long cdist;
	bool dir;
    /* dir detects a direction  of jump for current
       if (dir==true) i is next to current node else - previous
    */
    LinkedList * me=const_cast<LinkedList *>(this);
    void * res;
    if (i>currentindex)
    {dir=true;cdist=i-currentindex;}
    else
    {dir=false;cdist=currentindex-i;}

    if (fdist>ldist)            //If from first it will be gathered faster
    {
        if (cdist>fdist)        //If from current it will be gathered
        {
            if (dir)
            {res=jumpNext(currentnode,cdist);}
            else
            {res=jumpPrevious(currentnode,cdist);}
        }
        else
        {res=jumpNext(firstnode,fdist);}
    }
    else
    {                           //From last it will be gathered faster
        if (cdist>ldist)        //If from current it will be gathered
        {
            if (dir)
            {res=jumpNext(currentnode,cdist);}
            else
            {res=jumpPrevious(currentnode,cdist);}
        }
        else
        {res=jumpPrevious(lastnode,ldist);}
    }
	assert(res);
	assert(i<sz);
    me->currentindex=i;
    me->currentnode=(node*)res;
	
    return (node*)res;
}
template<class T> LinkedList<T> & LinkedList<T>::insert(const T &obj,long i)
{
	
	if (i>=sz) return add(obj);
	if (i<=0)
	{
		node * oldfirstnode=firstnode;
		node * newfirstnode=new node(NULL, firstnode, new T(obj));
		oldfirstnode->previous=newfirstnode;
		firstnode=newfirstnode;
		currentindex=0;
	    currentnode=newfirstnode;
	}
	else
	{
		node * previous=(node *)getNode(i-1);
		node * next=(node *)getNode(i);
		node * newnode=new node(previous, next, new T(obj));
		previous->next=newnode;
		next->previous=newnode;

		currentindex=i;
        currentnode=newnode;
	}
	++sz;
	
	return *this;
}
template<class T> LinkedList<T> & LinkedList<T>::add(const LinkedList & o)
{
	long i;
	for (i=0;i<o.count();i++)
		add(o[i]);
	
	return *this;
}
template<class T> LinkedList<T> & LinkedList<T>::operator<<(const LinkedList & o)
{
	
	return add(o);
}
template<class T> LinkedList<T> & LinkedList<T>::operator>>(T & obj)
{
	
	if (sz==0) {obj=T(); return *this;}
    if (sz==1)
	{
		obj=*(firstnode->me);
		makeClear();
	}
	else
	{
        node * oldlastnode=lastnode;
		node * newlastnode=(node*) getNode(sz-2);

		obj=*(oldlastnode->me);

		oldlastnode->~node();
		free(oldlastnode);

		newlastnode->next=0;
		lastnode=newlastnode;

		sz--;
		currentnode=lastnode;
		currentindex=sz-1;
	}
	
	return *this;
}
template<class T> LinkedList<T> & LinkedList<T>::removeAt(long i)
{
	
	if (i<0 || i>=sz) {return *this;}
    if (sz==0)        {return *this;}
    if (sz==1)
	{
		makeClear();
	}
	else
	{
      if (i==sz-1)      //if remove last
	  {
		node * oldlastnode=lastnode;
		node * newlastnode=(node*) getNode(sz-2);

		oldlastnode->~node();
		free(oldlastnode);

		newlastnode->next=0;
		lastnode=newlastnode;

		sz--;
		currentnode=lastnode;
		currentindex=sz-1;
		assert(currentindex<sz);
	  }
	  else
	  {
		 if (i==0)
		 {
			node * oldfirstnode=firstnode;
			node * newfirstnode=(node*)getNode(1);

			oldfirstnode->~node();
			free(oldfirstnode);

			newfirstnode->previous=0;
			firstnode=newfirstnode;

			sz--;
			currentnode=firstnode;
			currentindex=0;
		 }
		 else
		 {
			 node * me =(node*)getNode(i);
			 node * previous=(node*)getNode(i-1);
			 node * next=(node*)getNode(i+1);

			 me->~node();
			 free(me);

			 previous->next=next;
			 next->previous=previous;

			 sz--;
			 currentnode=next;
			 currentindex=i;
			 assert(currentindex<sz);
		 }
	  }
	}
	
	return *this;
}
template<class T> LinkedList<T> & LinkedList<T>::remove(const T& obj)
{
	
    long i;
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
template<class T> LinkedList<T> & LinkedList<T>::removeFirst(const T& obj)
{
	
    long i;
    for (i=0;i<sz;i++)
    {
        if ((*this)[i]==obj)
        {
            return removeAt(i);
        }
    }
	
   return *this;
}
template<class T> LinkedList<T> & LinkedList<T>::removeLast(const T& obj)
{
	
    long i,fnd=-1;
    for (i=0;i<sz;i++)
    {
        if ((*this)[i]==obj)
        {
            fnd=i;
        }
    }
    if (fnd!=-1) removeAt(fnd);
	
   return *this;
}
template<class T> LinkedList<T> & LinkedList<T>::removeRange(long imin,long imax)
{
	
    long i;
    for (i=imin;i<=imax;i++)
       removeAt(imin);
	
    return *this;
}
template<class T> LinkedList<T> & LinkedList<T>::push_back(const T & obj)
{
	return (*this)<<obj;
}
template<class T> LinkedList<T> & LinkedList<T>::pop_back(T & obj)
{
	obj=(*this)[0];
	return this->removeAt(0);
}
template<class T> LinkedList<T> & LinkedList<T>::push_front(const T & obj)
{
	return this->insert(obj,0);
}
template<class T> LinkedList<T> & LinkedList<T>::pop_front(T & obj)
{
  return (*this)>>obj;
}

#define DQ_NODE LinkedList<T>::node
#define DQ_CIT  LinkedList<T>::const_iterator

template<class T> DQ_CIT::const_iterator(class LinkedList<T> const * parent,typename DQ_NODE const * next,typename DQ_NODE const * me,typename DQ_NODE const * prev)
{
    m_parent=parent;
    m_next=next;
    m_me=me;
    m_prev=prev;
}

template<class T> DQ_CIT::const_iterator() {broke();};
template<class T> DQ_CIT::~const_iterator() {}
template<class T> inline void DQ_CIT::broke() {m_parent=NULL; m_me=NULL;m_prev=NULL;m_next=NULL;}
template<class T> inline bool DQ_CIT::works() const { return m_parent && (m_me || m_prev || m_next); }
template<class T> inline bool DQ_CIT::dereferencable() const { return m_parent && m_me; }
template<class T> inline bool DQ_CIT::hasPrevious() const { return m_parent && (m_prev || m_me->previous); }
template<class T> inline bool DQ_CIT::hasNext() const { return m_parent && (m_next || m_me->next); }

template<class T> DQ_CIT::const_iterator(const typename DQ_CIT & o)
{
    m_parent=o.m_parent;
    m_me=o.m_me;
    m_prev=o.m_prev;
    m_next=o.m_next;
}
template<class T> typename DQ_CIT & DQ_CIT::operator=(const typename DQ_CIT & o)
{
    m_parent=o.m_parent;
    m_me=o.m_me;
    m_prev=o.m_prev;
    m_next=o.m_next;
    return *this;
}
template<class T> bool DQ_CIT::operator==(const typename DQ_CIT &  o) const
{ return m_parent==o.m_parent && m_me==o.m_me && m_prev==o.m_prev && m_next==o.m_next; }
template<class T> bool DQ_CIT::operator!=(const typename DQ_CIT &  o) const
{ return m_parent!=o.m_parent || m_me!=o.m_me || m_prev!=o.m_prev || m_next!=o.m_next; }

template<class T> const T & DQ_CIT::operator*() const
{
    assert( dereferencable() );
    return *(m_me->me);
}
template<class T> typename DQ_CIT DQ_CIT::operator++(int)
{
    assert( hasNext() || ((m_me->next==NULL ||m_next==NULL) && m_me!=NULL) );
    const_iterator tmp=*this;
    //Set new position
    m_prev=m_me;
    m_me=m_next;
    if (m_me) m_next=m_me->next; else m_next=NULL;

    return tmp;
}
template<class T> typename DQ_CIT & DQ_CIT::operator++()
{
    assert( hasNext() || ((m_me->next==NULL ||m_next==NULL) && m_me!=NULL) );
    //Set new position
    m_prev=m_me;
    m_me=m_next;
    if (m_me) m_next=m_me->next; else m_next=NULL;

    return *this;
}
template<class T> typename DQ_CIT DQ_CIT::operator--(int)
{
    assert( hasPrevious() );
    const_iterator tmp=*this;

    //Set new position
    m_next=m_me;
    m_me=m_prev;
    if (m_me) m_prev=m_me->previous; else m_prev=NULL;

    return tmp;
}
template<class T> typename DQ_CIT & DQ_CIT::operator--()
{
    assert( hasPrevious() );

    //Set new position
    m_next=m_me;
    m_me=m_prev;
    if (m_me) m_prev=m_me->previous; else m_prev=NULL;

    return *this;
}

template<class T> typename DQ_CIT LinkedList<T>::const_begin() const
{
    return const_iterator(this,(firstnode)?firstnode->next:NULL,firstnode,NULL);
}

template<class T> typename DQ_CIT LinkedList<T>::const_end() const
{
    return const_iterator(this,NULL,NULL,lastnode);
}

#undef DQ_CIT

#define DQ_IT  LinkedList<T>::iterator


template<class T> DQ_IT::iterator(class LinkedList<T>  * parent,typename DQ_NODE  * next,typename DQ_NODE  * me,typename DQ_NODE  * prev)
{
    m_parent=parent;
    m_next=next;
    m_me=me;
    m_prev=prev;
}

template<class T> DQ_IT::iterator() {broke();};
template<class T> DQ_IT::~iterator() {}
template<class T> inline void DQ_IT::broke() {m_parent=NULL; m_me=NULL;m_prev=NULL;m_next=NULL;}
template<class T> inline bool DQ_IT::works() const { return m_parent && (m_me || m_prev || m_next); }
template<class T> inline bool DQ_IT::dereferencable() const { return m_parent && m_me; }
template<class T> inline bool DQ_IT::hasPrevious() const { return m_parent && (m_prev || m_me->previous); }
template<class T> inline bool DQ_IT::hasNext() const { return m_parent && (m_next || m_me->next); }

template<class T> DQ_IT::iterator(const typename DQ_IT & o)
{
    m_parent=o.m_parent;
    m_me=o.m_me;
    m_prev=o.m_prev;
    m_next=o.m_next;
}
template<class T> typename DQ_IT & DQ_IT::operator=(const typename DQ_IT & o)
{
    m_parent=o.m_parent;
    m_me=o.m_me;
    m_prev=o.m_prev;
    m_next=o.m_next;
    return *this;
}
template<class T> bool DQ_IT::operator==(const typename DQ_IT &  o) const
{ return m_parent==o.m_parent && m_me==o.m_me && m_prev==o.m_prev && m_next==o.m_next; }
template<class T> bool DQ_IT::operator!=(const typename DQ_IT &  o) const
{ return m_parent!=o.m_parent || m_me!=o.m_me || m_prev!=o.m_prev || m_next!=o.m_next; }

template<class T> T & DQ_IT::operator*()
{
    assert( dereferencable() );
    return *(m_me->me);
}
template<class T> typename DQ_IT DQ_IT::operator++(int)
{
    assert( hasNext() || ((m_me->next==NULL ||m_next==NULL) && m_me!=NULL) );
    iterator tmp=*this;
    //Set new position
    m_prev=m_me;
    m_me=(m_me)?m_me->next:m_next;
    if (m_me) m_next=m_me->next; else m_next=NULL;

    return tmp;
}
template<class T> typename DQ_IT & DQ_IT::operator++()
{
    assert( hasNext() || ((m_me->next==NULL ||m_next==NULL) && m_me!=NULL) );
    //Set new position
    m_prev=m_me;
    m_me=(m_me)?m_me->next:m_next;
    if (m_me) m_next=m_me->next; else m_next=NULL;

    return *this;
}
template<class T> typename DQ_IT DQ_IT::operator--(int)
{
    assert( hasPrevious() );
    iterator tmp=*this;

    //Set new position
    m_next=m_me;
    m_me=(m_me)?m_me->previous:m_prev;
    if (m_me) m_prev=m_me->previous; else m_prev=NULL;

    return tmp;
}
template<class T> typename DQ_IT & DQ_IT::operator--()
{
    assert( hasPrevious() );

    //Set new position
    m_next=m_me;
    m_me=(m_me)?m_me->previous:m_prev;
    if (m_me) m_prev=m_me->previous; else m_prev=NULL;

    return *this;
}

template<class T> typename DQ_IT LinkedList<T>::begin()
{
    return iterator(this,(firstnode)?firstnode->next:NULL,firstnode,NULL);
}

template<class T> typename DQ_IT LinkedList<T>::end()
{
    return iterator(this,NULL,NULL,lastnode);
}
template<class T> void  LinkedList<T>::insert(typename DQ_IT & it, const T & val)
{
 if (it!=end())
 {
    assert( it.dereferencable() );

    ++sz;
    node * inserted=new node(it.m_me->previous,it.m_me,new T(val));
    if (it.m_me==firstnode) firstnode=inserted;
    if (it.m_me==currentnode) { ++currentindex;}

    if (it.m_me->previous) it.m_me->previous->next=inserted;
    it.m_me->previous=inserted;
    it.m_me=inserted;
    it.m_prev=inserted->previous;
    it.m_next=inserted->next;
 }
 else
 {
     add(val);
     it=iterator(this,NULL,lastnode,lastnode->previous);
 }
}

template<class T> void  LinkedList<T>::remove(typename DQ_IT & it)
{
  //Remove item
  if (it.m_me==firstnode)  {firstnode=(it.m_me)?it.m_me->next:it.m_next;}
  if (it.m_me==lastnode)   {lastnode=(it.m_me)?it.m_me->previous:it.m_prev;}
  if (it.m_me==currentnode) {currentnode=firstnode; currentindex=0;}
  --sz;
  node * pr=it.m_me->previous;
  node * nn=it.m_me->next;
  if (it.m_me->previous) it.m_me->previous->next=it.m_me->next;
  if (it.m_me->next) it.m_me->next->previous=it.m_me->previous;

  delete it.m_me;

  //Rebuild iterator
  if (it.m_prev) it.m_me=pr;
  else  it.m_me=nn;
  if (it.m_me)
  {
      it.m_prev=it.m_me->previous;
      it.m_next=it.m_me->next;
  }
  else it.broke();
}
template<class T> typename DQ_IT & DQ_IT::insert(const  T & val)
{
    assert( m_parent );
    m_parent->insert( *this,val );
    return *this;
}
template<class T> typename DQ_IT & DQ_IT::erase()
{
    assert( dereferencable() );
    m_parent->remove( *this );
    return *this;
}
#undef DQ_IT
#undef DQ_NODE
#undef CHECK
}
