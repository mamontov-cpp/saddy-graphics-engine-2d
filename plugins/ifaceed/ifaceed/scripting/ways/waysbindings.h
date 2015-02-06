/*! \file waysbindings.h
    \author HiddenSeeker

    A bindings for ways are listed here
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>
#include <sadcolor.h>
#include <scene.h>

#include <QScriptEngine>
#include <QVector>


namespace scripting
{

namespace ways
{

/*! Lists all ways
    \param[in] ctx context
    \param[in] engine an enginge
    \return a scene nodes list
 */
QScriptValue list(
    QScriptContext* ctx,
    QScriptEngine* engine
);

}

}
