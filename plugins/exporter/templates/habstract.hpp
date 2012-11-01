/*! \file   habstract.hpp
    \author HiddenSeeker

	\brief Contains abstract type definitions

	This file contains a definition of types, which are not directly used.
*/
#pragma once
namespace hst
{
/*! Definition of sort direction. Used in
    hmap,hsmap and hprecmap
*/
enum sortorder
{
          Ascending=0,    //!< Defines ascending  sorting order
          Descending=1,   //!< Defines descending sorting order
};

}
/*! Convenience typedef
*/
typedef   hst::sortorder hSortOrder;
