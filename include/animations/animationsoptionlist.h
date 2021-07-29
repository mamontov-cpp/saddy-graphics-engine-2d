/*! \file animations/animationsoptionlist.h
    

    An animations as animation of changing options of sprite by time
 */
#pragma once

#include "../sadvector.h"
#include "../sadstring.h"
#include "../sadhash.h"

#include "animationsanimation.h"


namespace sad
{
    
namespace animations
{

/*! An animation, which changes options of sprite over time
 */
class OptionList: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    OptionList();
    /*! Can be inherited
     */
    virtual ~OptionList() override;
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
    /*! Sets list of options
        \param[in] list an option list, used in animation
     */
    void setList(const sad::Vector<sad::String>& list);
    /*! Returns option list
        \return option list
     */
    const sad::Vector<sad::String> & list() const;
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
    /*! A list of texture coordinates to be set
     */
    sad::Vector<sad::String> m_list;
};

}

}
