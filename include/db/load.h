/*! \file db/load,h
	\author HiddenSeeker
	
	Describes a load operations for a type
 */
#pragma once
#include "dberror.h"
#include "dbtypename.h"
#include "3rdparty/picojson/valuetotype.h"
#include "../object.h"

namespace sad
{

namespace db
{

/*! Load a value of specified type
	\param[in] ptr a value to be saved
	\param[in] v a special value, from which we should load stuff
	\return result
 */
template<
	typename _Type
>
bool Load<_Type>::perform(void * ptr, const picojson::value & v)
{
	if (!ptr)
	{
		throw sad::db::InvalidPointer();
	}
	if (sad::db::TypeName<_Type>::isSadObject())
	{
		return reinterpret_cast<sad::Object *>(ptr)->load(v);
	}
	throw sad::db::NotImplemented("sad::db::Load<_Type>::perform");
	return false;
}

/*! Declares template load for specified type
 */
#define DECLARE_DB_LOAD_FOR_TYPE( TYPE )                         \
template<>                                                       \
class Load< TYPE >                                               \
{                                                                \
public:                                                          \
static bool perform(void * ptr, const picojson::value & v)       \
{                                                                \
	if (!ptr)                                                    \
		throw sad::db::InvalidPointer();                         \
	sad::Maybe< TYPE >  cast = picojson::to_type< TYPE >(v);     \
	if (cast.exists())                                           \
	{                                                            \
		*( TYPE *)ptr = cast.value();                            \
	}                                                            \
	return cast.exists();                                        \
}                                                                \
};                

DECLARE_DB_LOAD_FOR_TYPE(bool)
DECLARE_DB_LOAD_FOR_TYPE(char)
DECLARE_DB_LOAD_FOR_TYPE(signed char)
DECLARE_DB_LOAD_FOR_TYPE(unsigned char)
DECLARE_DB_LOAD_FOR_TYPE(short)
DECLARE_DB_LOAD_FOR_TYPE(unsigned short)
DECLARE_DB_LOAD_FOR_TYPE(int)
DECLARE_DB_LOAD_FOR_TYPE(unsigned int)
DECLARE_DB_LOAD_FOR_TYPE(long)
DECLARE_DB_LOAD_FOR_TYPE(unsigned long)
DECLARE_DB_LOAD_FOR_TYPE(long long)
DECLARE_DB_LOAD_FOR_TYPE(unsigned long long)
DECLARE_DB_LOAD_FOR_TYPE(float)
DECLARE_DB_LOAD_FOR_TYPE(double)
DECLARE_DB_LOAD_FOR_TYPE(std::string)
DECLARE_DB_LOAD_FOR_TYPE(sad::String)
DECLARE_DB_LOAD_FOR_TYPE(sad::Point2D)
DECLARE_DB_LOAD_FOR_TYPE(sad::Point2I)
DECLARE_DB_LOAD_FOR_TYPE(sad::Rect2D)
DECLARE_DB_LOAD_FOR_TYPE(sad::Color)
DECLARE_DB_LOAD_FOR_TYPE(sad::AColor)
DECLARE_DB_LOAD_FOR_TYPE(sad::Size2D)
DECLARE_DB_LOAD_FOR_TYPE(sad::Size2I)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<sad::Vector<sad::AColor> >)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<sad::Point2D>)
DECLARE_DB_LOAD_FOR_TYPE(sad::dialogue::Phrase)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<sad::dialogue::Phrase*>)

}

}
