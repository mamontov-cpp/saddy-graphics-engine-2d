/*! \file common.hpp
    \author HiddenSeeker

	\brief defines common works

	Defines common search and sort routines
*/
#include "hdeque.hpp"
#include "hlist.hpp"
#pragma once


/*! Searches object in array
    \param[in] v array
	\param[in] me object
	\return position, -1 if not found
*/
template< template<typename> class ARR, typename T > long findInArray(const class  ARR<T> &  v ,const T & me);

/*! Searches object in list
    \param[in] v list
	\param[in] me object
	\return position, -1 if not found
*/
template<class T> inline long findInList(const class  hlist<T> &  v ,const T & me);

/*! Searches object in deque
    \param[in] v deque
	\param[in] me object
	\return position, -1 if not found
*/
template<class T> inline long findInDeque(const class  hdeque<T> &  v ,const T & me);

/*! Adds an object,with sorting, with checking unique sorting
    \param[in] v array
	\param[in] me object
	
*/
template<class T> void addAscendingUnique(class hdeque<T> & v, const T & me);

#ifndef HI_COMMON_ALGORYTHM_HPP
		#define HI_COMMON_ALGORYTHM_HPP
		#include "common_src.hpp"
#endif