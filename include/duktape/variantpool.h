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
	/*! Creates default variant pool
	 */
	VariantPool();
	/*! Frees variant pool data
	 */
	~VariantPool();
	/*! Copies state from other pool
		\param[in] p pool
	 */
	VariantPool(const sad::duktape::VariantPool& p);
	/*! Copies state from other pool
		\param[in] p pool
		\return self-reference
	 */
	VariantPool& operator=(const VariantPool& p);
	/*! Inserts new variant to a pool
		\param[in] v variant
		\return a string, which identifies a variant
	 */
	sad::String insert(sad::db::Variant* v);
	/*! Fetches variant by it's key. NULL returned if not found
		\param[in] key key, identifying variant
		\return variant or NULL if not found
	 */
	sad::db::Variant* get(const sad::String& key);
	/*! Destroys all variant in pool
	 */
	void free();
protected:
	/*! Maximal id of pool
	 */
	ttmath::UInt<5> m_maxid;
	/*! Pool of variants
	 */
	sad::PtrHash<sad::String, sad::db::Variant> m_pool;
	/*! Copies state from other pool
		\param[in] p pool
	 */
	void copyState(const sad::duktape::VariantPool& p);
};

}

}
