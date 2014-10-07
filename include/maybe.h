/*!  \file maybe.h
	 \author HiddenSeeker

	 Describes a value, that can be presented or not exists
 */
#pragma once

namespace sad
{
/*! A class, that represents a value or absent value
 */
template<typename T>
class Maybe
{
  private:
	  T m_data;      //!< An inner boxed value
	  bool m_exists; //!< Whether value exists
  public:
	  /*! Creates new non-existent value
	   */
	  Maybe() : m_exists(false) { }
	  /*! Creates a new presented value
		  \param[in] data an inner data for value
	   */
	  Maybe(const  T & data) :  m_exists(true), m_data(data) { }
	  /*! Sets a value for objects, making it behave like with inner value
		  \param[in] data data value
	   */
	  void setValue(const T & data)  { m_data = data; m_exists = true; }
	  /*! Clears a maybe value
	   */
	  void clear() { m_exists = false; }
	  /*! Whether inner value is exists
		  \return whether inner value is exists
	   */
	  bool exists() const { return m_exists; }
	  /*! Returns inner value. If value is not set, default value is returned,
		  which is undefined for basic POD types.
		  \return inner value
	   */
	  const  T & value() const  { return m_data; }
};

}
