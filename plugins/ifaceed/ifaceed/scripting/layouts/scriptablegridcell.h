/*! \file scripting/layouts/scriptablegridcell.h

    A grid cell scripting bindings should be placed here
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
class ScriptableGridCell: public scripting::ClassWrapper
{
	Q_OBJECT
public:
    /*! A destructor for grid
     */
    virtual ~ScriptableGridCell();

protected:
    /*! A major id for database object
     */
    unsigned long long m_majorid;
	/*! A row for cell
	 */
	unsigned int m_row;
	/*! A column for cell
	 */
	unsigned int m_column;
    /*! A main scripting object
     */
    scripting::Scripting* m_scripting;
};

}

}

