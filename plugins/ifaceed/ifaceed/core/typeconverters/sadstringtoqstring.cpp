#include "core/typeconverters/sadstringtoqstring.h"
#include "object.h"

#include "../../qstdstring.h"

DECLARE_COMMON_TYPE(QString)

void core::typeconverters::SadStringToQString::convert(void * source, void * dest)
{
    sad::String * src = reinterpret_cast<sad::String*>(source);
    QString * dst = reinterpret_cast<QString*>(dest);

    *dst = STD2QSTRING(*src);
}

core::typeconverters::SadStringToQString::~SadStringToQString()
{
    
}
