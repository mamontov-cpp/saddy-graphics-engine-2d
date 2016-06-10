/*! \file scripting/layouts/scriptablegridcell.h

    A grid cell scripting bindings should be placed here
 */
#pragma once
#include "../classwrapper.h"

#include <layouts/grid.h>

#include <QScriptValue>

namespace scripting
{

class Scripting;

namespace layouts
{

class ScriptableLengthValue;

/*! A wrapper for a reference for grid cell
 */
class ScriptableGridCell: public scripting::ClassWrapper
{
    Q_OBJECT
public:
    /*! Initializes a scriptable grid cell
        \param[in] major_id a major id for a parent grid
        \param[in] row a row index for cell
        \param[in] column a column index for cell
        \param[in] s scripting
     */
    ScriptableGridCell(
        unsigned long long major_id,
        unsigned int row,
        unsigned int column,
        scripting::Scripting* s
    );
    /*! Converts object to string representation
        \return object to string
     */
    virtual QString toString() const;
    /*! A destructor for grid cell
     */
    virtual ~ScriptableGridCell();
    /*! Returns referenced cell, if reference is valid or returns NULL and throws exception otherwise
        \param[in] throwexc whether we should throw exception or not
        \param[in] name a name for a called method, which can be used in exception
        \return grid
     */
    sad::layouts::Cell* cell(bool throwexc = true, const QString& name = "") const;
public slots:
    /*! Returns true, if referenced object is valid
     */
    bool valid() const;
    /*! Sets width for value
        \param[in] value a value for width of cell
     */
    void setWidth(scripting::layouts::ScriptableLengthValue* value);
    /*! Returns width of cell
        \return width of cell
     */
    QScriptValue width() const;
protected:
    /*! A major id for parent grid
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

