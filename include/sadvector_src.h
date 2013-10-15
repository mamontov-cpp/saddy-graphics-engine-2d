#pragma once
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


namespace sad
{

template<class T> Vector<T>::Vector()
{

}
template<class T> Vector<T>::~Vector()
{

}

template<class T> Vector<T>::Vector(const Vector & h): std::vector<T>(h)
{

}

template<class T> Vector<T> & Vector<T>::operator=(const Vector<T>&  h)        //Присваивание
{
  std::vector<T> & t1 = *this;
  const std::vector<T> & t2 = h;
  t1 = t2;
  return *this;
}

template<class T> inline unsigned long Vector<T>::count() const              {return this->size();}
template<class T> inline T*   Vector<T>::data()  const              
{
	if (this->size() == 0)
		return  NULL;
	sad::Vector<T> & v = const_cast<sad::Vector<T> &>(*this);
	return &(v[0]);
}

template<class T> Vector<T> & Vector<T>::operator<<(const T& obj)
{
    this->push_back(obj);
    return *this;
}

template<class T> Vector<T> & Vector<T>::operator<<(const Vector<T> & obj)
{
    for(unsigned int i = 0; i < obj.size(); i++)
		this->push_back(obj[i]);
    return *this;
}

template<class T> Vector<T> & Vector<T>::operator>>(T& obj)
{
    obj=(*this)[this->size()-1];
    removeAt(this->size()-1);
    return *this;
}

template<class T> Vector<T>& Vector<T>::removeAt(unsigned long i)
{
    if (i>=this->size()) return *this;
    this->erase(this->begin() + i);
    return *this;
}

template<class T> Vector<T> & Vector<T>::add(const T & obj)
{
    return *this<<obj;
}
template<class T> Vector<T> & Vector<T>::addFront(const T & obj)
{
    return insert(obj,0);
}

template<class T> Vector<T> & Vector<T>::insert(const T &obj,unsigned long i)
{
  if (i>=this->size()) return add(obj);
  this->std::vector<T>::insert(this->begin() + i, obj);
  return *this;
}

template<class T> Vector<T> & Vector<T>::removeAll(const T& obj)
{
    unsigned long i;
    for (i=0;i<this->size();i++)
    {
        if ((*this)[i]==obj)
        {
            removeAt(i);
            i--;
        }
    }
   return *this;
}
template<class T> Vector<T> & Vector<T>::removeFirst(const T& obj)
{
    unsigned long i;
    for (i=0;i<this->size();i++)
    {
        if ((*this)[i]==obj)
        {
            return removeAt(i);
        }
    }
   return *this;
}
template<class T> Vector<T> & Vector<T>::removeLast(const T& obj)
{
    unsigned long i,fnd=0;
    bool flag=false;
    for (i=0;i<this->size();i++)
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
template<class T> Vector<T> & Vector<T>::removeRange(unsigned long imin,unsigned long imax)
{
    unsigned long i;
    for (i=imin;i<=imax;i++)
       removeAt(imin);
    return *this;
}

template<class T> bool Vector<T>::operator==(const Vector<T> & o)
{
  const std::vector<T> & t1 = *this;
  const std::vector<T> & t2 = o;
  return t1 == t2;
}
template<class T> bool Vector<T>::operator!=(const Vector<T> & o)
{
	return !(*this==o);
}

template<class T> void  Vector<T>::rescale(unsigned long _sz)
{
   this->resize(_sz);
}


}
