#pragma once
#include "pushvalue.h"
#include "../db/save.h"
#include "../db/load.h"

namespace sad
{

namespace duktape
{

template<
	typename T
>
sad::String sad::duktape::PushValue<T>::perform(sad::duktape::Context* ctx, const T& v, bool persistent)
{
	if (persistent)
	{
		return ctx->pushPersistentVariant(new sad::db::Variant(v));
	}
	return ctx->pushVariant(new sad::db::Variant(v));
}


}

}
