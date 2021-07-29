/*! \file animations/animationsblinking.h
    

    An animations as animation of blinking for an object
 */
#pragma once

#include "../fuzzyequal.h"

#include "animationsanimation.h"

namespace sad
{
    
namespace animations
{

/*! An animation of changing color of an object
 */
class Blinking: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    Blinking();
    /*! Can be inherited
     */
    virtual ~Blinking() override;
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const override;
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successful
     */
    virtual bool loadFromValue(const picojson::value& v) override;
    /*! Sets frequency of blinking
        \param[in] freq frequency of blinking
     */
    void setFrequency(unsigned int freq);
    /*! Returns frequency of blinking
     */
    unsigned int frequency() const;
    /*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time) override;
    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o) override;
    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o) override;
protected:
    /*! A frequency of blinking
     */
    unsigned int m_frequency;
};

}

}
