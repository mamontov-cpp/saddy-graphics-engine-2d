/*! \file animations/animationsfontlist.h
    

    An animations as animation of changing fonts by time
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"

#include "animationsanimation.h"


namespace sad
{
    
namespace animations
{

/*! An animation, which changes font of label
 */
class FontList: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    FontList();
    /*! Can be inherited
     */
    virtual ~FontList() override;
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
    /*! Sets list of fonts
        \param[in] fonts a font list, used in animation
     */
    void setFonts(const sad::Vector<sad::String>& fonts);
    /*! Returns font list
        \return font list
     */
    const sad::Vector<sad::String> & fonts() const;
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
    /*! A list of fonts to be set in animation
     */
    sad::Vector<sad::String> m_fonts;
};

}

}
