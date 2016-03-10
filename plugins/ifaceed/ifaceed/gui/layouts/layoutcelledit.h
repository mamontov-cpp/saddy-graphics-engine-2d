/*! \file layoutcelledit.h

    Defines a layout cell editor for editing
    a layout cell
 */
#include <QTableWidget>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <vector>

#include <layouts/cell.h>

Q_DECLARE_METATYPE(sad::layouts::LengthValue)
Q_DECLARE_METATYPE(sad::layouts::HorizontalAlignment)
Q_DECLARE_METATYPE(sad::layouts::VerticalAlignment)
Q_DECLARE_METATYPE(sad::layouts::StackingType)

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
    /*! Returns size hint
        \return size hint
     */
    virtual QSize sizeHint() const;
protected:
    /*! A checkbox, which hints whether cell edit has been selected for merge or split
     */
    QCheckBox* m_checked;
    /*! A widget for width value
     */
    QDoubleSpinBox* m_width_value;
    /*! A widget for width value
     */
    QComboBox* m_width_unit;
    /*! A widget for height value
     */
    QDoubleSpinBox* m_height_value;
    /*! A widget for height unit
     */
    QComboBox* m_height_unit;
    /*! A widget for vertical alignmet
     */
    QComboBox* m_vertical_alignment;
    /*! A widget for horizontal alignmet
     */
    QComboBox* m_horizontal_alignment;
    /*! A widget for stacking type
     */
    QComboBox* m_stacking_type;
    /*! A top padding for cell
     */
    QDoubleSpinBox* m_padding_top;
    /*! A bottom padding for cell
     */
    QDoubleSpinBox* m_padding_bottom;
    /*! A left padding for cell
     */
    QDoubleSpinBox* m_padding_left;
    /*! A right padding for cell
     */
    QDoubleSpinBox* m_padding_right;
    /*! A list of children
     */
    QComboBox* m_children;
    /*! Moves children back
     */
    QPushButton* m_move_back;
    /*! Moves children front
     */
    QPushButton* m_move_front;
    /*! Add children button
     */
    QPushButton* m_add;
    /*! Remove children button
     */
    QPushButton* m_remove;
};

}

}
