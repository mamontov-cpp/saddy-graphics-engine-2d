#include "core/typeconverters/qstringtosadstring.h"


void core::typeconverters::QStringToSadString::convert(void * source, void * dest)
{
	QString * src = reinterpret_cast<QString*>(source);
	sad::String * dst = reinterpret_cast<sad::String*>(dest);

	*dst = sad::String(src->toStdString());
}

core::typeconverters::QStringToSadString::~QStringToSadString()
{
	
}
