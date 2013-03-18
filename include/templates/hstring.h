/*! \file    hstring.h
    \author  HiddenSeeker

	\brief   Definition of string.

	This file contains a definition of string.
*/
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#pragma once

#ifndef __HSTRING_H
   #define __HSTRING_H
namespace hst
{

class stringlist;

/*! \class  string
    \brief  String class

	Some little big string class. I wrote it for my own purpose.
*/
class string
{
 friend class hst::stringlist;
 private:
	     char * p;                                //!< Pointer to array of char.
	     long    len;                             //!< Length of string.
		 void __clean();                          //!< Not memory clean, just setting a pointer and length to 0.
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
	     ~string();                              //!< Destructor, which cleans a string.
		 //Methods
		 bool empty() const;                      //!< Detects emptyness of string.
		 void print() const;                      //!< Prints string to a screen
		 void clear();                            //!< Cleans a string;
		 /*! Removes a symbol at position
             \param[in] i position of symbol
		 */
		 void remove(long i);
         /*! Removes odd null characters at end
		 */
		 void removeOddNullsAtEnd();
		 /*! Reads a string line from file
		     \param[in] pf pointer to file descriptor
		 */
		 void readLine(FILE** pf);
		 /*! Splits a string by delimiter, returning a list of strings
		     \param[in] delimiter delimiter char, removing empty strings between two delimiters
			 \return list of strings as a result of splitting
		 */
		 class hst::stringlist split(char delimiter) const;
		 /*! Splits a string by delimiters, returning a list of strings
		     \param[in] delimiters delimiter char, removing empty strings between two delimiters
			 \return list of strings as a result of splitting
		 */
		 class hst::stringlist split(const char * delimiters) const;
	     /*! Splits a string to words and creates a list from it, trying to match length of string.
		     If one string does not match following length, it WON'T be splitted
			 \param[in] maxlen maximum length of string
			 \return list of strings
		 */
		 class hst::stringlist splitByLength(int maxlen) const;
		 /*! Assignment operator overload
		     \param[in] o other string
			 \return self-reference
		 */
		 string & operator=(const string & o);
		 /*!   Compares fully string and string, addressed by pointer
               \param[in] o pointer to string
               \return true if equal
		 */
		 bool operator==(const char * o) const;
		 /*!   Compares fully string and string, addressed by pointer
               \param[in] o pointer to string
               \return true if not equal
		 */
		 bool operator!=(const char * o) const;
		 /*!   Compares two strings
               \param[in] o pointer to string
               \return true if equal
		 */
		 bool operator==(const string & o) const;
		 /*!   Compares two strings
               \param[in] o pointer to string
               \return true if  not equal
		 */
		 bool operator!=(const string & o) const;
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
		 /*! Returns a reference to symbol, from position i
		      \param[in] i index of symbol
			  \return reference symbol. If index out of bound, it will cause undefined behaviour
		 */
		 char    & operator[](long i);
		 /*! Returns a symbol, from position i
		      \param[in] i index of symbol
			  \return 0 if index is broken, or symbol.
		 */
		 char   operator[](long i) const;
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
         /*! Returns a position of last occurence of substring,
		     bounded with string beg from left and string end from right.
			 \param[in]   beg  bounding string from left
			 \param[in]   end  bounding string from right
			 \param[out]  begi index of first symbol
			 \param[out]  rlen length of substring
		 */
		 void getLastOccurence(const char * beg, const char * end,long * begi,long * rlen);
		 /*! Removes last occurence of substring, bounded with string beg from left and string
			 end from right.
			 \param[in] beg bounding string from left
			 \param[in] end bounding string from right
		 */
		 void removeLastOccurence(const char * beg, const char * end);
		 /*! Replaces last occurence of substring, bounded with string beg from left and string
			 end from right.
			 \param[in] beg bounding string from left
			 \param[in] end bounding string from right
			 \param[in] to  string to replace to
		 */
		 void replaceLastOccurence(const char * beg, const char * end,const string & to);
         /*! Counts occurences of substrings, bounded with string beg from left and string
			 end from right.
			 \param[in] beg bounding string from left
			 \param[in] end bounding string from right
		 */
		 long getOccurences(const char * beg, const char * end);
		 /*! Removes all occurences of substrings, bounded with string beg from left and string
			 end from right.
			 \param[in] beg bounding string from left
			 \param[in] end bounding string from right
		 */
		 void removeAllOccurences(const char * beg,const char * end);
		 //Trivial
		 void removeSpaces();                                  //!< Removes all spaces from a string
		 void trimSpaces();                                    //!< Removes a spaces from begin and end of string
		 /*! Replace using own callback function
		     \param[in] beg       Inner begin sequence
		     \param[in] end       Inner end  sequence
		     \param[in] repfunc   replace function, which has a parameters - me (always will be called with *this), beg - begin sequence, len - length of sequence.
		     \return              Returns new index.
		 */
		 void   oc_Replace_Callback(const char * beg, const char * end, long (*repfunc)(string & me,long beg, long len));
		 /*! Strict replace using own callback function. Strict means, that all
             callbacks should be called with largest correct sequence.
             Exactly, callback will be called with string like {{}}, but not with {{}, because this
             is not correct sequence. Sounds kinda lispic, should it?
		     \param[in] beg - inner begin sequence
		     \param[in] end - inner end  sequence
		     \param[in] repfunc - replace function, with parameters - me (always calls with *this), beg - begin sequence, len - length of sequence. It should return new index to start from.
		     \return 1 if all correct parsed, 0 if uncorrect
		 */
		 int   oc_strictReplace_Callback(const char * beg, const char * end, long (*repfunc)(string & me,long beg, long len));
#ifdef H_USE_LOW_LEVEL_IO
		 /*!  Reads a line from a file, with descriptor fd, using CRT low-level functions
			  \param[in] fd file descriptor
		 */
		 void readLine_lowLevel(int fd);
         /*! Writes a string to a file, using CRT low-level functions
		     \param[in] fn name of file to write to
		 */
		 void writeToFile_lowLevel(const string & fn);
		 /*! Reads a string from a file, using CRT low-level functions
		     \param[in] fn name of file to read from
		 */
		 void readFile_lowLevel(const string & fn);
#endif
		 //Getters
	     long length() const;                 //!< Returns a length of string
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

#endif
