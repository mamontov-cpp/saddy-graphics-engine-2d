/*! \file dialoguesbindings.h
    \author HiddenSeeker

    A bindings for dialogues are listed here
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>

#include <QScriptEngine>
#include <QVector>

#include <dialogue/dialogue.h>

namespace scripting
{

class Scripting;

namespace dialogues
{

/*! Lists all dialogues
    \param[in] ctx context
    \param[in] engine an enginge
    \return a dialogues list
 */
QScriptValue list(
    QScriptContext* ctx,
    QScriptEngine* engine
);

/*! Adds dialogue. Prefixed by underscore, since it will be mapped to _add function and add
    is reserved for call, which will take object, preprocess it's fields and call _add using fields of this object.
	\param[in] scripting a scripting part
	\param[in] name a name for way
	\param[in] phrases a list of phrases
 */ 
unsigned long long _add(
	scripting::Scripting* scripting, 
	sad::String name,
	sad::Vector<sad::dialogue::Phrase> phrases
);

/*! Removes a dialogue
	\param[in] scripting a scripting part
	\param[in] d a dialogue to be removed
 */
void remove(scripting::Scripting* scripting, sad::dialogue::Dialogue* d);

}

}