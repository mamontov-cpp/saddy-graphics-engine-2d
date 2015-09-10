#include "mock4.h"

#include <db/dbfield.h>
#include <db/dbmethodpair.h>
#include "db/save.h"
#include "db/load.h"


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(Mock4);
DECLARE_SOBJ_INHERITANCE(Mock4, sad::Object)

Mock4::Mock4()
{
	m_schema.addParent(sad::db::Object::basicSchema());
	sad::db::Property* p = new sad::db::Field<Mock4, int>(&Mock4::m_id); 
	p->makeNonRequiredWithDefaultValue(new sad::db::Variant(22));
	m_schema.add("prop", p);
}

sad::db::schema::Schema * Mock4::schema() const
{
	return &(const_cast<Mock4*>(this)->m_schema);	
}
