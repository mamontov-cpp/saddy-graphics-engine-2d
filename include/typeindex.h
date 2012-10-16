/*! \file   typeindex.h
    \author HiddenSeeker
	
	Declares a system for autogenerating type IDs.
*/
#pragma once
#include <vector>
namespace  typeindex {
  /*! Returns new ID */
  int getIndex();
}

/*! Generates declaration for nodes
*/
#define   SAD_NODE                                 \
public:                                            \
         static int ID;                            \
		 static bool             InheritanceVectorInitted; \
		 static std::vector<int> InheritedFromIDs;         \
         virtual int type() const;                         \
		 virtual const std::vector<int> & inheritedFrom() const; 

/*! Generates an id for type
*/
#define  SAD_DECLARE_BASIC_NODE(TYPE)                     \
int TYPE :: ID=typeindex::getIndex();                     \
int TYPE :: type() const { return TYPE :: ID;}            \
bool  TYPE :: InheritanceVectorInitted = false;           \
std::vector<int> TYPE :: InheritedFromIDs;                \
const std::vector<int> & TYPE ::inheritedFrom() const     \
{                                                         \
  if (! TYPE :: InheritanceVectorInitted)                 \
  {                                                       \
      TYPE :: InheritanceVectorInitted=true;              \
  }                                                       \
  return TYPE :: InheritedFromIDs;                        \
}

/*! Generates a typeid for type and his basic type        
*/
#define  SAD_DECLARE(TYPE,BASICTYPE)                     \
int TYPE :: ID=typeindex::getIndex();                     \
int TYPE :: type() const { return TYPE :: ID;}            \
bool  TYPE :: InheritanceVectorInitted = false;           \
std::vector<int> TYPE :: InheritedFromIDs;                \
const std::vector<int> & TYPE ::inheritedFrom() const     \
{                                                         \
  if (! TYPE :: InheritanceVectorInitted)                 \
  {                                                       \
      TYPE :: InheritedFromIDs = BASICTYPE :: InheritedFromIDs; \
	  TYPE :: InheritedFromIDs.push_back(BASICTYPE :: ID); \
      TYPE :: InheritanceVectorInitted=true;              \
  }                                                       \
  return TYPE :: InheritedFromIDs;                        \
}
