/*! \file db/save,h
	\author HiddenSeeker
	
	Describes a save operations for a type
 */
#pragma once
#include "dberror.h"
#include "dbtypename.h"
#include "../sadstring.h"
#include "../sadrect.h"
#include "../sadcolor.h"
#include "../sadsize.h"

namespace sad
{

namespace db
{
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
template<typename _Type>
inline picojson::value Save<_Type>::perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	if (sad::db::TypeName<_Type>::isSadObject())
	{
		picojson::value v;
		reinterpret_cast<sad::Object *>(ptr)->save(v);
		return v;
	}
	throw sad::db::NotImplemented("sad::db::Save<_Type>::perform");
	return picojson::value();
}

/*! Specification for saving bool values
 */
template<>
class Save<bool>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	return picojson::value(*(bool*)ptr);
}

};

/*! Generates specification for saving to type via casting pointer to it
 */
#define SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE( TYPENAME ) \
template<>                                                     \
class Save<  TYPENAME >                                        \
{                                                              \
public:                                                        \
static inline picojson::value perform(void * ptr)              \
{                                                              \
	if (!ptr)                                                  \
		throw sad::db::InvalidPointer();                       \
	return picojson::value((double)(*( TYPENAME *)ptr));       \
}                                                              \
};

SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(char)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(signed char)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned char)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(short)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned short)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(int)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned int)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(long long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned long long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(float)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(double)

/*! Specification for saving string values
 */
template<>
class Save<std::string>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	return picojson::value(*(std::string*)ptr);
}

};
/*! Specification for saving string values
 */
template<>
class Save<sad::String>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	return picojson::value((sad::String)(*(sad::String*)ptr));
}

};

/*! Specification for saving point values
 */
template<>
class Save<sad::Point2D>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Point2D & p = *((sad::Point2D *)ptr);
	picojson::value v(picojson::object_type, false);
	v.insert("x", picojson::value((double)(p.x())));
	v.insert("y", picojson::value((double)(p.y())));
	return v;
}

};

/*! Specification for saving point values
 */
template<>
class Save<sad::Point2I>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Point2I & p = *((sad::Point2I *)ptr);
	picojson::value v(picojson::object_type, false);
	v.insert("x", picojson::value((double)(p.x())));
	v.insert("y", picojson::value((double)(p.y())));
	return v;
}

};

/*! Specification for saving rectangle values
 */
template<>
class Save<sad::Rect2D>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Rect2D & p = *((sad::Rect2D *)ptr);
	picojson::value v(picojson::object_type, false);
	v.insert("p1", sad::db::Save<sad::Point2D>::perform(&(p[0])));
	v.insert("p2", sad::db::Save<sad::Point2D>::perform(&(p[1])));
	v.insert("p3", sad::db::Save<sad::Point2D>::perform(&(p[2])));
	v.insert("p4", sad::db::Save<sad::Point2D>::perform(&(p[3])));
	return v;
}

};

/*! Specification for saving color values
 */
template<>
class Save<sad::Color>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Color & p = *((sad::Color *)ptr);
	picojson::value v(picojson::object_type, false);
	v.insert("r", picojson::value((double)(p.r())));
	v.insert("g", picojson::value((double)(p.g())));
	v.insert("b", picojson::value((double)(p.b())));
	return v;
}

};

/*! Specification for saving values of type color with alpha-channel
 */
template<>
class Save<sad::AColor>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::AColor & p = *((sad::AColor *)ptr);
	picojson::value v(picojson::object_type, false);
	v.insert("r", picojson::value((double)(p.r())));
	v.insert("g", picojson::value((double)(p.g())));
	v.insert("b", picojson::value((double)(p.b())));
	v.insert("a", picojson::value((double)(p.a())));
	return v;
}

};

/*! Specification for saving values of type vector of vectors of color with alpha-channel
 */
template<>
class Save<sad::Vector<sad::Vector<sad::AColor> > >
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Vector<sad::Vector<sad::AColor> > & p = *((sad::Vector<sad::Vector<sad::AColor> > *)ptr);
	picojson::value v(picojson::array_type, false);
	for(size_t i = 0; i < p.size(); i++)
	{
		picojson::value tmp(picojson::array_type, false);
		for(size_t j = 0; j < p[i].size(); j++)
		{
			tmp.push_back(sad::db::Save<sad::AColor>::perform(&(p[i][j])));
		}
		v.push_back(tmp);
	}
	return v;
}

};

/*! Specification for saving size values
 */
template<>
class Save<sad::Size2D>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Size2D & p = *((sad::Size2D *)ptr);
	picojson::value v(picojson::object_type, false);
	v.insert("width", picojson::value((double)(p.Width)));
	v.insert("height", picojson::value((double)(p.Height)));
	return v;
}

};

/*! Specification for saving size values
 */
template<>
class Save<sad::Size2I>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Size2I & p = *((sad::Size2I *)ptr);
	picojson::value v(picojson::object_type, false);
	v.insert("width", picojson::value((double)(p.Width)));
	v.insert("height", picojson::value((double)(p.Height)));
	return v;
}

};

/*! Specification for saving values of type vector of vectors of 2-dimensional points
 */
template<>
class Save<sad::Vector<sad::Point2D> >
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Vector<sad::Point2D> & p = *((sad::Vector<sad::Point2D> *)ptr);
	picojson::value v(picojson::array_type, false);
	for(size_t i = 0; i < p.size(); i++)
	{
		v.push_back(sad::db::Save<sad::Point2D>::perform(&(p[i])));
	}
	return v;
}

};

}

}
