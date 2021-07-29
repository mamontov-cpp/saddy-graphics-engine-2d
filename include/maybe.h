/*!  \file maybe.h
     

     Describes a value, that can be presented or not exists
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <cstring>

#ifdef _MSC_VER
    #pragma warning(disable:26495)
#endif

namespace sad
{
/*! A class, that represents a value or absent value
 */
template<typename T>
class Maybe
{
  private:
      T m_data;        //!< An inner boxed value
      T* m_reference;  //!< A reference part for maybe
      bool m_exists;   //!< Whether value exists
  public:
      /*! Creates new non-existent value
       */
      Maybe() : m_reference(nullptr), m_exists(false) { } //-V730
      /*! Creates a new presented value
          \param[in] data an inner data for value
       */
      Maybe(const  T & data) :  m_data(data), m_reference(nullptr), m_exists(true) { }
      /*! Creates new value for a reference to data
          \param[in] data a data to be set
       */
      Maybe(T* data) : m_reference(data), m_exists(true) { } //-V730
      /*! Sets maybe into a reference mode to make sure, that mutable value will be returned
       */
      void setReference(T* data) { m_reference = data; m_exists = true; }
      /*! Sets a value for objects, making it behave like with inner value
          \param[in] data data value
       */
      void setValue(const T & data)  { if (m_reference) { *m_reference = m_data; } else { m_data = data; } m_exists = true; }
      /*! Clears a maybe value
       */
      void clear() { m_exists = false; }
      /*! Whether inner value is exists
          \return whether inner value is exists
       */
      bool exists() const { return m_exists; }
      /*! Returns true if object can be referenceable
          \return whether object can be referenceable
       */
      bool referenceable() const { return m_reference != nullptr; }
      /*! Returns inner value. If value is not set, default value is returned,
          which is undefined for basic POD types.
          \return inner value
       */
      const  T & value() const  { if (m_reference) return *m_reference; return m_data; }
      /*! Returns inner mutable value. If value is not set, default value is returned,
          which is undefined for basic POD types.
          \return inner value
       */
      T& mutableValue() { if (m_reference) return *m_reference; return m_data; }
};

}
