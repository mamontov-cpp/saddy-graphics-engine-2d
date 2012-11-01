#include "marshal/variant.h"

sad::VariantVector::~VariantVector()
{
	for (unsigned int i=0;i<count();i++)
		delete (*this)[i];
}
