/*! \file   hwstring.h
    \author HiddenSeeker
	\brief  Wide string definition
	
	A wide string definition is placed here
*/
#include <wchar.h>
#include <stdlib.h>
#include <string>
#pragma once

#ifndef __HWSTRING_H
	#define __HWSTRING_H
namespace hst
{
  template<typename T> class list;
  /*! \class wstring
      
	  Definition a wide char string is placed here
  */
  class wstring 
  {
    private:
	       std::wstring m_str; //!< Wstring, fucking
    public:
	       /*! Assignment operator overload
		       \param[in] o other string, lol
			   \return self-reference
		   */
		   wstring & operator=(const wstring & o);
	       /*! Appends a string to a self
		       \param[in] o other  string
			   \return self-reference
		   */
		   wstring & operator<<(const wstring & o);
		   /*! Constructor
		   */
	       wstring();
		   /*! Constructs from string
		       \param[in] str string
		   */
	       wstring(const char * str);
		   /*! Constructs from string
		       \param[in] str string
		   */
		   wstring(const wchar_t * str);
		   /*! Constructs from string
		       \param[in] str string
		   */
		   wstring(const std::wstring & str);
		   /*! Destructor
		   */
		   ~wstring();
		   /*!  Accessor
		        \param[in] i index
		   */
		   inline wchar_t & operator[](unsigned int i)   {return m_str[i];}
		   /*!  Accessor
		        \param[in] i  index
		   */
		   inline const wchar_t & operator[](unsigned int i) const {return m_str[i];}
		   /*! Returns a data
		   */
		   inline const wchar_t * data() const { return m_str.c_str(); }
		   /*! Return length
		   */
		   inline unsigned int length() const  { return m_str.length(); }
		   /*! Clears 
		   */
		   inline void clear() { return m_str.clear(); }
		   /*! Is an empty?
		   */
		   inline bool empty() const { return m_str.empty(); }
		   /*! Removes a char at position
		       \param[in] i position
			   \return self-reference
		   */
		   wstring & remove(unsigned int i);
		   /*! Inserts a char in a specified position
		       \param[in] i position
			   \param[in] str string
			   \return self-reference
		   */
		   wstring & insert(unsigned int i,const wstring & str);
		   /*! Concats two strings 
		       \param[in] str other string
			   \return sum of strings
		   */
		   wstring operator+(const wstring & str);
		   /*! Compares two strings
		       \param[in] o other string
			   \return true, if equal
		   */
		   bool operator==(const wstring & o) const;
		   /*! Compares two strings
		       \param[in] o other string
			   \return true, if not equal
		   */
		   bool operator!=(const wstring & o) const;
		   /*! Splits a wide string
		       \param[in] o splitter
			   \return splitted parts
		   */
		   hst::list<wstring> split(const wstring & o);
		   /*! Removes first occurence from a wide string
		       \param[in] o other string
		   */
		   void removeFirst(const wstring & o);
		   /*! Removes last occurence from a wide string
		       \param[in] o other string
		   */
		   void removeLast(const wstring & o);
  };
}

#endif
