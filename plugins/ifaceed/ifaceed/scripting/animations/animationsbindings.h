/*! \file animationsbindings.h
    \author HiddenSeeker

    A bindings for animations are listed here
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>

#include <QScriptEngine>
#include <QVector>

#include <animations/animationsanimation.h>
#include <animations/animationscomposite.h>

namespace scripting
{

class Scripting;

namespace animations
{

/*! Lists all animations
    \param[in] ctx context
    \param[in] engine an enginge
    \return a ways list
 */
QScriptValue list(
    QScriptContext* ctx,
    QScriptEngine* engine
);


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


}

}
