/*! \file db/save,h
    
    
    Describes a save operations for a type
 */
#pragma once
#include "dberror.h"
#include "dbtypename.h"
#include "../sadstring.h"
#include "../sadrect.h"
#include "../sadcolor.h"
#include "../sadsize.h"
#include "../sadvector.h"
#include "../sadpair.h"
#include "../layouts/lengthvalue.h"

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
        static_cast<sad::Object *>(ptr)->save(v);
        return v;
    }
    throw sad::db::NotImplemented("sad::db::Save<_Type>::perform");
    //return picojson::value();
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
    return picojson::value(*static_cast<bool*>(ptr));
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
    return picojson::value(*static_cast<std::string*>(ptr));
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
    return picojson::value(static_cast<sad::String>(*static_cast<sad::String*>(ptr)));
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
    const sad::Point2D & p = *(static_cast<sad::Point2D *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("x", picojson::value(static_cast<double>(p.x())));
    v.insert("y", picojson::value(static_cast<double>(p.y())));
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
    const sad::Point2I & p = *(static_cast<sad::Point2I *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("x", picojson::value(static_cast<double>(p.x())));
    v.insert("y", picojson::value(static_cast<double>(p.y())));
    return v;
}

};

/*! Specification for saving point values
 */
template<>
class Save<sad::Point3D>
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    const sad::Point3D & p = *(static_cast<sad::Point3D *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("x", picojson::value(static_cast<double>(p.x())));
    v.insert("y", picojson::value(static_cast<double>(p.y())));
    v.insert("z", picojson::value(static_cast<double>(p.z())));
    return v;
}

};

/*! Specification for saving point values
 */
template<>
class Save<sad::Point3I>
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    const sad::Point3I & p = *(static_cast<sad::Point3I *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("x", picojson::value(static_cast<double>(p.x())));
    v.insert("y", picojson::value(static_cast<double>(p.y())));
    v.insert("z", picojson::value(static_cast<double>(p.z())));
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
    sad::Rect2D & p = *(static_cast<sad::Rect2D *>(ptr));
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
    const sad::Color & p = *(static_cast<sad::Color *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("r", picojson::value(static_cast<double>(p.r())));
    v.insert("g", picojson::value(static_cast<double>(p.g())));
    v.insert("b", picojson::value(static_cast<double>(p.b())));
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
    const sad::AColor & p = *(static_cast<sad::AColor *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("r", picojson::value(static_cast<double>(p.r())));
    v.insert("g", picojson::value(static_cast<double>(p.g())));
    v.insert("b", picojson::value(static_cast<double>(p.b())));
    v.insert("a", picojson::value(static_cast<double>(p.a())));
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
    sad::Vector<sad::Vector<sad::AColor> > & p = *(static_cast<sad::Vector<sad::Vector<sad::AColor> > *>(ptr));
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
    const sad::Size2D & p = *(static_cast<sad::Size2D *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("width", picojson::value(static_cast<double>(p.Width)));
    v.insert("height", picojson::value(static_cast<double>(p.Height)));
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
    const sad::Size2I & p = *(static_cast<sad::Size2I *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("width", picojson::value(static_cast<double>(p.Width)));
    v.insert("height", picojson::value(static_cast<double>(p.Height)));
    return v;
}

};

/*! Specification for saving values of type vector of 2-dimensional points
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
    sad::Vector<sad::Point2D> & p = *(static_cast<sad::Vector<sad::Point2D> *>(ptr));
    picojson::value v(picojson::array_type, false);
    for(size_t i = 0; i < p.size(); i++)
    {
        v.push_back(sad::db::Save<sad::Point2D>::perform(&(p[i])));
    }
    return v;
}

};

/*! Specification for saving dialogue phrase values
 */
template<>
class Save<sad::dialogue::Phrase>
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    const sad::dialogue::Phrase & p = *(static_cast<sad::dialogue::Phrase *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("name", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.actorName()))));
    v.insert("portrait", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.actorPortrait()))));
    v.insert("phrase", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.phrase()))));
    double duration = p.duration();
    v.insert("duration", sad::db::Save<double>::perform(&duration));
    v.insert("viewhint", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.viewHint()))));
    return v;
}

};

