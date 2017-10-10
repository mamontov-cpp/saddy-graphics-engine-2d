/*! \file animationsanimation.h
    

    Defines a basic class for all kind of animation as a resource in project
 */
#pragma once

#include "../resource/resource.h"
#include "../mrobject.h"

#include "animationssavedobjectstatecreators.h"

#include "setstate/abstractsetstatecommand.h"
#include "setstate/dummycommand.h"

namespace sad
{

namespace animations
{

class Instance;

namespace easing
{

class Function;

}

class Animation: public sad::resource::Resource, public sad::MRObject
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    Animation();
    /*! Must be inherited
     */
    virtual ~Animation();
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
    /*! Sets, whether animation is looped
        \param[in] looped whether animation is looped
     */
    void setLooped(bool looped);
    /*! Returs, whether animation is looped
        \return whether animation is looped
     */
    bool looped() const;
    /*! Sets playback time for animation
        \param time a playback time for animation
     */
    virtual void setTime(double time);
    /*! Returns playback time for animation
        \return playback time for animation
     */
    virtual double time() const;
    /*! Sets easing function
        \param[in] f function
     */
    virtual void setEasing(sad::animations::easing::Function* f);
    /*! Returns easing function
        \return easing function
     */
    sad::animations::easing::Function* easing() const;
    /*! Starts an animation. Called on start of instance.
        \param[in] i animation instance
     */
    virtual void start(sad::animations::Instance* i);
    /*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time) = 0;
    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o) = 0;
    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o) = 0;
    /*! Returns, whether animation is valid
        \return whether animation is valid
     */
    // ReSharper disable once CppRedundantInlineSpecifier
    inline bool valid() const
    {
        return m_valid;
    }
    /*! Returns creators for saving object states
        \return creators list
     */
    virtual const sad::Vector<sad::animations::AbstractSavedObjectStateCreator*>& creators() const;
    /*! Update animation before starting to play
     */
    virtual void updateBeforePlaying();
protected:
    /*! Updates flag of validity
     */
    void updateValidFlag();
    /*! Loads an animation from specified file, using specified renderer for resolving some
        properties.
        \param[in] file a file, via which a resource should be loaded
        \param[in] r  a renderer, which resource should be linked to (NULL if global renderer)
        \param[in] options  an options for loading a resource
        \return whether loading was successfull
     */
    virtual bool load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
    );
    /*! Whether animation is looped
     */
    bool m_looped;
    /*! A total time of animation playback
     */
    double m_time;
    /*! Whether animation's specific implementation is valid
     */
    bool m_inner_valid;
    /*! Whether all animation is valid
     */
    bool m_valid;
    /*! A creator list for creating a saved object state
     */
    sad::animations::SavedObjectStateCreators m_creators;
    /*! An easing function
     */
    sad::animations::easing::Function* m_easing;
private:
    /*! Disable copying for animation
        \param[in] a animation
     */
    Animation(const sad::animations::Animation& a);
    /*! Disable copying for animation
        \param[in] a animation
        \return copied animation
     */
    sad::animations::Animation& operator=(const sad::animations::Animation& a);

    /*! Returns easing property, which has pretty complex implementation
        \return easing property
     */
    static sad::db::Property* easingProperty();
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::animations::Animation)
