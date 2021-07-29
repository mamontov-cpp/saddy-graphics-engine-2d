/*! \file animations/animationsfontsize.h
    

    An animations as animation of font size by time
 */
#pragma once

#include "../fuzzyequal.h"

#include "animationsanimation.h"

namespace sad
{
    
namespace animations
{

/*! An animation of changing of font size 
 */
class FontSize: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    FontSize();
    /*! Can be inherited
     */
    virtual ~FontSize() override;
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
    /*! Sets minimal font size
        \param[in] size minimal font size
     */
    void setMinSize(unsigned int size);
    /*! Returns minimal font size
        \return minimal font size
     */
    unsigned int minSize() const;
    /*! Sets maximal font size
        \param[in] size maximal font size
     */
    void setMaxSize(unsigned int size);
    /*! Returns maximal font size
        \return maximal font size
     */
    unsigned int maxSize() const;
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
    /*! A minimal font size
     */
    unsigned int m_min_size;
    /*! A maximal font size
     */
    unsigned int m_max_size;
};

}

}
