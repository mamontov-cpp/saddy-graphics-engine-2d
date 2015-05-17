/*! \file scenenodesareasetter.h
    \author HiddenSeeker

    Describes a setter for area property of scene node
 */
#pragma once
#include "scenenodessetter.h"

#include "../../history/scenenodes/scenenodeschangearea.h"

namespace scripting
{

namespace scenenodes
{

class AreaSetter: public scripting::scenenodes::Setter<sad::Rect2D, history::scenenodes::ChangeArea>
{
public:
    /*! Creates new setter for specified area
        \param e
     */
    AreaSetter(QScriptEngine* e);
    /*! Can be inherited
     */
    virtual ~AreaSetter();
    /*! Determines, whether it can be called with this context
        Additionally checks, whether rectangle is AABB.
        \param[in] ctx context
        \return whether it could be called, or error
     */
    virtual scripting::MatchResult canBeCalled(QScriptContext* ctx);
};


}

}
