/*! \file    sadstring.h
    \author  mamontov-cpp

    \brief   Definition of string.

    This file contains a definition of string.
*/
#pragma once
#include "sadvector.h"
#include <string>
#include <ostream>

namespace sad
{

class String;

typedef sad::Vector<sad::String> StringList;

/*! \class  String
    \brief  String class

    String class, written to extend standard class with some useful methods
*/
class String: public std::string
{
public:
        /*! Settings for splitting parts of string
         */
        enum SplitBehaviour
        {
            /*! Keep empty parts of string
             */
            KEEP_EMPTY_PARTS,
            /*! Omits empty parts of string
             */
            OMIT_EMPTY_PARTS
        };
         String();                               //!< Default constructor.
         /*! Creates a string by copying the string, addressed by pointer.
             \param[in] _p address of string
         */
         String(const char* _p);
         /*! Copies a len symbols from p, an adds a 0-symbol.
             \param[in] _p    address of symbols
             \param[in] len  length of sequence
         */
         String(const char* _p,long len);
         /*! Copy constructor.
             \param[in] o other string
         */
         String(const String & o);
        /*! Copy constructor.
             \param[in] o other string
         */
         String(const std::string & o);
         /*! Removes a symbol at position
             \param[in] i position of symbol
         */
         void remove(long i);
         /*! Splits a string by delimiter, returning a list of strings
             \param[in] delimiter delimiter char, removing empty strings between two delimiters
             \param[in] b behaviour for splitting a string
             \return list of strings as a result of splitting
         */
         sad::StringList split(
             char delimiter,
             sad::String::SplitBehaviour b = sad::String::OMIT_EMPTY_PARTS
         ) const;
         /*! Splits a string by delimiters, returning a list of strings
             \param[in] delimiters delimiter char, removing empty strings between two delimiters
             \param[in] b behaviour for splitting a string
             \return list of strings as a result of splitting
         */
         sad::StringList split(
             const char * delimiters, 
             sad::String::SplitBehaviour b = sad::String::OMIT_EMPTY_PARTS
         ) const;
         /*! Places a character to end of string
             \param[in] c character
             \return self-reference
         */
         String & operator<<(char c);
         /*! Places one string to end of other
             \param[in] o string
             \return self-reference
        */
         String & operator<<(const sad::String & o);
         /*! Inserts a character into a position
             \param[in] c char to be inserted
             \param[in] i index of place
             \return self-reference
         */
         String & insert(char c,long i);
         /*! Inserts a string into a position
             \param[in] o string to be inserted
             \param[in] i index of place
             \return self-reference
         */
         String & insert(const String & o,long i);
         /*! Removes right_length symbols, beginning from beg
             \param[in] beg  beginning index
             \param[in] right_length length of range
             \return self-reference
         */
         String & removeRange(long beg,long right_length);
         /*! Concatenates two strings
             \param[in] o other string
             \return concatenated symbols
         */
         String  operator+(const String & o) const;
         /*! Concatenates two strings
             \param[in] o other string
             \return concatenated symbols
         */
         String  operator+(const char * o) const;
         /*! Concatenates two strings
             \param[in] o other string
             \return concatenated symbols
         */
         String  operator+(const std::string & o) const;
         /*! Returns last i characters
             \param[in] i amount of characters to be extracted
             \return string of last characters
         */
         String  getLastCharacters(long i) const;
         /*! Extracts an extension for file, but
             if no extension is specified it returns an empty string
             \return extension
         */
         String  getExtension() const;
         /*! Adds a file extension to string
             \param[in] new_extension new extension
         */
         void  addExtension(const String & new_extension);
         /*! Changes a file extension to string
             \param[in] new_extension new extension
         */
         void  changeExtension(const String & new_extension);
         /*! Removes file extension or does nothing if no extension found
         */
         void  removeExtension();
         /*! Returns a substring of string
             \param[in] beg index beginning char
             \param[in] len length of substring
             \return substring
         */
         String  subString(long beg,long len) const;
         /*! Returns a right part of string. If len bigger length, all string will be returned
             \param[in] len length of substring
             \return substring
         */
         String  getRightPart(long len) const;
         /*! Returns a left part of string. If len bigger length, all string will be returned
             \param[in] len length of substring
             \return substring
         */
         String  getLeftPart(long len) const;
         /*! Count occurrences of string sub_string int this string.
             \param[in] sub_string string, which occurrences will be counted
             \return -1 if no occurrences, otherwise amount of occurrences
         */
         long getOccurrences(const String & sub_string) const;
         /*! Returns a position of substring.
            \param[in] sub_string  searching string
            \param[in] o_max  searching occurrence of string.0 - for first.
            \return -1  if not found, otherwise - index
         */
         long getOccurrence(const String & sub_string, long o_max = 0) const;
         /*! Returns index of last occurrence of one string from this string
             \param[in] sub_string substring to be removed
             \return -1 if not found, otherwise index of substring
         */
         long getLastOccurrence(const String & sub_string) const;
         /*! Removes last occurrence of one string from this string
             \param[in] sub_string substring to be removed
             \param[in] o_max index of occurrence
         */
         void removeOccurrence(const String & sub_string, long o_max = 0);
         /*! Removes last occurrence of one string from this string
             \param[in] sub_string substring to be removed
         */
         void removeLastOccurrence(const String & sub_string);
         /*! Removes all occurrences of one string from this string
             \param[in] sub_string substring to be removed
         */
         void removeAllOccurrences(const String & sub_string);
         /*! Replaces following occurrence of one string to other. If this occurrence
             isn't found, nothing will be done
             \param[in] sub_string string to be replaced
             \param[in] to   replacing string
             \param[in] o_max index of occurrence
         */
         void replaceOccurrence(const String & sub_string,const String & to,long o_max=0);
         /*! Replaces last occurrence of one string to other
             \param[in] sub_string string to be replaced
             \param[in] to   replacing string
         */
         void replaceLastOccurrence(const String & sub_string,const String & to);
         /*! Replaces all occurrences of one string to other
             \param[in] sub_string string to be replaced
             \param[in] to   replacing string
         */
         void replaceAllOccurrences(const String & sub_string,const String & to);
         //Trivial
         void removeSpaces();                                  //!< Removes all spaces from a string
         void trimSpaces();                                    //!< Removes a spaces from begin and end of string
         /*! Converts number to string.
         *   Supports 8,10,16 radixes.
         *   \param[in] a       numeric to convert
         *   \param[in] radix   Conversion radix
         */
         static String number(int a,int radix=10);
         /*! Converts pointer definition from string to long
             \param[in]  str  string to query
             \param[out] address  integral address
             \return    true if success
         */
         static bool  queryPointer(const String & str, long* address);
         /*! String conversion to integral type. Uses sscanf() to convert
             \param[in] str string to be converted
             \return    result of conversion
         */
         static int  toInt(const String & str);
         /*! An unsafe fast version of parsing unsigned int
             \param[in]  begin beginning of buffer
             \param[in]  end last character of buffer
             \param[out] r resulting number
             \return whether parsing was successfull
          */
         static bool parseUInt(const char* begin, const char* end, unsigned int* r);
         /*! String conversion to float type. Uses sscanf() to convert
             \param[in] str string to be converted
             \return    result of conversion
         */
         static float toFloat(const String & str);
         /*! String conversion to double type. Uses sscanf() to convert
             \param[in] str string to be converted
             \return    result of conversion
         */
         static double toDouble(const String & str);
         /*! Tests, whether string consists only from white-space characters, like
             spaces, tabs, or newlines or return of carets
             \return result
          */
         bool consistsOfWhitespaceCharacters() const;
         /*! Trims a whitespace characters, like tabs, spaces or newlines from both sides
          */
         void trim();
         /*! Trims a whitespace characters on left side of string
            \return whether string consists from whitespaces
          */
         bool trimLeft();
         /*! Trims a whitespace characters on right side of string
            \return whether string consists from whitespaces
          */
         bool trimRight();
         /*! Tests, whether string starts with specified symbols
             \param[in] s string
             \param[in] sz size
          */
         bool startsWith(const char* s, size_t sz) const;
         /*! Tests, whether string ends with specified symbols
             \param[in] s string
             \param[in] sz size
          */   
         bool endsWith(const char* s, size_t sz) const;
         /*! Converts string to upper case
          */
         void toUpper();
         /*! Converts string to lower case
         */
         void toLower();
         /*! Tests if string contains only numeric (and negativity sign if provided)
             \param[in] allowMinus if true, minus can be first character in string
             \return whether string matches all of constraints
          */
         bool isNumeric(bool allowMinus = false) const;
};

/*! Joins a list into a string with separator sep.
    For example join({'a', 'b', 'c'}) will become  'a,b,c'
    \param[in] list list of strings
    \param[in] sep  separator
 */
sad::String join(const sad::StringList& list, const sad::String & sep);

};

/*! Writes a string to output stream
    \param[in] stream output stream
    \param[in] string a string data
 */
inline std::ostream & operator<<(std::ostream & stream, const sad::String & string)
{
    return stream << static_cast<const std::string&>(string);
}

