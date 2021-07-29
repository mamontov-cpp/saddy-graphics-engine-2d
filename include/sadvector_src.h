#pragma once
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


namespace sad
{

template<
    typename T
>
sad::Vector<T>::Vector()
{

}

template<
    typename T
>
sad::Vector<T>::~Vector()
{

}

template<
    typename T
>
inline size_t sad::Vector<T>::count() const              
{
    return this->size();
}
template<
    typename T
>
inline T* sad::Vector<T>::data()  const              
{
    if (this->size() == 0)
        return  NULL;
    sad::Vector<T> & v = const_cast<sad::Vector<T> &>(*this);
    return &(v[0]);
}

template<
    typename T
>
sad::Vector<T>& sad::Vector<T>::operator<<(const T& obj)
{
    this->push_back(obj);
    return *this;
}

template<
    typename T
>
sad::Vector<T>& sad::Vector<T>::operator<<(const sad::Vector<T>& obj)
{
    for(unsigned int i = 0; i < obj.size(); i++)
        this->push_back(obj[i]);
    return *this;
}

template<
    typename T
>
sad::Vector<T> & sad::Vector<T>::operator>>(T& obj)
{
    if (this->size()) 
    {
        obj=(*this)[this->size() - 1];
        removeAt(this->size() - 1);
    }
    return *this;
}

template<
    typename T
>
sad::Vector<T>& sad::Vector<T>::removeAt(unsigned long i)
{
    if (i >= this->size()) return *this;
    this->erase(this->begin() + i);
    return *this;
}

template<
    typename T
>
Vector<T> & Vector<T>::add(const T & obj)
{
    return *this << obj;
}
template<
    typename T
>
Vector<T> & Vector<T>::addFront(const T & obj)
{
    return insert(obj,0);
}

template<
    typename T
>
sad::Vector<T> & sad::Vector<T>::insert(const T &obj,unsigned long i)
{
  if (i >= this->size()) return add(obj);
  this->std::vector<T>::insert(this->begin() + i, obj);
  return *this;
}

template<
    typename T
>
sad::Vector<T> & sad::Vector<T>::removeAll(const T& obj)
{
    unsigned long i;
    for (i = 0; i < this->size(); i++)
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
sad::Vector<T> & sad::Vector<T>::removeFirst(const T& obj)
{
    unsigned long i;
    for (i = 0; i < this->size(); i++)
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
sad::Vector<T> & sad::Vector<T>::removeLast(const T& obj)
{
    unsigned long i, fnd = 0;
    bool flag = false;
    for (i = 0; i < this->size();i++)
    {
        if ((*this)[i] == obj)
        {
            fnd = i;
            flag = true;
        }
    }
    if (flag) 
        removeAt(fnd);
   return *this;
}

template<
    typename T
>
sad::Vector<T> & sad::Vector<T>::removeRange(unsigned long i_min,unsigned long i_max)
{
    unsigned long i;
    for (i = i_min;i <= i_max;i++)
       removeAt(i_min);
    return *this;
}

}
