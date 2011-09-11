#include "typeindex.h"

static int m_typesystem_registered_types_amount=0;

int typeindex::getIndex()
{
 return m_typesystem_registered_types_amount++;
}
