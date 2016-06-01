/*! \file scripting/layouts/scriptablegrid.h

    A grid scripting interface is placed here
 */
#pragma once
#include "../classwrapper.h"

#include <layouts/grid.h>

namespace scripting
{

class Scripting;

namespace layouts
{

/*! A wrapper to point reference
 */
class ScriptableGrid: public scripting::ClassWrapper
{
 Q_OBJECT
public:
    /*! A destructor for grid
     */
    virtual ~ScriptableGrid();

protected:
    /*! A major id for database object
     */
    unsigned long long m_majorid;
    /*! A main scripting object
     */
    scripting::Scripting* m_scripting;
};

}

}
