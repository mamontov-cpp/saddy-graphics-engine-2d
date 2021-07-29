/*! \file animationstyping.h
    

    Describes an animation for typing of string in label
 */
#pragma once
#include "animationsanimation.h"


namespace sad
{
    
namespace animations
{

/*! An animation for typing of string in label
 */
class Typing: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    Typing();
    /*! Can be inherited
     */
    virtual ~Typing() override;
    /*! An animations instance
        \param[in] i an instance of animations
     */
    virtual void start(sad::animations::Instance* i) override;
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
};

}

}

