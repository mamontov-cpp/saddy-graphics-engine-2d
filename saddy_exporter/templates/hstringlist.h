/*! \file    hstringlist.h
    \author  HiddenSeeker
	
	\brief   Definition of string list.
	
	This file contains a definition of string list.
*/
#pragma once
#include "hstring.h"

namespace hst
{
class string;
/*! \class  stringlist
    \brief  String  list class
	
	Some little big string list class. 
*/
class  stringlist
{
 friend class string;
 private:
     string * p;                           //!< Pointer to array of strings.
	 long len;                             //!< Length of array.
	 void __clean();                       //!< Not memory clean, just setting a pointer and length to 0.
 public:	 
	 stringlist();                        //!< Default constructor of empty list.
	 /*! Copy constructor.
	    \param[in] o other list
	 */
	 stringlist(const stringlist & o);   
	 ~stringlist();                       //!< Simple destructor, which removes all strings and frees memory.
	 //Methods
	 /*! Removes a string from a list
	     \param[in] i index of string, which will be removed
	 */
	 void remove(long i);                  
 	 //Overloads
	 /*! Assignment operator overload
	     \param[in] o other list of strings
		 \return self-reference
	 */
	 stringlist & operator=(const stringlist & o);  
	 /*! Adds a string to list
	     \param[in] h string to be added
		 \return self-reference
	*/
	 stringlist & operator<<(const string & h);     
	 /*! Adds a string to list
	     \param[in] p pointer, addressing string to be added
		 \return self-reference
	*/
	 stringlist & operator<<(char * p);              
	 /*!  Returns a reference to string from a position. If position is incorrect,
	      it will cause undefined behaviour
		  \param[in] i index
		  \return reference to a string
	 */
	 string & operator[](long i);                    
	 /*!  Returns a constant reference to string from a position. 
	      If position is incorrect, it will cause undefined behaviour
		  \param[in] i index
		  \return constant reference to a string
	 */
	 const string & operator[](long i) const;        
	 void print();                                    //!< Prints a string
	 //Getters
	 long length() const;                             //!< Returns a length of string
	 string * data() const;                          //!< Returns a pointer to array

};

}

/*! Convenience typedef
*/
typedef hst::stringlist hStringList;
