#include "db/dbconversiontable.h"

sad::db::AbstractTypeConverter::~AbstractTypeConverter()
{
	
}

sad::db::ConversionTable::ConversionTable()
{
	declareImplicit<short, unsigned short>();
	declareImplicit<short, unsigned int>();
	declareImplicit<short, unsigned long>();
	declareImplicit<short, unsigned long long>();
	declareImplicit<short, int>();
	declareImplicit<short, long>();
	declareImplicit<short, long long>();

	declareImplicit<int, unsigned int>();
	declareImplicit<int, unsigned long>();
	declareImplicit<int, unsigned long long>();
	declareImplicit<int, long>();
	declareImplicit<int, long long>();
	
	declareImplicit<long, unsigned long>();
	declareImplicit<long, unsigned long long>();
	declareImplicit<long, long long>();

	declareImplicit<long long, unsigned long long>();

	declareImplicit<float, double>();
}

void sad::db::ConversionTable::add(
	const sad::String & from, 
	const sad::String & to, 
	sad::db::AbstractTypeConverter * c
)
{
	sad::PtrHash<sad::String, sad::db::AbstractTypeConverter> * h = NULL;
	if (m_converters.contains(from) == false)
	{
		m_converters.insert(from, sad::PtrHash<sad::String, sad::db::AbstractTypeConverter>());
	}
	h = &(m_converters[from]);
	if (h->contains(to))
	{
		delete (*h)[to];
	}
	h->insert(to, c);
}

sad::db::AbstractTypeConverter *   sad::db::ConversionTable::converter(
		const sad::String & from, 
		const sad::String & to
)
{
	sad::db::AbstractTypeConverter * result = NULL;
	if (m_converters.contains(from))
	{
		sad::PtrHash<sad::String, sad::db::AbstractTypeConverter> & h = m_converters[from];
		if (h.contains(to))
		{
			result = h[to];
		}
	}
	return result;
}

sad::db::ConversionTable * sad::db::ConversionTable::ref()
{
	if (sad::db::ConversionTable::m_instance == NULL)
	{
		sad::db::ConversionTable::m_instance = new sad::db::ConversionTable();
		atexit(sad::db::ConversionTable::freeInstance);
	}
	return sad::db::ConversionTable::m_instance;
}


void sad::db::ConversionTable::freeInstance()
{
	delete sad::db::ConversionTable::m_instance;
}


sad::db::ConversionTable* sad::db::ConversionTable::m_instance = NULL;
