/*! \file easingfunction.h
    
    Defines an abstract easing function for interpolating beeasing two points, based on path
 */
#pragma once
#include "../../object.h"
#include "../../db/dbobjectfactory.h"
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
    Function(sad::animations::easing::Types type, double overshootAmplitude, double period);
    /*! Could be inherited
     */
    virtual ~Function();
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

}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::animations::easing::Function)