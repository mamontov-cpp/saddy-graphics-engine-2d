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

}
}