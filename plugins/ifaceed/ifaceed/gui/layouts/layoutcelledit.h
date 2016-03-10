/*! \file layoutcelledit.h

    Defines a layout cell editor for editing
    a layout cell
 */
#include <QTableWidget>
#include <QCheckBox>
#include <vector>

namespace gui
{

namespace layouts
{

class LayoutCellEdit: public QTableWidget
{
Q_OBJECT
public:
    /*! Constructs new editor
        \param[in] parent a parent widget
     */
    LayoutCellEdit(QWidget* parent = 0);
    /*! This class could be inherited
     */
    virtual ~LayoutCellEdit();
    /*! A row for a cell editor in source grid
     */
    size_t Row;
    /*! A column for a cell editor in source grid
     */
    size_t Col;
protected:
    /*! A checkbox, which hints whether cell edit has been selected for merge or split
     */
    QCheckBox* m_checked;
};

}

}
