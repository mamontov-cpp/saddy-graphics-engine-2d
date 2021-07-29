/*! \file dialogue/phrase.h
    
    
    Describes a dialogue phrase, which could be used, when implemented dialogue system in your game
 */
#pragma once
#include "../sadstring.h"
 
 
namespace sad
{
 
namespace dialogue
{

/*! A phrase, as phrase from dialogue
 */
class Phrase
{
public:
    /*! Constructs new empty phrase
     */
    Phrase();
    /*! Could be inherited
     */
    virtual ~Phrase();
    /*! Sets actor name for phrase
        \param[in] name a name for actor
     */
    virtual void setActorName(const sad::String& name);
    /*! Sets actor portrait
        \param[in] portrait a portrait for actor
     */
    virtual void setActorPortrait(const sad::String& portrait);
    /*! Sets phrase 
        \param[in] phrase a phrase
     */
    virtual void setPhrase(const sad::String& phrase);
    /*! Sets duration 
        \param[in] duration a duration for phrase
     */ 
    virtual void setDuration(double duration);
    /*! Sets view hint for a phrase
        \param[in] view_hint a hint for viewing phrase
     */
    virtual void setViewHint(const sad::String& view_hint);
    /*! Returns actor name
        \return actor name
     */
    virtual const sad::String& actorName() const;
    /*! Returns actor portrait, which could be used as link to sad::Sprite2D::Options link
        \return actor portrait
     */
    virtual const sad::String& actorPortrait() const;
    /*! Returns phrase
        \return phrase
     */
    virtual const sad::String& phrase() const;
    /*! Returns duration
        \return duration
     */
    virtual double duration() const;
    /*! Returns view hint for a phrase
        \return view hint for a phrase
     */
    virtual const sad::String& viewHint() const;
protected:
    /*! An actor (possibly name, which could be shown near portrait)
     */
    sad::String m_actor_name;
    /*! Possibly actor portrait, which could be used as link to sad::Sprite2D::Options link
     */
    sad::String m_actor_portrait;
    /*! A phrase, which should be shown
     */
    sad::String m_phrase;
    /*! A duration, which should be used to display phrase
     */
    double m_duration;
    /*! A user-defines hint for viewing phrase
     */
    sad::String m_view_hint;
}; 
 
}
 
}
