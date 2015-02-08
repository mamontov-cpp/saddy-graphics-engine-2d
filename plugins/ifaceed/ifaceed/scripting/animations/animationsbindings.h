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

}

}
