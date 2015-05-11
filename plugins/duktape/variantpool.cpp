#include "duktape/variantpool.h"

sad::duktape::VariantPool::VariantPool()
{
	m_maxid.SetZero();
}

sad::duktape::VariantPool::~VariantPool()
{
	
}

sad::duktape::VariantPool::VariantPool(const sad::duktape::VariantPool& p)
{
	copyState(p);
}

sad::duktape::VariantPool& sad::duktape::VariantPool::operator=(const sad::duktape::VariantPool& p)
{
	free();
	copyState(p);
	return *this;
}

sad::String sad::duktape::VariantPool::insert(sad::db::Variant* v)
{
	assert(v);
	std::string id;
	m_maxid.ToString(id, 10);
	m_maxid.AddOne();
	m_pool.insert(id, v);
	return id;
}

sad::db::Variant* sad::duktape::VariantPool::get(const sad::String& key)
{
	if (m_pool.contains(key))
	{
		return m_pool[key];
	}
	return NULL;
}

void sad::duktape::VariantPool::free()
{
	for(sad::PtrHash<sad::String, sad::db::Variant>::iterator it = m_pool.begin();
		it != m_pool.end();
		++it)
	{
		delete it.value();
	}
	m_pool.clear();
	m_maxid.SetZero();
}

void sad::duktape::VariantPool::copyState(const sad::duktape::VariantPool& p)
{
	m_maxid = p.m_maxid;
	for(sad::PtrHash<sad::String, sad::db::Variant>::const_iterator it = p.m_pool.const_begin();
		it != p.m_pool.const_end();
		++it)
	{
		m_pool.insert(it.key(), new sad::db::Variant(*(it.value())));
	}
}