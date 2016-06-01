/*! \file scripting/layouts/scriptablegrid.h

    A grid scripting interface is placed here
 */
#pragma once
#include "../classwrapper.h"

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
	/*! Declares a scriptable grid
		\param[in] major_id a major id for a grid
		\param[in] s scripting
	 */
	ScriptableGrid(
		unsigned long long major_id,
		scripting::Scripting* s
	);
    /*! A destructor for grid
     */
    virtual ~ScriptableGrid();
    /*! Converts object to string representation
        \return object to string
     */
    QString toString() const;
public slots:	
	/*! Returns true, if referenced object is valid
	 */		 
	bool valid() const;
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
