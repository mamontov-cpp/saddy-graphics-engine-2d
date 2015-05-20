#include "refcountable.h"

sad::RefCountable::RefCountable() : m_references(0)
{
	
}

void sad::RefCountable::addRef()  
{
	++m_references;
}

void sad::RefCountable::delRef()  
{
	--m_references; 
	if (m_references == 0) 
		delete this; 
}

int sad::RefCountable::refsCount() const
{
    return m_references;
}

sad::RefCountable::~RefCountable() {}
