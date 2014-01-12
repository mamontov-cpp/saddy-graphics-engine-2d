namespace sad
{
  #define TEMPLATE_DEF  template<typename Key, typename T>
  TEMPLATE_DEF  Ptrie<Key,T>::Ptrie() { m_root=NULL; }
  TEMPLATE_DEF  Ptrie<Key,T>::Ptrie( const Ptrie<Key,T> & o)
  {
      m_root=NULL;
      for (typename  Ptrie<Key,T>::const_iterator it=o.const_begin();it.dereferencable();it++)
         add(it.key(),it.value());
  }
  TEMPLATE_DEF  void Ptrie<Key,T>::clear() {makeClear();}
  TEMPLATE_DEF  Ptrie<Key,T> & Ptrie<Key,T>::operator=( const Ptrie<Key,T> & o)
  {
      makeClear();
      m_root=NULL;
      for (typename  Ptrie<Key,T>::const_iterator it=o.const_begin();it.dereferencable();it++)
         add(it.key(),it.value());
      return *this;
  }
  TEMPLATE_DEF  Ptrie<Key,T>::node::node(const typename sad::LinkedList< sad::Triplet<Key,T,void *> >::iterator & m_t)
  {
    m_val=m_t;
    m_children=NULL;
  }
  TEMPLATE_DEF Ptrie<Key,T>::node::node() { m_children=NULL; }

  TEMPLATE_DEF typename Ptrie<Key,T>::node *& Ptrie<Key,T>::node::getChildren(unsigned char byte)
  {
      if (!m_children) m_children=(node **)calloc(256,sizeof(node));
      return m_children[byte];
  }

  TEMPLATE_DEF Ptrie<Key,T>::node::~node()
  {
	  if (m_children) { for (int i=0;i<256;i++) if (m_children[i]) delete m_children[i]; free(m_children); }
  }
  TEMPLATE_DEF void Ptrie<Key,T>::makeClear()
  {
      m_list.clear();
	  if (m_root) { delete m_root;m_root=NULL; }
  }
  TEMPLATE_DEF  Ptrie<Key,T>::~Ptrie() { makeClear(); }
  TEMPLATE_DEF  Ptrie<Key,T> & Ptrie<Key,T>::add(const Key & key, const T & val)
  {
      if (!m_root) m_root=new  node();
      node * cur=m_root;
      unsigned char const * bytes=reinterpret_cast<unsigned char const *>(&key); //Byte representation of key
      size_t sz=sizeof(Key);
      size_t i=0;
      for (i=0;i<sz;i++)
      {
          node ** tmp=&((cur->getChildren(bytes[i])));
          //printf("Iteration: %d Byte: %d  &&: %p &: %p\n",i,bytes[i],tmp,*tmp);
          if (!(*tmp))
             *tmp=new node();
          cur=*tmp;
      }

      //printf("Started adding: \n");
      //Adding  to list and setting iterator
      if (!cur->m_val.works())
      {
       m_list<<triplet<Key,T,void*>(key,val,NULL);
       typename hst::deque< hst::triplet<Key,T, void *> >::iterator last=m_list.end(); --last;
       cur->m_val=last;
       (*last).set3((void*)&(cur->m_val));
      }
      else
      {
         (*(cur->m_val)).set2(val);
      }
      //printf("Setting iterator: \n");
      //Setting pointer to iterator
      return *this;
  }
  TEMPLATE_DEF T const * Ptrie<Key,T>::operator[](const Key & key) const
  {
      node * cur=jump(key);
      if (!cur) return NULL;
      if (!(cur->m_val.works())) return NULL;
      return &(  (*(cur->m_val)).p2() );
  }
  TEMPLATE_DEF T  * Ptrie<Key,T>::operator[](const Key & key)
  {
      node * cur=jump(key);
      //printf("Returned value []::cur : %p\n",cur);
      if (!cur) return NULL;
      if (!(cur->m_val.works())) return NULL;
      return &(  (*(cur->m_val))._2() );
  }
  TEMPLATE_DEF Ptrie<Key,T> & Ptrie<Key,T>::remove(const Key & key)
  {
     node * cur=jump(key);
     if (!cur) return *this;
     if (!(cur->m_val.works())) return *this;
     (cur->m_val.erase())=typename hst::deque<hst::triplet<Key,T,void *> >::iterator();
     return *this;
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::node * Ptrie<Key,T>::jump(const Key & key) const
  {
      node * cur=m_root;
      unsigned char const * bytes=reinterpret_cast<unsigned char const *>(&key); //Byte representation of key
      size_t sz=sizeof(Key);
      size_t i=0;
      for (i=0;(i<sz) && (cur);i++)
      {
          cur=cur->getChildren(bytes[i]);
      }
      return cur;
  }
  TEMPLATE_DEF inline long Ptrie<Key,T>::count() const {return m_list.count();}
  TEMPLATE_DEF inline bool Ptrie<Key,T>::empty() const {return m_list.empty();}

  TEMPLATE_DEF typename Ptrie<Key,T>::const_iterator  Ptrie<Key,T>::const_begin() const
  {
    return const_iterator(m_list.const_begin());
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::const_iterator  Ptrie<Key,T>::const_end() const
  {
    return const_iterator(m_list.const_end());
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator  Ptrie<Key,T>::begin()
  {
    return iterator(m_list.begin());
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator  Ptrie<Key,T>::end()
  {
    return iterator(m_list.end());
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator  Ptrie<Key,T>::last()
  {
	typename hst::deque<hst::triplet<Key,T,void *> >::iterator ll=m_list.end();
    if (ll.hasPrevious())
        --ll;
    return iterator(ll);
  }
  TEMPLATE_DEF Ptrie<Key,T>::const_iterator::const_iterator(const typename sad::LinkedList< sad::Triplet<Key,T,void *> >::const_iterator & rt)
  {
    m_imp=rt;
  }
  TEMPLATE_DEF Ptrie<Key,T>::const_iterator::const_iterator() {}
  TEMPLATE_DEF Ptrie<Key,T>::const_iterator::~const_iterator() {}
  TEMPLATE_DEF Ptrie<Key,T>::const_iterator::const_iterator(  const typename Ptrie<Key,T>::const_iterator & rt ) { m_imp=rt.m_imp; }
  TEMPLATE_DEF typename Ptrie<Key,T>::const_iterator & Ptrie<Key,T>::const_iterator::operator=(  const typename Ptrie<Key,T>::const_iterator & rt ) { m_imp=rt.m_imp; return *this; }
  TEMPLATE_DEF inline bool Ptrie<Key,T>::const_iterator::works() const {return m_imp.works();}
  TEMPLATE_DEF inline bool Ptrie<Key,T>::const_iterator::dereferencable() const {return m_imp.dereferencable();}
  TEMPLATE_DEF typename Ptrie<Key,T>::const_iterator & Ptrie<Key,T>::const_iterator::operator++()
  {
    ++m_imp; return  *this;
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::const_iterator & Ptrie<Key,T>::const_iterator::operator--()
  {
    --m_imp; return  *this;
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::const_iterator  Ptrie<Key,T>::const_iterator::operator++(int)
  {
    const_iterator tmp(*this);
    ++m_imp; return
    tmp;
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::const_iterator  Ptrie<Key,T>::const_iterator::operator--(int)
  {
    const_iterator tmp(*this);
    --m_imp;
    return  tmp;
  }
  TEMPLATE_DEF const Key & Ptrie<Key,T>::const_iterator::key() const
  {
      assert( m_imp.dereferencable() );
      return (*m_imp).p1();
  }
  TEMPLATE_DEF const T & Ptrie<Key,T>::const_iterator::value() const
  {
      assert( m_imp.dereferencable() );
      return (*m_imp).p2();
  }
  TEMPLATE_DEF bool  Ptrie<Key,T>::const_iterator::operator==(const typename Ptrie<Key,T>::const_iterator & rt) const
  {
       return m_imp==rt.m_imp;
  }
  TEMPLATE_DEF bool  Ptrie<Key,T>::const_iterator::operator!=(const typename Ptrie<Key,T>::const_iterator & rt) const
  {
       return m_imp!=rt.m_imp;
  }


  TEMPLATE_DEF Ptrie<Key,T>::iterator::iterator(const typename sad::LinkedList< sad::Triplet<Key,T,void *> >::iterator & rt)
  {
    m_imp=rt;
  }
  TEMPLATE_DEF Ptrie<Key,T>::iterator::iterator() {}
  TEMPLATE_DEF Ptrie<Key,T>::iterator::~iterator() {}
  TEMPLATE_DEF Ptrie<Key,T>::iterator::iterator(  const typename Ptrie<Key,T>::iterator & rt ) { m_imp=rt.m_imp; }
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator & Ptrie<Key,T>::iterator::operator=(  const typename Ptrie<Key,T>::iterator & rt ) { m_imp=rt.m_imp; return *this; }
  TEMPLATE_DEF inline bool Ptrie<Key,T>::iterator::works() const {return m_imp.works();}
  TEMPLATE_DEF inline bool Ptrie<Key,T>::iterator::dereferencable() const {return m_imp.dereferencable();}
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator & Ptrie<Key,T>::iterator::operator++()
  {
    ++m_imp; return  *this;
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator & Ptrie<Key,T>::iterator::operator--()
  {
    --m_imp; return  *this;
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator  Ptrie<Key,T>::iterator::operator++(int)
  {
    iterator tmp(*this);
    ++m_imp; return
    tmp;
  }
  TEMPLATE_DEF typename Ptrie<Key,T>::iterator  Ptrie<Key,T>::iterator::operator--(int)
  {
    iterator tmp(*this);
    --m_imp;
    return  tmp;
  }
  TEMPLATE_DEF const Key & Ptrie<Key,T>::iterator::key()
  {
      assert( m_imp.dereferencable() );
      return (*m_imp)._1();
  }
  TEMPLATE_DEF T & Ptrie<Key,T>::iterator::value()
  {
      assert( m_imp.dereferencable() );
      return (*m_imp)._2();
  }
  #define HDPtrie_IT hst::deque< hst::triplet<Key,T,void *> >::iterator
  TEMPLATE_DEF void  Ptrie<Key,T>::iterator::erase()
  {
      assert( m_imp.dereferencable() );
      typename HDPtrie_IT * p=reinterpret_cast<typename  HDPtrie_IT *>((*m_imp)._3());
      (*p=typename HDPtrie_IT());
      m_imp.erase();
  }
  #undef  HDPtrie_IT
  TEMPLATE_DEF bool  Ptrie<Key,T>::iterator::operator==(const typename Ptrie<Key,T>::iterator & rt) const
  {
       return m_imp==rt.m_imp;
  }
  TEMPLATE_DEF bool  Ptrie<Key,T>::iterator::operator!=(const typename Ptrie<Key,T>::iterator & rt) const
  {
       return m_imp!=rt.m_imp;
  }
  #undef TEMPLATE_DEF
}
