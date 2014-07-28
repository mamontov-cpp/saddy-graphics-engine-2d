/*! \file customschema.h
	\author HiddenSeeker

	A custom schema for creating custom items for tree
 */
#pragma once
#include "../schema/schema.h"
#include "../../resource/resource.h"

namespace sad
{

namespace db
{
	
namespace custom
{
	
class Schema: public sad::db::schema::Schema
{
public:
	/*! A default constructor for schema 
	 */
	Schema();
	/*! A destructor for schema 
	 */
	~Schema();
protected:
	/*! A linked resource item for a schema
	 */
	sad::String m_tree_item;
};

}

}
	
}
