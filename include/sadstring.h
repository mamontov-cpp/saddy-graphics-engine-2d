/*! \file    sadstring.h
    \author  mamontov-cpp

	\brief   Definition of string.

	This file contains a definition of string.
*/
#pragma once
#include "sadvector.h"
#include <ostream>

namespace sad
{

class String;

typedef sad::Vector<sad::String> StringList;

/*! \class  string
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
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if bigger.
		 */
		 bool operator>(const String & o) const;
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if bigger or equal
		 */
		 bool operator>=(const String & o) const;
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if lesser.
		 */
		 bool operator<(const String & o) const;
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if lesser or equal.
		 */
		 bool operator<=(const String & o) const;
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
		 /*! Removes rlen symbols, beginning from beg
		     \param[in] beg  beginning index
			 \param[in] rlen length of range
			 \return self-reference
		 */
		 String & removeRange(long beg,long rlen);
	     /*! Concates two strings
		     \param[in] o other string
			 \return concatenated symbols
		 */
		 String  operator+(const String & o) const;
		 /*! Concates two strings
		     \param[in] o other string
			 \return concatenated symbols
		 */
		 String  operator+(const char * o) const;
		 /*! Concates two strings
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
		     \param[in] newext new extension
     	 */
		 void  addExtension(const String & newext);
		 /*! Changes a file extension to string
		     \param[in] newext new extension
     	 */
		 void  changeExtension(const String & newext);
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
		 String  getRightPart(long len);
		 /*! Returns a left part of string. If len bigger length, all string will be returned
		     \param[in] len length of substring
			 \return substring
		 */
		 String  getLeftPart(long len);
		 /*! Count occurences of string sstr int this string.
		     \param[in] sstr string, which occurences will be counted
			 \return -1 if no occurences, otherwise amount of occurences
		 */
		 long getOccurences(const String & sstr) const;
		 /*! Returns a position of substring.
		    \param[in] sstr  searching string
			\param[in] omax  searching occurence of string.0 - for first.
		    \return -1  if not found, otherwise - index
		 */
		 long getOccurence(const String & sstr,long omax=0);
		 /*! Returns index of last occurence of one string from this string
		     \param[in] sstr substring to be removed
			 \return -1 if not found, otherwise index of substring
		 */
		 long getLastOccurence(const String & sstr);
		 /*! Removes last occurence of one string from this string
		     \param[in] sstr substring to be removed
			 \param[in] omax index of occurence
		 */
		 void removeOccurence(const String & sstr,long omax=0);
		 /*! Removes last occurence of one string from this string
		     \param[in] sstr substring to be removed
		 */
		 void removeLastOccurence(const String & sstr);
		 /*! Removes all occurences of one string from this string
		     \param[in] sstr substring to be removed
		 */
		 void removeAllOccurences(const String & sstr);
		 /*! Replaces following occurence of one string to other. If this occurence
		     isn't found, nothing will be done
		     \param[in] sstr string to be replaced
			 \param[in] to   replacing string
			 \param[in] omax index of occurence
		 */
         void replaceOccurence(const String & sstr,const String & to,long omax=0);
		 /*! Replaces last occurence of one string to other
		     \param[in] sstr string to be replaced
			 \param[in] to   replacing string
		 */
		 void replaceLastOccurence(const String & sstr,const String & to);
		 /*! Replaces all occurences of one string to other
		     \param[in] sstr string to be replaced
			 \param[in] to   replacing string
		 */
		 void replaceAllOccurences(const String & sstr,const String & to);
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
		     \param[out] addr  integral address
		     \return    true if success
		 */
		 static bool  queryPointer(const String & str, long * addr);
		 /*! String conversion to integral type. Uses sscanf() to convert
		     \param[in] str string to be converted
			 \return    result of conversion
		 */
		 static int  toInt(const String & str);
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
		 /*! Trims a whitespace characters, like tabs, spaces or newlinws from both sides
		  */
		 void trim();
private:
	     /*! It compares symbols it by priority on russian language.
			 The biggest are big latin symbols
             then go other symbols, like small latin symbols, locales and numerics.
			 \param[in] c1 first char
			 \param[in] c2 second char
			 \return false if c1 is bigger than c2 by priority or code
		 */
		 bool cmpchar(char c1,char c2) const;
};

/*! Joins a list into a string with separator sep.
	For example join({'a', 'b', 'c'}) will become  'a,b,c'
	\param[in] list list of strings
	\param[in] sep  separator
 */
sad::String join(const sad::StringList list, const sad::String & sep);

};

/*! Writes a string to output stream
	\param[in] stream output stream
	\param[in] sadstring a string data
 */
inline std::ostream & operator<<(std::ostream & stream, const sad::String & sadstring)
{
	return stream << static_cast<const std::string&>(sadstring);
}

