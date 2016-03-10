/*! \file layoutcelledit.h

    Defines a layout cell editor for editing
    a layout cell
 */
#include <QTableWidget>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>


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
    /*! Sets row and column for cell
        \param[in] row a row
        \param[in] col a column
     */
    void setRowAndColumn(size_t row, size_t col);
    /*! Returns size hint
        \return size hint
     */
    virtual QSize sizeHint() const;
    /*! Whether cell is checked for split/merge
        \return whether it's checked
     */
    bool checked() const;
signals:
    /*! Emitted, when width is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for width
     */
    void widthChanged(size_t row, size_t col, sad::layouts::LengthValue newvalue);
    /*! Emitted, when width is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for width
     */
    void heightChanged(size_t row, size_t col, sad::layouts::LengthValue newvalue);
    /*! Emitted, when horizontal alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for horizontal alignment
     */
    void horizontalAlignmentChanged(size_t row, size_t col, sad::layouts::HorizontalAlignment newvalue);
    /*! Emitted, when vertical alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for vertical alignment
     */
    void verticalAlignmentChanged(size_t row, size_t col, sad::layouts::VerticalAlignment newvalue);
    /*! Emitted, when stacking type is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for stacking type
     */
    void stackingTypeChanged(size_t row, size_t col, sad::layouts::StackingType newvalue);
    /*! Emitted, when top padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void topPaddingChanged(size_t row, size_t col, double newvalue);
    /*! Emitted, when bottom padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void bottomPaddingChanged(size_t row, size_t col, double newvalue);
    /*! Emitted, when left padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void leftPaddingChanged(size_t row, size_t col, double newvalue);
    /*! Emitted, when right padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void rightPaddingChanged(size_t row, size_t col, double newvalue);
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
