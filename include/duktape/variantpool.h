/*! \file variantpool.h
	\author HiddenSeeker

	Describes a pool for variants, which are identified by a very large number,
	stored as string.
 */
#pragma once
#include "../3rdparty/ttmath/ttmath.h"

#include "../db/dbvariant.h"

#include "../sadptrhash.h"
#include "../sadstring.h"

namespace sad
{

namespace duktape
{

/*! Describes a pool of variants, stored in context
 */
class VariantPool
{
public:

protected:
	/*! Maximal id of pool
	 */
	ttmath::UInt<5> m_maxid;
	/*! Pool of variants
	 */
	sad::PtrHash<sad::String, sad::db::Variant> m_pool;
};

}

}
