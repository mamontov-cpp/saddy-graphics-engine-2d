/*! \file scripting/layouts/scriptablegrid.h

    A grid scripting interface is placed here
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
    /*! Returns object, if reference is valid or returns NULL and throws exception otherwise
        \param[in] throwexc whether we should throw exception or not
        \param[in] name a name for a grid
        \return grid
     */
    sad::layouts::Grid* grid(bool throwexc = true, const QString& name = "") const;
public slots:	
    /*! Returns true, if referenced object is valid
     */		 
    bool valid() const;
    /*! Returns area from grid
        \return area
     */
    QScriptValue area() const;
    /*! Sets an area for a grid
        \param[in] newarea new area value
     */
    void setArea(const QScriptValue& newarea) const;
    /*! Returns current major id of grid
        \returns current major id of grid
     */
    unsigned long long majorId() const;
    /*! Returns current minor id of grid
        \returns current minor id of grid
     */
    unsigned long long minorId() const;
    /*! Returns current name of grid
        \return name of grid
     */
    QString name() const;
    /*! Sets name for a grid
        \param[in] name a name
     */
    void setName(const QString& name) const;
    /*! Returns whether grid has fixed width
        \returns whether grid has fixed width 
     */
    bool fixedWidth() const;
    /*! Sets fixed width for grid
        \param[in] fixed_width a flag value
     */
    void setFixedWidth(bool fixed_width) const;
    /*! Returns whether grid has fixed height
        \returns whether grid has fixed height
     */
    bool fixedHeight() const;
    /*! Sets fixed height for grid
        \param[in] height a flag value
     */
    void setFixedHeight(bool fixed_height) const;
    /*! Returns amount of rows in grid
        \return amount of rows
     */
    unsigned long rows() const;
    /*! Sets amount of rows in grid
        \param[in] rows amount of rows
     */
    void setRows(int rows);
    /*! Returns amount of columngs in grid
        \return amount of columns
     */
    unsigned long columns() const;
    /*! Sets amount of columns in grid
        \param[in] columns amount of columns
     */
    void setColumns(int columns);
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
