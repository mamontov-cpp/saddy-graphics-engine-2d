/*! \file animationsbindings.h
    \author HiddenSeeker

    A bindings for animations are listed here
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>

#include <QScriptEngine>
#include <QVector>

#include <p2d/app/way.h>

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

}

}
