/*! \file   sadwstring.h
    
    \brief  Wide string definition
    
    A wide string definition is placed here
*/
#pragma once
#include "sadstring.h"
#include "sadvector.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <cwchar>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdlib>
#include <string>

namespace sad
{
  /*! \class WString
      
      Definition a wide char string is placed here
  */
  class WString: public std::wstring 
  {
    public:
           /*! Appends a string to a self
               \param[in] o other  string
               \return self-reference
           */
           WString & operator<<(const WString & o);
           /*! Constructor
           */
           WString();
           /*! Constructs from string
               \param[in] str string
           */
           WString(const wchar_t * str);
           /*! Constructs from string
               \param[in] str string
           */
           WString(const std::wstring & str);
           /*! Destructor
           */
           ~WString();
           /*! Returns a data
           */
           inline const wchar_t * data() const { return this->c_str(); }
           /*! Removes a char at position
               \param[in] i position
               \return self-reference
           */
           WString & remove(unsigned int i);
           /*! Inserts a char in a specified position
               \param[in] i position
               \param[in] str string
               \return self-reference
           */
           WString & insert(unsigned int i,const WString & str);
           /*! Concatenates two strings 
               \param[in] str other string
               \return sum of strings
           */
           WString operator+(const WString & str);
           /*! Compares two strings
               \param[in] o other string
               \return true, if equal
           */
           bool operator==(const WString & o) const;
           /*! Compares two strings
               \param[in] o other string
               \return true, if not equal
           */
           bool operator!=(const WString & o) const;
           /*! Splits a wide string
               \param[in] o splitter
               \param[in] b behaviour
               \return split parts
           */
           sad::Vector<sad::WString> split(const sad::WString & o, sad::String::SplitBehaviour b = sad::String::SplitBehaviour::OMIT_EMPTY_PARTS);
           /*! Removes first occurrence from a wide string
               \param[in] o other string
           */
           void removeFirst(const WString & o);
           /*! Removes last occurrence from a wide string
               \param[in] o other string
           */
           void removeLast(const WString & o);
  };
  /*! Converts a char pointer to wide string
      \param[in] p pointer
      \return wide string
   */
  sad::WString widestring_from_charpointer(const char * p);
}
