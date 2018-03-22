/*! \file easingfunction.h
    
    Defines an abstract easing function for interpolating beeasing two points, based on path
 */
#pragma once
#include "../../object.h"
#include "../../db/dbobjectfactory.h"
#include "../../db/load.h"
#include "../../db/save.h"

#include "easingtypes.h"

namespace sad
{

namespace animations
{

namespace easing
{

class Function: public sad::Object
{
SAD_OBJECT
public:
    /*! Creates a default linear function
     */
    Function();
    /*! Creates a function with specified parameters
        \param[in] type a type of function
        \param[in] overshootAmplitude an overshoot amplitude
        \param[in] period specified period
    */
    Function(unsigned int type, double overshootAmplitude, double period);
    /*! Creates a function with specified parameters
        \param[in] type a type of function
        \param[in] overshootAmplitude an overshoot amplitude
        \param[in] period specified period
     */
    Function(sad::animations::easing::Types type, double overshootAmplitude, double period);
    /*! Could be inherited
     */
    virtual ~Function();
    /*! Clones an object
        \return cloned object
     */
    virtual sad::animations::easing::Function* clone() const;
    /*! Computes specified value at time with specified duration
        \param[in] time a time
        \param[in] duration a duration time
     */
    virtual double eval(double time, double duration);
    /*! Computes specified value at time with specified duration (strictly beeasing 0..1)
        \param[in] time a time
        \param[in] duration a duration time
     */
    virtual double evalBounded(double time, double duration);
    /*! Sets type of easing function as unsigned int
        \param[in] type a type value
     */
    void setFunctionTypeAsUnsignedInt(unsigned int type);
    /*! Returns type of easing function a unsigned int    
        \return type as unsigned int
     */
    unsigned int functionTypeAsUnsignedInt() const;
    /*! Sets type of easing function
        \param[in] type a type value
     */
    void setFunctionType(sad::animations::easing::Types type);
    /*! Returns type of easing function a unsigned int    
        \return type as unsigned int
     */
    sad::animations::easing::Types functionType() const;
    /*! Sets overshoot amplitude
        \param[in] a value
     */
    void setOvershootAmplitude(double a);
    /*! Returns overshoot amplitude
        \return overshoot amplitude
     */
    double overshootAmplitude() const;
    /*! Sets period
        \param[in] a period parameter
     */
    void setPeriod(double a);
    /*! Returns period parameter
        \return period parameter
     */
    double period() const;
    /*! A basic schema for object
        \return a schema 
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Returns a factory for deserializing this type from value
        \return a factory 
     */
    static sad::db::ObjectFactory* factory();
protected:
    /*! A type for easing function
     */
    sad::animations::easing::Types m_type;
    /*! A function callback for easing function (depends from type)
     */
    sad::animations::easing::FunctionCallback m_callback;
    /*! An overshoot amplitude
     */
    double m_overshoot_amplitude;
    /*! A period information for some interpolation functions
     */
    double m_period;
};

}

}


namespace db
{
    
/*! Load a value of specified type
    \param[in] ptr a value to be saved
    \param[in] v a special value, from which we should load stuff
    \return result
 */
template<>
inline bool Load<sad::animations::easing::Function*>::perform(void * ptr, const picojson::value & v)
{
    if (!ptr)
    {
        throw sad::db::InvalidPointer();
    }
    sad::animations::easing::Function** tptr = reinterpret_cast<sad::animations::easing::Function**>(ptr);
    sad::animations::easing::Function* maybe_result = static_cast<sad::animations::easing::Function*>(sad::animations::easing::Function::factory()->createFromEntry(v));
    bool result = false;
    if (maybe_result)
    {
        result = (maybe_result)->load(v);
        if (result)
        {
            *tptr = maybe_result;
        }
        else
        {
            delete maybe_result;
        }
    }
    return result;
}

/*! Saves a value of specified type
    \param[in] ptr a value to be saved
    \return value
 */
template<>
inline picojson::value Save<sad::animations::easing::Function*>::perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::animations::easing::Function** tptr = reinterpret_cast<sad::animations::easing::Function**>(ptr);
    picojson::value tmp(picojson::object_type, false);
    (*tptr)->save(tmp);
    return tmp;
}

}

}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::animations::easing::Function)