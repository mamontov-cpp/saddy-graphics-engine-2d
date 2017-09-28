/*! \file animationsbindings.h
    

    A bindings for animations are listed here
 */
#pragma once
#include "../dukqtcontext.h"
#include <sadstring.h>
#include <sadrect.h>
#include <maybe.h>

#include <QVector>

#include <animations/animationsanimation.h>
#include <animations/animationscomposite.h>

namespace scripting
{

class Scripting;

namespace animations
{

/*! Returns all major ids for animations, stored in database
    \return list of animations, stored in database
 */
QVector<unsigned long long> list();


/*! Adds animation. Prefixed by underscore, since it will be mapped to _add function and add
    is reserved for call, which will take object, preprocess it's fields and call _add using fields of this object.
    \param[in] scripting a scripting part
    \param[in] type a type name
    \param[in] name a name for way
    \param[in] time a time of animations
    \param[in] looped whether animation is looped
 */ 
unsigned long long _add(
    scripting::Scripting* scripting, 
    sad::String type,
    sad::String name,
    double time,
    bool looped
);

/*! Removes an animation
    \param[in] scripting a scripting part
    \param[in] a animation
 */
void remove(
    scripting::Scripting* scripting, 
    sad::animations::Animation* a
);

/*! Adds animation to composite animation, or does nothing if it's produces loops
    \param[in] scripting a scripting part
    \param[in] list a composite list
    \param[in] a animation
    \return true on success
 */
bool addToComposite(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    sad::animations::Animation* a
);

/*! Removes from composite animations, or does nothing if it's not in list
    \param[in] scripting a scripting part
    \param[in] list a composite list
    \param[in] a animation
    \return true on success
 */
bool removeFromComposite(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    sad::animations::Animation* a
);

/*! Returns length of composite animation
    \param[in] scripting a scripting part
    \param[in] list a composite list
    \return length of composite part
 */
int compositeLength(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list
);

/*! Returns id of animation in composite part (0 if cannot be retrieved)
    \param[in] scripting a scripting part
    \param[in] list a composite list
    \param[in] pos position
    \return length of composite part
 */
unsigned long long getAnimation(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    unsigned int pos
);

/*! Moves animation back in list
    \param[in] scripting a scripting part
    \param[in] list a composite list
    \param[in] pos position
    \return whether it was successfull
 */
bool moveBackInCompositeList(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    unsigned int pos
);

/*! Moves animation front in list
    \param[in] scripting a scripting part
    \param[in] list a composite list
    \param[in] pos position
    \return whether it was successfull
 */
bool moveFrontInCompositeList(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    unsigned int pos
);

/*! Sets readable or writable properties for animations
    \param[in] obj object
    \param[out] list a list of properties
    \param[in] readable a readable or writable properties
 */
void checkPropertiesForAnimations(
    const sad::Maybe<sad::db::Object*>& obj,
    QStringList& list,
    bool readable
);

/*! Initializes bindings for animations
    \param[in] s scripting object
    \param[in] e a global binding object
    \return created animations object
 */
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* e);

}

}
