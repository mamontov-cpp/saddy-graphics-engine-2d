/*! \file scenenodesfontsizesetter.h
    \author HiddenSeeker

    Describes a setter for font size property of scene node
 */
#pragma once
#include "scenenodessetter.h"

#include "../../history/label/changefontsize.h"

namespace scripting
{

namespace scenenodes
{

class FontSizeSetter: public scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>
{
public:
    /*! Creates new setter for specified area
        \param e
     */
    FontSizeSetter(QScriptEngine* e);
    /*! Can be inherited
     */
    virtual ~FontSizeSetter();
    /*! Determines, whether it can be called with this context
        Additionally checks, whether number is greater than zero.
        \param[in] ctx context
        \return whether it could be called, or error
     */
    virtual sad::Maybe<QString> canBeCalled(QScriptContext* ctx);
};


}

}
