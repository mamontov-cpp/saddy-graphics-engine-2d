/*! \file scripting/layouts/scriptablegridcell.h

    A grid cell scripting bindings should be placed here
 */
#pragma once

#include <layouts/grid.h>
#include <QObject>
#include <QMetaType>
#include <3rdparty/dukpp-03/include/maybe.h>

namespace history
{
class Command;
}

namespace scripting
{

class Scripting;

namespace layouts
{

class ScriptableLengthValue;

/*! A wrapper for a reference for grid cell
 */
class ScriptableGridCell: public QObject
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
    /*! A destructor for grid cell
     */
    virtual ~ScriptableGridCell();
    /*! Returns referenced cell, if reference is valid or returns NULL and throws exception otherwise
        \param[in] throwexc whether we should throw exception or not
        \param[in] name a name for a called method, which can be used in exception
        \return cell
     */
    sad::layouts::Cell* cell(bool throwexc = true, const QString& name = "") const;
    /*! Swaps children, providing specified name of called function as own
        \param[in] callname a called name 
        \param[in] pos1 position of first child
        \param[in] pos2 position of second child
        \return false on invalid positions
     */
    bool swapChildrenWithCallName(const QString& callname, int pos1, int pos2) const;
    /*! Returns list of children's major ids
        \return list of children's major ids
     */
    // ReSharper disable once CppHidingFunction
    QVector<unsigned long long> children() const;
    /*! Tries to find child by major id
        \param[in] o object
        \return NULL if not found, otherwise position
     */
    dukpp03::Maybe<unsigned int> findChild(sad::SceneNode* o) const;
    /*! Tries to add child to a cell. If child already has parent grid, returns false
        \param[in] node  a node object
        \return false, if child already has parent grid
     */
    bool addChild(sad::SceneNode* node) const;
public slots:
    /*! Converts object to string representation
       \return object to string
    */
    virtual QString toString() const;
    /*! Returns true, if referenced object is valid
     */
    bool valid() const;
    /*! Returns area for a grid cell
        \return area
     */
    sad::Rect2D area() const;
    /*! Returns major id of grid for cell
        \return major id
     */
    unsigned long long grid() const;
    /*! Sets width for a cell
        \param[in] value a value for width of cell
     */
    void setWidth(scripting::layouts::ScriptableLengthValue* value) const;
    /*! Returns width of cell
        \return width of cell
     */
    scripting::layouts::ScriptableLengthValue* width() const;
    /*! Sets height for a cell
        \param value a value for height of cell
     */
    void setHeight(scripting::layouts::ScriptableLengthValue* value) const;
    /*! Returns height of cell
        \return height of cell
     */
    scripting::layouts::ScriptableLengthValue* height() const;
    /*! Sets horizontal alignment for cell
        \param[in] v value
     */
    void setHorizontalAlignment(unsigned int v) const;
    /*! Returns horizontal alignment for a scriptable grid cell
        \return horizontal alignment
     */
    unsigned int  horizontalAlignment() const;
    /*! Sets vertical alignment for cell
        \param[in] v value
     */
    void setVerticalAlignment(unsigned int  v) const;
    /*! Returns vertical alignment for a scriptable grid cell
        \return vertical alignment
     */
    unsigned int  verticalAlignment() const;
    /*! Sets stacking type for cell
        \param[in] v value
     */
    void setStackingType(unsigned int  v) const;
    /*! Returns stacking type for a scriptable grid cell
        \return stacking type
     */
    unsigned int  stackingType() const;
    /*! Sets top padding for cell
        \param[in] v new padding value
     */
    void setPaddingTop(double v) const;
    /*! Returns top padding for cell
        \return top padding for cell
     */
    double paddingTop() const;
    /*! Sets bottom padding for cell
        \param[in] v new padding value
     */
    void setPaddingBottom(double v) const;
    /*! Returns bottom padding for cell
        \return bottom padding for cell
     */
    double paddingBottom() const;
    /*! Sets left padding for cell
        \param[in] v new padding value
     */
    void setPaddingLeft(double v) const;
    /*! Returns left padding for cell
        \return left padding for cell
     */
    double paddingLeft() const;
    /*! Sets right padding for cell
        \param[in] v new padding value
     */
    void setPaddingRight(double v) const;
    /*! Returns right padding for cell
        \return right padding for cell
     */
    double paddingRight() const;
    /*! Tries to remove child from a cell
        \param[in] pos position of object
        \return false if invalid position
     */
    bool removeChild(int pos) const;
    /*! Tries to clear cell, removing all children from it      
     */
    void clearChildren() const;
    /*! Swaps two children of cell
        \param[in] pos1 position of first children
        \param[in] pos2 position of second children
        \return false, if one of positions is invalid
     */
    bool swapChildren(int pos1, int pos2) const;
    /*! Moves child specified by position back in children list. Returns false, if object could not be moved in children list
        \param[in] pos position of child
        \return false, if object could not be moved in children list
     */
    bool moveBack(int pos) const;
    /*! Moves child specified by position front in children list. Returns false, if object could not be moved in children list
        \param[in] pos position of child
        \return false, if object could not be moved in children list
     */
    bool moveFront(int pos) const;
protected:    
    /*! Returns new command for changing padding
        \param[in] callname called function name for exception
        \param[in] propname property name for it
        \param[in] newvalue a new value for padding
        \return command
     */
    history::Command* commandForPadding(
        const QString& callname,
        const QString& propname,
        double newvalue
    ) const;
    /*! Handles attempt for changing padding
        \param[in] callname a called function name
        \param[in] newvalue a new value for padding
     */
    void tryChangePadding(const QString& callname, double newvalue) const;
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

Q_DECLARE_METATYPE(scripting::layouts::ScriptableGridCell**);