/*! Specification for saving values of type vector of phrases
 */
template<>
class Save<sad::Vector<sad::dialogue::Phrase*> >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::Vector<sad::dialogue::Phrase*> & p = *(static_cast<sad::Vector<sad::dialogue::Phrase*> *>(ptr));
    picojson::value v(picojson::array_type, false);
    for(size_t i = 0; i < p.size(); i++)
    {
        v.push_back(sad::db::Save<sad::dialogue::Phrase>::perform(p[i]));
    }
    return v;
}

};

/*! Specification for saving values of type vector of unsigned long long
 */
template<>
class Save<sad::Vector<unsigned long long> >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::Vector<unsigned long long> & p = *(static_cast<sad::Vector<unsigned long long> *>(ptr));
    picojson::value v(picojson::array_type, false);
    for(size_t i = 0; i < p.size(); i++)
    {
        v.push_back(sad::db::Save<unsigned long long>::perform(&(p[i])));
    }
    return v;
}

};

/*! Specification for saving values of type vector of unsigned long long
 */
template<>
class Save<sad::Vector<sad::String> >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::Vector<sad::String> & p = *(static_cast<sad::Vector<sad::String> *>(ptr));
    picojson::value v(picojson::array_type, false);
    for(size_t i = 0; i < p.size(); i++)
    {
        v.push_back(sad::db::Save<sad::String>::perform(&(p[i])));
    }
    return v;
}

};


/*! Specification for saving values of type sad::layouts::LengthValue
 */
template<>
class Save<sad::layouts::LengthValue>
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    const sad::layouts::LengthValue & p = *(static_cast<sad::layouts::LengthValue *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("unit", picojson::value(static_cast<double>(p.Unit)));
    v.insert("value", picojson::value(static_cast<double>(p.Value)));
    return v;
}

};


/*! Specification for saving pair
 */
template<typename T1, typename T2>
class Save<sad::Pair<T1, T2> >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::Pair<T1, T2> & p = *(static_cast<sad::Pair<T1, T2> *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("field1", sad::db::Save<T1>::perform(&(p._1())));
    v.insert("field2", sad::db::Save<T2>::perform(&(p._2())));
    return v;
}

};


/*! Specification for saving triplet
 */
template<typename T1, typename T2, typename T3>
class Save<sad::Triplet<T1, T2, T3> >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::Triplet<T1, T2, T3> & p = *(static_cast<sad::Triplet<T1, T2, T3> *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("field1", sad::db::Save<T1>::perform(&(p._1())));
    v.insert("field2", sad::db::Save<T2>::perform(&(p._2())));
    v.insert("field3", sad::db::Save<T2>::perform(&(p._3())));
    return v;
}

};

/*! Specification for saving quadruplet
 */
template<typename T1, typename T2, typename T3, typename T4>
class Save<sad::Quadruplet<T1, T2, T3, T4> >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::Quadruplet<T1, T2, T3, T4> & p = *(static_cast<sad::Quadruplet<T1, T2, T3, T4> *>(ptr));
    picojson::value v(picojson::object_type, false);
    v.insert("field1", sad::db::Save<T1>::perform(&(p._1())));
    v.insert("field2", sad::db::Save<T2>::perform(&(p._2())));
    v.insert("field3", sad::db::Save<T2>::perform(&(p._3())));
    v.insert("field4", sad::db::Save<T2>::perform(&(p._4())));
    return v;
}

};


/*! Specification for saving quadruplet
 */
template<typename T>
class Save<sad::Vector<T> >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    const sad::Vector<T> & p = *(static_cast<sad::Vector<T> *>(ptr));
    picojson::value v(picojson::array_type, false);
    for(size_t i = 0; i < p.size(); i++)
    {
        const void* vpi = &(p[i]);
        void* pi = const_cast<void*>(vpi);
        v.push_back(sad::db::Save<T>::perform(pi));
    }
    return v;
}

};

}

}
