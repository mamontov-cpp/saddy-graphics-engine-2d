/*! \file    hstring.h
    \author  HiddenSeeker

	\brief   Definition of string.

	This file contains a definition of string.
*/
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <string.h>
#include <vector>
#include "hlvector.hpp"
#include <string>
#pragma once

#ifndef __HSTRING_H
   #define __HSTRING_H
namespace hst
{

class string;

typedef hst::vector<hst::string> stringlist;

/*! \class  string
    \brief  String class

	Some little big string class. I wrote it for my own purpose.
*/
class string: public std::string
{
 private:
	     /*! It compares symbols it by priority on russian language.
			 The biggest are big latin symbols
             then go other symbols, like small latin symbols, locales and numerics.
			 \param[in] c1 first char
			 \param[in] c2 second char
			 \return false if c1 is bigger than c2 by priority or code
		 */
		 bool cmpchar(char c1,char c2) const;
 public:

         string();                               //!< Default constructor.
	     /*! Creates a string by copying the string, addressed by pointer.
		     \param[in] _p address of string
		 */
		 string(const char* _p);
		 /*! Copies a len symbols from p, an adds a 0-symbol.
		     \param[in] _p    address of symbols
			 \param[in] len  length of sequence
		 */
		 string(const char* _p,long len);
	     /*! Copy constructor.
		     \param[in] o other string
		 */
		 string(const string & o);
		/*! Copy constructor.
		     \param[in] o other string
		 */
		 string(const std::string & o);
	     ~string();                              //!< Destructor, which cleans a string.
		 //Methods
		 bool empty() const;                      //!< Detects emptyness of string.
		 void print() const;                      //!< Prints string to a screen
		 /*! Comparison of two strings
			 \param[in] o string
			 \return whether they are equal
		  */
		 bool operator==(const hst::string & o) const;
		 /*! Comparison of two strings
			 \param[in] o string
			 \return whether they are equal
		  */
		 bool operator!=(const hst::string & o) const;
		 /*! Comparison of two strings
			 \param[in] o string
			 \return whether they are equal
		  */
		 bool operator==(const char *  o) const;
		 /*! Comparison of two strings
			 \param[in] o string
			 \return whether they are equal
		  */
		 bool operator!=(const char * o) const;
		 /*! Removes a symbol at position
             \param[in] i position of symbol
		 */
		 void remove(long i);
		 /*! Reads a string line from file
		     \param[in] pf pointer to file descriptor
		 */
		 void readLine(FILE** pf);
		 /*! Splits a string by delimiter, returning a list of strings
		     \param[in] delimiter delimiter char, removing empty strings between two delimiters
			 \return list of strings as a result of splitting
		 */
		 hst::stringlist split(char delimiter) const;
		 /*! Splits a string by delimiters, returning a list of strings
		     \param[in] delimiters delimiter char, removing empty strings between two delimiters
			 \return list of strings as a result of splitting
		 */
		 hst::stringlist split(const char * delimiters) const;
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if bigger.
		 */
		 bool operator>(const string & o) const;
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if bigger or equal
		 */
		 bool operator>=(const string & o) const;
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if lesser.
		 */
		 bool operator<(const string & o) const;
		 /*!  Compares two strings, firstly by length, secondly symbol-wise
		      \sa string::cmpchar
			  \param[in] o other string
			  \return true if lesser or equal.
		 */
		 bool operator<=(const string & o) const;
		 /*! Places a character to end of string
		     \param[in] c character
		     \return self-reference
		 */
		 string & operator<<(char c);
		 /*! Places one string to end of other
		     \param[in] o string
			 \return self-reference
		*/
		 string & operator<<(const string & o);
		 /*! Inserts a character into a position
		     \param[in] c char to be inserted
			 \param[in] i index of place
			 \return self-reference
		 */
		 string & insert(char c,long i);
		 /*! Inserts a string into a position
		     \param[in] o string to be inserted
			 \param[in] i index of place
			 \return self-reference
		 */
		 string & insert(const string & o,long i);
		 /*! Removes rlen symbols, beginning from beg
		     \param[in] beg  beginning index
			 \param[in] rlen length of range
			 \return self-reference
		 */
		 string & removeRange(long beg,long rlen);
	     /*! Concates two strings
		     \param[in] o other string
			 \return concatenated symbols
		 */
		 string  operator+(const string & o) const;
		 /*! Concates two strings
		     \param[in] o other string
			 \return concatenated symbols
		 */
		 string  operator+(const char * o) const;
		 /*! Concates two strings
		     \param[in] o other string
			 \return concatenated symbols
		 */
		 string  operator+(const std::string & o) const;
		 /*! Returns last i characters
		     \param[in] i amount of characters to be extracted
			 \return string of last characters
		 */
		 string  getLastCharacters(long i) const;
		 /*! Extracts an extension for file, but
		     if no extension is specified it returns an empty string
			 \return extension
		 */
		 string  getExtension() const;
		 /*! Adds a file extension to string
		     \param[in] newext new extension
     	 */
		 void  addExtension(const string & newext);
		 /*! Changes a file extension to string
		     \param[in] newext new extension
     	 */
		 void  changeExtension(const string & newext);
		 /*! Removes file extension or does nothing if no extension found
		 */
		 void  removeExtension();
		 /*! Returns a substring of string
		     \param[in] beg index beginning char
			 \param[in] len length of substring
			 \return substring
		 */
		 string  subString(long beg,long len) const;
		 /*! Returns a right part of string. If len bigger length, all string will be returned
		     \param[in] len length of substring
			 \return substring
		 */
		 string  getRightPart(long len);
		 /*! Returns a left part of string. If len bigger length, all string will be returned
		     \param[in] len length of substring
			 \return substring
		 */
		 string  getLeftPart(long len);
		 /*! Count occurences of string sstr int this string.
		     \param[in] sstr string, which occurences will be counted
			 \return -1 if no occurences, otherwise amount of occurences
		 */
		 long getOccurences(const string & sstr);
		 /*! Returns a position of substring.
		    \param[in] sstr  searching string
			\param[in] omax  searching occurence of string.0 - for first.
		    \return -1  if not found, otherwise - index
		 */
		 long getOccurence(const string & sstr,long omax=0);
		 /*! Returns index of last occurence of one string from this string
		     \param[in] sstr substring to be removed
			 \return -1 if not found, otherwise index of substring
		 */
		 long getLastOccurence(const string & sstr);
		 /*! Removes last occurence of one string from this string
		     \param[in] sstr substring to be removed
			 \param[in] omax index of occurence
		 */
		 void removeOccurence(const string & sstr,long omax=0);
		 /*! Removes last occurence of one string from this string
		     \param[in] sstr substring to be removed
		 */
		 void removeLastOccurence(const string & sstr);
		 /*! Removes all occurences of one string from this string
		     \param[in] sstr substring to be removed
		 */
		 void removeAllOccurences(const string & sstr);
		 /*! Replaces following occurence of one string to other. If this occurence
		     isn't found, nothing will be done
		     \param[in] sstr string to be replaced
			 \param[in] to   replacing string
			 \param[in] omax index of occurence
		 */
         void replaceOccurence(const string & sstr,const string & to,long omax=0);
		 /*! Replaces last occurence of one string to other
		     \param[in] sstr string to be replaced
			 \param[in] to   replacing string
		 */
		 void replaceLastOccurence(const string & sstr,const string & to);
		 /*! Replaces all occurences of one string to other
		     \param[in] sstr string to be replaced
			 \param[in] to   replacing string
		 */
		 void replaceAllOccurences(const string & sstr,const string & to);
		 //Trivial
		 void removeSpaces();                                  //!< Removes all spaces from a string
		 void trimSpaces();                                    //!< Removes a spaces from begin and end of string
		 //Getters
	     char * data() const;                 //!< Returns a pointer to array
		 /*! Converts number to string.
		 *   Supports 8,10,16 radixes.
		 *   \param[in] a       numeric to convert
		 *   \param[in] radix   Conversion radix
		 */
		 static string number(int a,int radix=10);
		 /*! Converts pointer definition from string to long
		     \param[in]  str  string to query
		     \param[out] addr  integral address
		     \return    true if success
		 */
		 static bool  queryPointer(const string & str, long * addr);
		 /*! String conversion to integral type. Uses sscanf() to convert
		     \param[in] str string to be converted
			 \return    intergal conversion
		 */
		 static int  toInt(const string & str);
		 /*! String conversion to float type. Uses sscanf() to convert
		     \param[in] str string to be converted
			 \return    intergal conversion
		 */
		 static float toFloat(const string & str);
		 /*! String conversion to double type. Uses sscanf() to convert
		     \param[in] str string to be converted
			 \return    intergal conversion
		 */
		 static double toDouble(const string & str);
};

};
/*! Convenience typedef
*/
typedef hst::string hString;
/*! Joints a list into a string with separator sep.
	For example join({'a', 'b', 'c'}) will become  'a,b,c'
	\param[in] list list of strings
	\param[in] sep  separator
 */
hst::string join(const hst::stringlist list, const hst::string & sep);

#endif
