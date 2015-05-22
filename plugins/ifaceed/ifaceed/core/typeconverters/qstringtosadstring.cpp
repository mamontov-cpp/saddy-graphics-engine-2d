#include "core/typeconverters/qstringtosadstring.h"

#include "../../qstdstring.h"

void core::typeconverters::QStringToSadString::convert(void * source, void * dest)
{
	QString * src = reinterpret_cast<QString*>(source);
	sad::String * dst = reinterpret_cast<sad::String*>(dest);

	*dst = sad::String(Q2STDSTRING(*src));
}

core::typeconverters::QStringToSadString::~QStringToSadString()
{
	
}
