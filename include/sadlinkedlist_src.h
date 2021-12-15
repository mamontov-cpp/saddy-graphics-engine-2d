#include <math.h>
#include <malloc.h>
#include <cassert>
#include <stdio.h>


namespace sad
{

// ============================================= PUBLIC sad::LinkedList IMPLEMENTATION  =============================================
    
template<
    typename T
>
sad::LinkedList<T>::node::node(typename sad::LinkedList<T>::node* previous, typename LinkedList<T>::node* next, T* me)
{
  this->m_previous= previous;
  this->m_next    = next;
  this->m_me      = me;
}

template<
    typename T
>
sad::LinkedList<T>::node::~node()
{
    delete m_me;
    m_me = NULL;
}

template<
    typename T
>
sad::LinkedList<T>::LinkedList()
{
    m_firstnode = NULL;
    m_lastnode = NULL;
    m_currentnode = NULL;
    m_currentindex = 0;
    m_size = 0;
}

template<
    typename T
>
sad::LinkedList<T>::LinkedList(const sad::LinkedList<T> & h)
{
    m_firstnode = NULL;
    m_lastnode = NULL;
    m_currentnode = NULL;
    m_currentindex = 0;
    m_size = 0;;
    add(h);	
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::operator=(const sad::LinkedList<T> & h)
{	
    this->makeClear();
    this->add(h);

    return *this;
}

template<
    typename T
>
void sad::LinkedList<T>::clear()
{
    this->makeClear();
}

template<
    typename T
>
void sad::LinkedList<T>::makeClear()
{
    node * curr = m_firstnode;
    node * next;
    long   i = 0;
    if (m_size > 0 && m_firstnode!=0)
    {
        while (i < m_size)
        {
            assert(curr);
            next = curr->m_next;
            delete curr;
            curr = next;   
            ++i;
        }
    }
    m_firstnode = NULL;
    m_lastnode = NULL;
    m_currentnode = NULL;
    m_currentindex = 0;
    m_size = 0;
}

template<
    typename T
>
sad::LinkedList<T>::~LinkedList()
{   
    makeClear();
}

template<
    typename T
>
T * sad::LinkedList<T>::data()   const 
{
    return m_firstnode;
}

template<
    typename T
>
long sad::LinkedList<T>::count() const 
{
    return m_size;
}

template<
    typename T
>
bool sad::LinkedList<T>::empty() const 
{
    return m_size == 0;
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::addFront(const T& obj) 
{
    return insert(obj,0);
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::add(const T& obj)
{
    if (m_size == 0)
    {
        m_firstnode = new node(NULL, NULL, new T(obj));
        m_lastnode=m_firstnode;
        m_currentnode = m_firstnode;
        m_currentindex = 0;
        m_size = 1;
    }
    else
    {
        if (m_size==1)
        {
            m_lastnode = new node(m_firstnode, NULL, new T(obj));
            m_firstnode->m_next = m_lastnode;
            m_currentnode = m_lastnode;
            m_currentindex = 1;
            m_size=2;
        }
        else
        {
            node * oldlastnode=m_lastnode;
            m_lastnode = new node(oldlastnode, NULL, new T(obj));
            oldlastnode->m_next = m_lastnode;
            m_currentnode = m_lastnode;
            m_currentindex = m_size;
            ++m_size;
        }
    }

    return *this;
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::operator<<(const T& obj)
{
    return add(obj);
}

template<
    typename T
>
T&  sad::LinkedList<T>::operator[](long i)
{
    return *(  getNode(i)->m_me );
}

template<
    typename T
>
const T & sad::LinkedList<T>::operator[](long i) const
{
    return *( getNode(i)->m_me );
}

// ============================================= PRIVATE sad::LinkedList IMPLEMENTATION  =============================================

template<
    typename T
>
void* LinkedList<T>::jumpPrevious(typename LinkedList<T>::node* from, long jumps) const
{	
    long i;
    typename LinkedList<T>::node*  res = from;
    for (i = 0; i < jumps; i++)
        res = res->m_previous;

    return static_cast<void*>(res);
}

template<
    typename T
>
void* LinkedList<T>::jumpNext(typename LinkedList<T>::node* from, long jumps) const
{
    long i;
    typename LinkedList<T>::node* res = from;
    for (i = 0; i < jumps;i++)
        res = res->m_next;

    return static_cast<void*>(res);
}
template<
    typename T
>
typename LinkedList<T>::node *   LinkedList<T>::getNode(long i) const
{	
    if (i < 0 || i >= m_size) 
        return new node(NULL,NULL,new T());
    long fdist = i, ldist=m_size - 1 - i; //Distances from first nodes and last nodes
    long cdist;
    bool dir;
    /* dir detects a direction  of jump for current
       if (dir==true) i is next to current node else - previous
    */
    sad::LinkedList<T>* me = const_cast<sad::LinkedList<T>*>(this);
    void * res;
    if (i > m_currentindex)
    {
        dir = true;
        cdist = i - m_currentindex;
    }
    else
    {
        dir = false;
        cdist = m_currentindex - i;
    }

    if (fdist > ldist)            //If from first it will be gathered faster
    {
        if (cdist > fdist)        //If from current it will be gathered
        {
            if (dir)
            {
                res=jumpNext(m_currentnode, cdist);
            }
            else
            {
                res=jumpPrevious(m_currentnode, cdist);
            }
        }
        else
        {
            res=jumpNext(m_firstnode, fdist);
        }
    }
    else
    {                           //From last it will be gathered faster
        if (cdist > ldist)        //If from current it will be gathered
        {
            if (dir)
            {
                res=jumpNext(m_currentnode, cdist);
            }
            else
            {
                res=jumpPrevious(m_currentnode, cdist);
            }
        }
        else
        {
            res=jumpPrevious(m_lastnode, ldist);
        }
    }
    
    assert( res );
    assert( i < m_size );
    me->m_currentindex = i;
    me->m_currentnode = static_cast<node*>(res);

    return static_cast<node*>(res);
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::insert(const T& obj, long i)
{
    
    if (i >= m_size) 
        return add(obj);
    if (i <= 0)
    {
        node * oldfirstnode = m_firstnode;
        node * newfirstnode = new node(NULL, m_firstnode, new T(obj));
        oldfirstnode->m_previous = newfirstnode;
        m_firstnode = newfirstnode;
        m_currentindex = 0;
        m_currentnode = newfirstnode;
    }
    else
    {
        node * previous = static_cast<node *>(getNode(i-1));
        node * next = static_cast<node *>(getNode(i));
        node * newnode = new node(previous, next, new T(obj));
        previous->next = newnode;
        next->previous = newnode;

        m_currentindex = i;
        m_currentnode = newnode;
    }
    ++m_size;	
    return *this;
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::add(const sad::LinkedList<T>& o)
{
    for (long i = 0; i < o.count(); i++)
        add(o[i]);	
    return *this;
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::operator<<(const sad::LinkedList<T>& o)
{	
    return add(o);
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::operator>>(T& obj)
{	
    if (m_size == 0) 
    {
        obj = T(); 
        return *this;
    }
    if (m_size == 1)
    {
        obj = *(m_firstnode->m_me);
        makeClear();
    }
    else
    {
        node * oldlastnode = m_lastnode;
        node * newlastnode = static_cast<node*>(getNode(m_size - 2));

        obj = *(oldlastnode->m_me);
        delete oldlastnode;

        newlastnode->m_next = 0;
        m_lastnode = newlastnode;

        m_size--;
        m_currentnode = m_lastnode;
        m_currentindex = m_size - 1;
    }
    
    return *this;
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::removeAt(long i)
{	
    if (i < 0 || i >= m_size || m_size ==0) 
    {
        return *this;
    }
    if (m_size == 1)
    {
        makeClear();
    }
    else
    {
        if (i == m_size - 1)      //if last node is being removed
        {
            node * oldlastnode = m_lastnode;
            node * newlastnode = static_cast<node*>(getNode(m_size-2));

            delete oldlastnode;

            newlastnode->m_next = 0;
            m_lastnode = newlastnode;

            m_size--;
            m_currentnode = m_lastnode;
            m_currentindex = m_size-1;
            assert(m_currentindex < m_size);
        }
        else
        {
            if (i == 0)
            {
                node * oldfirstnode = m_firstnode;
                node * newfirstnode = static_cast<node*>(getNode(1));

                delete oldfirstnode;

                newfirstnode->m_previous = 0;
                m_firstnode = newfirstnode;

                m_size--;
                m_currentnode = m_firstnode;
                m_currentindex = 0;
            }
            else
            {
                node* previous = static_cast<node*>(getNode(i-1));				
                node* me = static_cast<node*>(getNode(i));
                node* next = static_cast<node*>(getNode(i+1));

                delete me;

                previous->m_next = next;
                next->m_previous = previous;

                m_size--;
                m_currentnode = next;
                m_currentindex = i;
                assert(m_currentindex < m_size);
            }
        }
    }
    
    return *this;
}
template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::remove(const T& obj)
{
    for (long i = 0; i < m_size; i++)
    {
        if ((*this)[i] == obj)
        {
            removeAt(i);
            i--;
        }
    }
   return *this;
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::removeFirst(const T& obj)
{
    
    for (long i = 0; i < m_size; i++)
    {
        if ((*this)[i] == obj)
        {
            return removeAt(i);
        }
    }	
   return *this;
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::removeLast(const T& obj)
{
    
    long i, fnd = -1;
    for (i = 0; i < m_size; i++)
    {
        if ((*this)[i] == obj)
        {
            fnd = i;
        }
    }
    if (fnd != -1) 
        removeAt(fnd);
    
    return *this;
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::removeRange(long imin, long imax)
{
    
    long i;
    for (i = imin; i <= imax; i++)
       removeAt(imin);
    
    return *this;
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::push_back(const T& obj)
{
    return (*this) << obj;
}

template<
    typename T
>
sad::LinkedList<T>& sad::LinkedList<T>::pop_back(T& obj)
{
    obj=(*this)[0];
    return this->removeAt(0);
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::push_front(const T& obj)
{
    return this->insert(obj,0);
}

template<
    typename T
>
sad::LinkedList<T> & sad::LinkedList<T>::pop_front(T& obj)
{
    return (*this) >> obj;
}

// =========================== sad::LinkedList::const_iterator implementation ===========================

#define DQ_NODE sad::LinkedList<T>::node
#define DQ_CIT  sad::LinkedList<T>::const_iterator

template<
    typename T
>
DQ_CIT::const_iterator(
    class sad::LinkedList<T> const* parent,
    typename DQ_NODE const* next,
    typename DQ_NODE const* me,
    typename DQ_NODE const* prev
)	
{
    m_parent = parent;
    m_next = next;
    m_me = me;
    m_prev = prev;
}

template<
    typename T
>
DQ_CIT::const_iterator() 
{
    make_invalid();
};

template<
    typename T
>
DQ_CIT::~const_iterator() 
{
    
}

template<
    typename T
>
inline void DQ_CIT::make_invalid() 
{
    m_parent = NULL; 
    m_me = NULL;
    m_prev = NULL;
    m_next = NULL;
}

template<
    typename T
>
inline bool DQ_CIT::works() const 
{ 
    return m_parent && (m_me || m_prev || m_next); 
}

template<
    typename T
>
inline bool DQ_CIT::dereferencable() const 
{ 
    return m_parent && m_me; 
}

template<
    typename T
>
inline bool DQ_CIT::hasPrevious() const 
{ 
    return m_parent && (m_prev || m_me->m_previous); 
}

template<
    typename T
>
inline bool DQ_CIT::hasNext() const 
{ 
    return m_parent && (m_next || m_me->m_next); 
}

template<
    typename T
>
DQ_CIT::const_iterator(const typename DQ_CIT& o)
{
    m_parent=o.m_parent;
    m_me=o.m_me;
    m_prev=o.m_prev;
    m_next=o.m_next;
}

template<
    typename T
>
typename DQ_CIT & DQ_CIT::operator=(const typename DQ_CIT& o)
{
    m_parent=o.m_parent;
    m_me=o.m_me;
    m_prev=o.m_prev;
    m_next=o.m_next;
    return *this;
}

template<
    typename T
>
bool DQ_CIT::operator==(const typename DQ_CIT&  o) const
{ 
    return m_parent == o.m_parent 
        && m_me == o.m_me 
        && m_prev==o.m_prev 
        && m_next==o.m_next; 
}

template<
    typename T
>
bool DQ_CIT::operator!=(const typename DQ_CIT&  o) const
{ 
    return m_parent != o.m_parent 
        || m_me != o.m_me 
        || m_prev != o.m_prev 
        || m_next != o.m_next; 
}

template<
    typename T
>
const T & DQ_CIT::operator*() const
{
    assert( dereferencable() );
    return *(m_me->m_me);
}

template<
    typename T
>
typename DQ_CIT DQ_CIT::operator++(int)
{
    assert( hasNext() || (m_me != NULL && (m_me->m_next == NULL || m_next == NULL)) );
    const_iterator tmp = *this;
    //Set new position
    m_prev = m_me;
    m_me = m_next;
    if (m_me) 
        m_next = m_me->m_next; 
    else 
        m_next = NULL;
    return tmp;
}

template<
    typename T
>
typename DQ_CIT & DQ_CIT::operator++()
{
    assert( hasNext() || (m_me != NULL && (m_me->m_next == NULL || m_next == NULL)) );
    //Set new position
    m_prev = m_me;
    m_me = m_next;
    if (m_me) 
        m_next = m_me->m_next; 
    else 
        m_next = NULL;

    return *this;
}
template<
    typename T
>
typename DQ_CIT DQ_CIT::operator--(int)
{
    assert( hasPrevious() );
    const_iterator tmp = *this;

    //Set new position
    m_next = m_me;
    m_me = m_prev;
    if (m_me) 
        m_prev = m_me->m_previous; 
    else 
        m_prev = NULL;

    return tmp;
}

template<
    typename T
>
typename DQ_CIT & DQ_CIT::operator--()
{
    assert( hasPrevious() );

    //Set new position
    m_next = m_me;
    m_me = m_prev;
    if (m_me) 
        m_prev = m_me->m_previous; 
    else 
        m_prev = NULL;

    return *this;
}

template<
    typename T
>
typename DQ_CIT sad::LinkedList<T>::const_begin() const
{
    return const_iterator(this, (m_firstnode)? m_firstnode->m_next : NULL, m_firstnode, NULL);
}

template<
    typename T
>
typename DQ_CIT LinkedList<T>::const_end() const
{
    return const_iterator(this, NULL, NULL, m_lastnode);
}

#undef DQ_CIT


// =========================== sad::LinkedList::iterator implementation ===========================

#define DQ_IT  sad::LinkedList<T>::iterator

template<
    typename T
>
DQ_IT::iterator(
    class sad::LinkedList<T> * parent,
    typename DQ_NODE  * next,
    typename DQ_NODE  * me,
    typename DQ_NODE  * prev
)
{
    m_parent = parent;
    m_next = next;
    m_me = me;
    m_prev = prev;
}

template<
    typename T
>
DQ_IT::iterator() 
{
    make_invalid();
}

template<
    typename T
>
DQ_IT::~iterator() 
{
    
}

template<
    typename T
>
inline void DQ_IT::make_invalid() 
{
    m_parent = NULL; 
    m_me = NULL;
    m_prev = NULL;
    m_next = NULL;
}

template<
    typename T
>
inline bool DQ_IT::works() const 
{
    return m_parent && (m_me || m_prev || m_next); 
}

template<
    typename T
>
inline bool DQ_IT::dereferencable() const 
{
    return m_parent && m_me; 
}

template<
    typename T
>
inline bool DQ_IT::hasPrevious() const 
{
    return m_parent && (m_prev || m_me->m_previous); 
}

template<
    typename T
>
inline bool DQ_IT::hasNext() const 
{
    return m_parent && (m_next || m_me->m_next); 
}

template<
    typename T
>
DQ_IT::iterator(const typename DQ_IT&  o)
{
    m_parent = o.m_parent;
    m_me = o.m_me;
    m_prev = o.m_prev;
    m_next = o.m_next;
}

template<
    typename T
>
typename DQ_IT & DQ_IT::operator=(const typename DQ_IT& o)
{
    m_parent = o.m_parent;
    m_me = o.m_me;
    m_prev = o.m_prev;
    m_next = o.m_next;
    return *this;
}

template<
    typename T
>
bool DQ_IT::operator==(const typename DQ_IT&  o) const
{
    return m_parent == o.m_parent 
        && m_me == o.m_me 
        && m_prev == o.m_prev 
        && m_next == o.m_next; 
}

template<
    typename T
>
bool DQ_IT::operator!=(const typename DQ_IT&  o) const
{
    return m_parent != o.m_parent 
        || m_me != o.m_me 
        || m_prev != o.m_prev 
        || m_next != o.m_next; 
}

template<
    typename T
>
T& DQ_IT::operator*()
{
    assert( dereferencable() );
    return *(m_me->m_me);
}

template<
    typename T
>
typename DQ_IT DQ_IT::operator++(int)
{
    assert( hasNext() || (m_me != NULL && (m_me->m_next == NULL || m_next == NULL)) );
    iterator tmp = *this;
    //Set new position
    m_prev = m_me;
    m_me = (m_me)? m_me->m_next : m_next;
    if (m_me) 
        m_next = m_me->m_next; 
    else 
        m_next = NULL;

    return tmp;
}

template<
    typename T
>
typename DQ_IT & DQ_IT::operator++()
{
    assert( hasNext() || (m_me != NULL && (m_me->m_next == NULL || m_next == NULL)) );
    //Set new position
    m_prev = m_me;
    m_me = (m_me)? m_me->m_next : m_next;
    if (m_me) 
        m_next = m_me->m_next; 
    else 
        m_next = NULL;

    return *this;
}

template<
    typename T
>
typename DQ_IT DQ_IT::operator--(int)
{
    assert( hasPrevious() );
    iterator tmp = *this;

    //Set new position
    m_next = m_me;
    m_me = (m_me)? m_me->m_previous : m_prev;
    if (m_me) 
        m_prev = m_me->m_previous; 
    else 
        m_prev = NULL;

    return tmp;
}

template<
    typename T
>
typename DQ_IT & DQ_IT::operator--()
{
    assert( hasPrevious() );

    //Set new position
    m_next = m_me;
    m_me = (m_me)? m_me->m_previous : m_prev;
    if (m_me) 
        m_prev = m_me->m_previous; 
    else 
        m_prev = NULL;

    return *this;
}

template<
    typename T
>
typename DQ_IT sad::LinkedList<T>::begin()
{
    return iterator(this, (m_firstnode)? m_firstnode->m_next : NULL, m_firstnode, NULL);
}

template<
    typename T
>
typename DQ_IT sad::LinkedList<T>::end()
{
    return iterator(this, NULL, NULL, m_lastnode);
}

// ============================================= iterator related methods  =============================================

template<
    typename T
>
void  sad::LinkedList<T>::insert(typename DQ_IT& it, const T& val)
{
    if (it != end())
    {
        assert( it.dereferencable() );

        ++m_size;
        node * inserted = new node(it.m_me->m_previous, it.m_me, new T(val));
        if (it.m_me == m_firstnode) 
        {
            m_firstnode = inserted;
        }
        if (it.m_me == m_currentnode) 
        { 
            ++m_currentindex;
        }

        if (it.m_me->m_previous) 
        {
            it.m_me->m_previous->m_next = inserted;
        }
        it.m_me->m_previous = inserted;
        it.m_me = inserted;
        it.m_prev = inserted->m_previous;
        it.m_next = inserted->m_next;
    }
    else
    {
        add(val);
        it = iterator( this, NULL, m_lastnode, m_lastnode->m_previous);
    }
}

template<
    typename T
>
void  sad::LinkedList<T>::remove(typename DQ_IT& it)
{
    //Remove item
    if (it.m_me == m_firstnode)  
    {
        m_firstnode = (it.m_me)? it.m_me->next : it.m_next;
    }
    if (it.m_me == m_lastnode)   
    {
        m_lastnode = (it.m_me)? it.m_me->previous : it.m_prev;
    }
    if (it.m_me == m_currentnode) 
    {
        m_currentnode = m_firstnode; 
        m_currentindex = 0;
    }
    --m_size;
    node * pr = it.m_me->m_previous;
    node * nn = it.m_me->m_next;
    if (it.m_me->m_previous) 
        it.m_me->m_previous->m_next = it.m_me->m_next;
    if (it.m_me->m_next) 
        it.m_me->m_next->m_previous = it.m_me->m_previous;

    delete it.m_me;

    //Rebuild iterator
    if (it.m_prev) 
        it.m_me = pr;
    else  
        it.m_me = nn;
    if (it.m_me)
    {
        it.m_prev = it.m_me->m_previous;
        it.m_next = it.m_me->m_next;
    }
    else 
        it.make_invalid();
}

template<
    typename T
>
typename DQ_IT & DQ_IT::insert(const T& val)
{
    assert( m_parent );
    m_parent->insert( *this,val );
    return *this;
}

template<
    typename T
>
typename DQ_IT & DQ_IT::erase()
{
    assert( dereferencable() );
    m_parent->remove( *this );
    return *this;
}

#undef DQ_IT
#undef DQ_NODE
#undef CHECK

}
