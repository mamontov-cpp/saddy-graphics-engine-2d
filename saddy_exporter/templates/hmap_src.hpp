namespace hst
{


template<class Key,class T> bool map<Key,T>::cmp(const Key & o1, const Key & o2) const
{
 //printf("Cmp does: %d %d %d\n",o1,o2,(ord==Ascending)?o1<o2:o1>o2);
 if (ord==Ascending)
 {return o1<o2;}
 else
 {return o1>o2;}
}

template<class Key,class T> long map<Key,T>::bkeysearch(long b,long h, const Key & v) const
{
    //keys.dbg_ass("%d ");
    //printf("Dbg :%ld %ld %d %d\n",b,h,keys[b],keys[h]);
    if (_keys[b]==v) return b;
    if (_keys[h]==v) return h;
    if (h-b<2)
    {
          return -1;
    }
    bool flag1=cmp(v,_keys[b]);
    bool flag2=!cmp(v,_keys[h]);
    //printf("Cmp1: %d Cmp2: %d\n",flag1,flag2);
    if (flag1 || flag2)
    {
     //printf("Cmp1: %d Cmp2: %d\n",flag1,flag2);
     return -1;
    }


    int ind=(b+h)/2;
    if (cmp(v,_keys[ind]))
        return bkeysearch(b,ind,v);
    else
        return bkeysearch(ind,h,v);
}

template<class Key,class T> bool map<Key,T>::empty() const
{
    return _keys.count()==0;
}
template<class Key,class T> long map<Key,T>::searchKey(const Key & v) const
{
    if (empty()) return -1;
    return bkeysearch(0,count()-1,v);
}

template<class Key,class T> long map<Key,T>::count() const {return _keys.count();}
template<class Key,class T> const hlvector<Key> & map<Key,T>::keys() const {return _keys;}
template<class Key,class T> const hlvector<hdeque<T> > & map<Key,T>::values() const {return _values;}

template<class Key,class T>   map<Key,T>::map()
{
    ord=Ascending;
}
template<class Key,class T>   map<Key,T>::map(hSortOrder _ord)
{
    ord=_ord;
}

template<class Key,class T> map<Key,T>::map(const map<Key,T> & o)
{
    ord=o.ord;
    _keys=o._keys;
    _values=o._values;
}
template<class Key,class T> map<Key,T> & map<Key,T>::operator=(const map<Key,T> & o)
{
    ord=o.ord;
    _keys=o._keys;
    _values=o._values;
    return *this;
}

template<class Key,class T>   map<Key,T>::map(const hlvector<Key> & keys,const hlvector<T> & values,hSortOrder ord)
{
  this->ord=ord;
  long imax=(keys.count()<values.count())?keys.count():values.count();
  long i;
  for (i=0;i<imax;i++)
      add(keys[i],values[i]);
}

template<class Key,class T>  map<Key,T>::~map()
{

}


template<class Key,class T> map<Key,T> & map<Key,T>::add(const Key & v, const T & t)
{
    long pos,i;
    if ((pos=searchKey(v))!=-1)
    {
        _values[pos]<<t;
        return *this;  //Replace with this
    }
    bool flag=true;
    for (i=0;(i<count()) && (flag);i++)
       if (cmp(v,_keys[i]))
       {
           flag=false;
           --i;
       }
    //puts("--------------");
    //printf("%ld",i);
    //printf("\n%s\n","-------------");
    _keys.insert(v,i);
    _values.insert(hdeque<T>(),i);
    _values[i]<<t;
    return *this;
}



template<class Key,class T> void map<Key,T>::dbg_ass(const char * kfmt,const char * fmt) const
{
  puts("---map---");
  long i,j;
  for (i=0;i<count();i++)
  {
      printf("%ld. Key:",i);
      printf(kfmt,_keys[i]);
      printf("\nValues %c",'\n');
      for (j=0;j<_values[i].count();j++)
      {
        printf(fmt,_values[i][j]);
        printf("%c",'\n');
      }
      printf("%c",'\n');
  }
}

template<class Key,class T> void map<Key,T>::dbg_ass(void (*kfun)(const T & o),void (*fun)(const T & o)) const
{
  puts("---map---");
  long i,j;
  for (i=0;i<count();i++)
  {
      printf("%d. Key:",i);
      (*kfun)(_keys[i]);
      printf("\nValues %c",'\n');
      for (j=0;j<_values[i].count();j++)
         (*fun)(_values[i][j]);
      printf("%c",'\n');
  }
}

template<class Key,class T>  void map<Key,T>::clear()
{
    _keys.clear();
    _values.clear();
}

template<class Key,class T> void map<Key,T>::reverse()
{
 ord=(ord==Ascending)?Descending:Ascending;
 long imax=(count() % 2 ==0)?count()/2-1:count()/2;
 long i;
 hdeque<T> tmp;
 Key tmpkey;
 for (i=0;i<=imax;i++)
 {
     tmp=_values[i];
     _values[i]=_values[count()-1-i];
     _values[count()-1-i]=tmp;

     tmpkey=_keys[i];
     _keys[i]=_keys[count()-1-i];
     _keys[count()-1-i]=tmpkey;
 }
}

template<class Key,class T> bool map<Key,T>::operator==(const map<Key,T> & o)
{
    return (o.ord==ord) && (_keys==o._keys) && (_values==o._values);
}
template<class Key,class T> bool map<Key,T>::operator!=(const map<Key,T> & o)
{
    return !(*this==o);
}

template<class Key,class T> bool map<Key,T>::contains(const Key & v) const
{
    return searchKey(v)!=-1;
}
template<class Key,class T> map<Key,T> & map<Key,T>::remove(const Key & v)
{
  long pos=searchKey(v);
  if (pos!=-1)
  {
      _keys.removeAt(pos);
      _values.removeAt(pos);
  }
  return *this;
}


template<class Key,class T> T & map<Key,T>::operator[](const Key & k)
{
    long pos=searchKey(k);
    if (pos!=-1) return _values[pos][0];
    add(k,T());
    return (*this)[k];
}

template<class Key,class T> const T & map<Key,T>::operator[](const Key & k) const
{
    long pos=searchKey(k);
    if (pos!=-1) return _values[pos][0];
    return *(new T());
}

template<class Key,class T> hdeque<T> & map<Key,T>::elements(const Key & k)
{
    long pos=searchKey(k);
    if (pos!=-1) return _values[pos];
    add(k,T());
    return (*this)[k];
}

template<class Key,class T> const hdeque<T> & map<Key,T>::elements(const Key & k) const
{
    long pos=searchKey(k);
    if (pos!=-1) return _values[pos];
    return &((*(new T()))<<T());
}

#define __mapCIT hst::map<Key,T>::const_iterator
//Here follows an implementation of const_iterator
template<class Key,class T> __mapCIT::const_iterator()
{
    parent=NULL;
    pos=0;
    depth=0;
}
template<class Key,class T> __mapCIT::~const_iterator()
{

}
template<class Key,class T> __mapCIT::const_iterator(const class __mapCIT & o)
{
    parent=o.parent;
    pos=o.pos;
    depth=o.depth;
}
template<class Key,class T> __mapCIT::const_iterator(const class map<Key,T> * parent, long mypos, long mydepth)
{
  this->parent=parent;
  this->pos=mypos;
  this->depth=mydepth;
}

template<class Key,class T>
typename __mapCIT & __mapCIT::operator=(const class __mapCIT & o)
{
    parent=o.parent;
    pos=o.pos;
    depth=o.depth;
    return *this;
}
template<class Key,class T>
bool __mapCIT::operator==(const class __mapCIT & o) const
{
    return !(*this!=o);
}
template<class Key,class T>
bool __mapCIT::operator!=(const class __mapCIT & o) const
{
    if (parent!=o.parent) return true;
	if (pos!=o.pos)       return true;
	if (depth!=o.depth)   return true;
	return false;
}

template<class Key,class T>  void __mapCIT::broke() {parent=NULL;}
template<class Key,class T>  bool __mapCIT::works() const {return parent!=NULL;}
template<class Key,class T>
const Key &  __mapCIT::key() const
{
    if (works())
       return parent->keys()[pos];
    else
       return *(new Key());
}
template<class Key,class T>
const T & __mapCIT::value() const
{
    if (works())
       return parent->values()[pos][depth];
    else
       return *(new T());
}

template<class Key,class T>
typename __mapCIT & __mapCIT::operator++()
{
    ++pos;
    depth=0;
    return *this;
}
template<class Key,class T>
typename __mapCIT & __mapCIT::operator--()
{
    --pos;
    depth=0;
    return *this;
}
template<class Key,class T>
typename __mapCIT  __mapCIT::operator++(int)
{
    const_iterator res=*this;
    ++pos;
    depth=0;
    return res;
}
template<class Key,class T>
typename __mapCIT  __mapCIT::operator--(int)
{
    const_iterator res=*this;
    --pos;
    depth=0;
    return res;
}
template<class Key,class T>
typename __mapCIT & __mapCIT::up()
{
    --depth;
    return *this;
}
template<class Key,class T>
typename __mapCIT & __mapCIT::dn()
{
    ++depth;
    return *this;
}

template<class Key,class T>
typename __mapCIT  __mapCIT::begin()
{
    if (works())
        return const_iterator(parent,pos,0);
    return  const_iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapCIT  __mapCIT::end()
{
    if (works())
        return const_iterator(parent,pos,parent->values()[pos].count());
    return  const_iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapCIT map<Key,T>::const_begin() const
{
    if (_keys.count()!=0)
       return const_iterator(this,0,0);
    return const_iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapCIT map<Key,T>::const_end() const
{
    if (_keys.count()!=0)
       return const_iterator(this,_values.count(),0);
    return const_iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapCIT map<Key,T>::at(long i) const
{
    if (i>=0 && i<_values.count())
        return const_iterator(this,i,0);
    return const_iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapCIT map<Key,T>::at(long i,long j) const
{
    if (i>=0 && i<_values.count())
       if (j>=0 && j<_values[i].count())
            return const_iterator(this,i,j);
    return const_iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapCIT map<Key,T>::seek(const Key & k) const
{
    long pos=searchKey(k);
    if (pos!=-1)
         return const_iterator(this,pos,0);
    return const_iterator(NULL,0,0);
}
#undef  __mapCIT

#define __mapIT hst::map<Key,T>::iterator

template<class Key,class T> __mapIT::iterator()
{
    parent=NULL;
    pos=0;
    depth=0;
}
template<class Key,class T> __mapIT::~iterator()
{

}
template<class Key,class T> __mapIT::iterator(const class __mapIT & o)
{
    parent=o.parent;
    pos=o.pos;
    depth=o.depth;
}
template<class Key,class T> __mapIT::iterator(class map<Key,T> * parent, long mypos, long mydepth)
{
  this->parent=parent;
  this->pos=mypos;
  this->depth=mydepth;
}

template<class Key,class T>
typename __mapIT & __mapIT::operator=(const class __mapIT & o)
{
    parent=o.parent;
    pos=o.pos;
    depth=o.depth;
    return *this;
}
template<class Key,class T>
bool __mapIT::operator==(const class __mapIT & o) const
{
    return !(*this!=o);
}
template<class Key,class T>
bool __mapIT::operator!=(const class __mapIT & o) const
{
    if (parent!=o.parent) return true;
	if (pos!=o.pos)       return true;
	if (depth!=o.depth)   return true;
	return false;
}

template<class Key,class T>  void __mapIT::broke() {parent=NULL;}
template<class Key,class T>  bool __mapIT::works() const {return parent!=NULL;}
template<class Key,class T>
const Key &  __mapIT::key() const
{
    if (works())
       return parent->keys()[pos];
    else
       return *(new Key());
}
template<class Key,class T>
T & __mapIT::value() const
{
    if (works())
       return parent->_values[pos][depth];
    else
       return *(new T());
}

template<class Key,class T>
typename __mapIT & __mapIT::operator++()
{
    ++pos;
    depth=0;
    return *this;
}
template<class Key,class T>
typename __mapIT & __mapIT::operator--()
{
    --pos;
    depth=0;
    return *this;
}
template<class Key,class T>
typename __mapIT  __mapIT::operator++(int)
{
    iterator res=*this;
    ++pos;
    depth=0;
    return res;
}
template<class Key,class T>
typename __mapIT  __mapIT::operator--(int)
{
    iterator res=*this;
    --pos;
    depth=0;
    return res;
}
template<class Key,class T>
typename __mapIT & __mapIT::up()
{
    --depth;
    return *this;
}
template<class Key,class T>
typename __mapIT & __mapIT::dn()
{
    ++depth;
    return *this;
}

template<class Key,class T>
typename __mapIT  __mapIT::begin()
{
    if (works())
        return iterator(parent,pos,0);
    return  iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapIT  __mapIT::end()
{
    if (works())
        return iterator(parent,pos,parent->values()[pos].count());
    return  iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapIT &  __mapIT::insert(const T & obj)
{
    if (parent==NULL) return *this;
    if (pos<0 || (unsigned long)pos>=(parent->_values.count())) return *this;
    if (parent->_values[pos].count()!=depth)
    {
        parent->_values[pos].insert(obj,depth);
    }
    else
    {
        parent->_values[pos].add(obj);
    }
    return *this;
}

template<class Key,class T>
typename __mapIT &  __mapIT::erase()
{
    if (parent==NULL) return *this;
    if (pos<0 || (unsigned long)pos>=(parent->_values.count())) return *this;
    if (depth<0 || depth>=(parent->_values[pos].count())) return *this;
    parent->_values[pos].removeAt(depth);
    if (depth>=parent->_values[pos].count())
    {
        depth=parent->_values[pos].count()-1;
    }
    if (parent->_values[pos].count()==0)
    {
        depth=0;
        --pos;
    }
    if ((unsigned long)pos>=parent->_values.count())
    {
        pos=parent->_values.count()-1;
    }
    if (pos==-1) broke();
    return *this;
}


template<class Key,class T>
typename __mapIT map<Key,T>::begin()
{
    if (_keys.count()!=0)
       return iterator(this,0,0);
    return iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapIT map<Key,T>::end()
{
    if (_keys.count()!=0)
       return iterator(this,_values.count(),0);
    return iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapIT map<Key,T>::at(long i)
{
    if (i>=0 && (unsigned long)i<_values.count())
        return iterator(this,i,0);
    return iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapIT map<Key,T>::at(long i,long j)
{
    if (i>=0 && (unsigned long)i<_values.count())
	{
	   unsigned long cnt=_values[(unsigned long)i].count();
       if (j>=0 && ((unsigned long)j)<cnt)
            return iterator(this,i,j);
	}
	return iterator(NULL,0,0);
}
template<class Key,class T>
typename __mapIT map<Key,T>::seek(const Key & k)
{
    long pos=searchKey(k);
    if (pos!=-1)
         return iterator(this,pos,0);
    return iterator(NULL,0,0);
}
#undef  __mapIT

}
