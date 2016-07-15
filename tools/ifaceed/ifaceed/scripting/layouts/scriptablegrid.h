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
    /*! Initiailizes a scriptable grid
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
        \param[in] name a name for a called method, which can be used in exception
        \return grid
     */
    sad::layouts::Grid* grid(bool throwexc = true, const QString& name = "") const;
public slots:	
    /*! Returns true, if referenced object is valid
     */		 
    bool valid() const;
    /*! Returns area for a grid
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
        \param[in] fixed_height a flag value
     */
    void setFixedHeight(bool fixed_height) const;
    /*! Returns amount of rows in grid
        \return amount of rows
     */
    unsigned long rows() const;
    /*! Sets amount of rows in grid
        \param[in] rows amount of rows
     */
    void setRows(int rows) const;
    /*! Returns amount of columngs in grid
        \return amount of columns
     */
    unsigned long columns() const;
    /*! Sets amount of columns in grid
        \param[in] columns amount of columns
     */
    void setColumns(int columns) const;
    /*! Returns top padding for grid
        \return top padding
     */
    double paddingTop() const;
    /*! Returns bottom padding for grid
        \return bottom padding
     */
    double paddingBottom() const;
    /*! Returns left padding for grid
        \return left padding
     */
    double paddingLeft() const;
    /*! Returns right padding for grid
        \return right padding
     */
    double paddingRight() const;
    /*! Sets top padding
        \param[in] v value
        \param[in] propagate whether we propagate value to cells
     */
    void setPaddingTop(double v, bool propagate) const;
    /*! Sets bottom padding
        \param[in] v value
        \param[in] propagate whether we propagate value to cells
     */
    void setPaddingBottom(double v, bool propagate) const;
    /*! Sets left padding
        \param[in] v value
        \param[in] propagate whether we propagate value to cells
     */
    void setPaddingLeft(double v, bool propagate) const;
    /*! Sets right padding
        \param[in] v value
        \param[in] propagate whether we propagate value to cells
     */
    void setPaddingRight(double v, bool propagate) const ;
    /*! Sets top padding, propagating values to cells
        \param v value
     */
    void setPaddingTop(double v) const;
    /*! Sets bottom padding, propagating values to cells
        \param v value
     */
    void setPaddingBottom(double v) const;
    /*! Sets left padding, propagating values to cells
        \param v value
     */
    void setPaddingLeft(double v) const;
    /*! Sets right padding, propagating values to cells
        \param v value
     */
    void setPaddingRight(double v) const;
    /*! Returns a cell for specified row or column
        \param[in] row a row value
        \param[in] column a column value
        \return NULL and throws exception if cell not exists, cell otherwiser
     */
    QScriptValue cell(int row, int column);
    /*! Tries to merge cells, using specified region
        \param[in] row row of top-left cell of region
        \param[in] column column of top-left cell of region
        \param[in] rowspan amount of rows spanned
        \param[in] colspan amount of columns spanned
        \return whether it was successfull or failed
     */
    bool merge(int row, int column, int rowspan, int colspan);
    /*! Tries to split cells, using specified region
        \param[in] row row of top-left cell of region
        \param[in] column column of top-left cell of region
        \param[in] rowspan amount of rows spanned
        \param[in] colspan amount of columns spanned
        \return whether it was successfull or failed
     */
    bool split(int row, int column, int rowspan, int colspan);
    /*! Tries to find child by major id
        \param[in] o object
        \return NULL if not found, array with first element of cell, and second is position, otherwise
     */
    QScriptValue findChild(const QScriptValue& o);
    /*! Returns list of childrens major ids from a grid
        \return list of major ids of children
     */
    QScriptValue children() const;
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
