#include "dukpp-03/context.h"

void sad::dukpp03::pushVariant(sad::dukpp03::BasicContext* ctx, const sad::db::Variant& v)
{
    sad::String typeName = v.typeName();
#define  DUK_IF_PUSH(TYPE)   if (typeName == #TYPE) {  ::dukpp03::PushValue< DUKPP03_TYPE(TYPE), sad::dukpp03::BasicContext>::perform(ctx, v.get<DUKPP03_TYPE(TYPE)>().value()); return;  }
    DUK_IF_PUSH(bool)
    DUK_IF_PUSH(char)
    DUK_IF_PUSH(unsigned char)
    DUK_IF_PUSH(short)
    DUK_IF_PUSH(unsigned short)
    DUK_IF_PUSH(int)
    DUK_IF_PUSH(unsigned int)
    DUK_IF_PUSH(long)
    DUK_IF_PUSH(unsigned long)
    DUK_IF_PUSH(long long)
    DUK_IF_PUSH(unsigned long long)
    DUK_IF_PUSH(float)
    DUK_IF_PUSH(double)
    DUK_IF_PUSH(long double)
    DUK_IF_PUSH(std::string)
    DUK_IF_PUSH(sad::String)
#undef DUK_IF_PUSH
    ctx->pushUntypedVariant(v.typeName(), new sad::db::Variant(v));
}