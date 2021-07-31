/*! \file layoutcelledit.h

    Defines a layout cell editor for editing
    a layout cell
 */
#pragma once
#include <QTableWidget>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QPair>

#include <layouts/cell.h>

Q_DECLARE_METATYPE(sad::layouts::LengthValue)
Q_DECLARE_METATYPE(sad::layouts::HorizontalAlignment)
Q_DECLARE_METATYPE(sad::layouts::VerticalAlignment)
Q_DECLARE_METATYPE(sad::layouts::StackingType)

namespace gui
{

namespace layouts
{

/*! Describes a simple widget, which is used to edit instances of 
    sad::layouts::Cell and manage it's content
 */
class LayoutCellEdit: public QTableWidget
{
Q_OBJECT
public:
/*! A children provider, which is used to connect widget with other 
    parts of program. Used to get list of possible children and also 
    to get name of children
 */
class ChildrenProvider
{
public:
    /*! Returns a viewable name for a node
        \param[in] node a node
        \return name 
     */
    virtual QString name(sad::SceneNode* node) = 0;
    /*! Returns a list of possible children
        \return list of children
     */
    virtual QVector<QPair<QString, unsigned long long> > possibleChildren() = 0;
    /*! Must be inherited
     */
    virtual ~ChildrenProvider();
};

    /*! Constructs new editor
        \param[in] parent a parent widget
     */
    LayoutCellEdit(QWidget* parent = nullptr);
    /*! This class could be inherited
     */
    virtual ~LayoutCellEdit() override;
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
    virtual QSize sizeHint() const override;
    /*! Whether cell is checked for split/merge
        \return whether it's checked
     */
    bool checked() const;
    /*! Sets values from following cell
        \param[in] cell a cell
     */
    void set(sad::layouts::Cell* cell);
    /*! Sets width for cell
        \param[in] v value
     */
    void setWidth(const sad::layouts::LengthValue& v) const;
    /*! Sets height for cell
        \param[in] v value
     */
    void setHeight(const sad::layouts::LengthValue& v) const;
    /*! Sets horizontal alignment for cell
        \param[in] v value
     */
    void setHorizontalAlignment(sad::layouts::HorizontalAlignment v) const;
    /*! Sets vertical alignment for cell
        \param[in] v value
     */
    void setVerticalAlignment(sad::layouts::VerticalAlignment v) const;
    /*! Sets stacking type for cell
        \param[in] v value
     */
    void setStackingType(sad::layouts::StackingType v) const;
    /*! Sets top padding for cell
        \param[in] v value
     */
    void setPaddingTop(double v) const;
    /*! Sets bottom padding for cell
        \param[in] v value
     */
    void setPaddingBottom(double v) const;
    /*! Sets left padding for cell
        \param[in] v value
     */
    void setPaddingLeft(double v) const;
    /*! Sets right padding for cell
        \param[in] v value
     */
    void setPaddingRight(double v) const;
    /*! Removes child from a cell. Note, that this function DOES NOT emit
        corresponding signal
        \param[in] child a child position
     */
    void removeChild(size_t child) const;
    /*! Adds child to a cell. Note, that this function DOES NOT emit 
        corresponding signal
        \param[in] node a node, that is being added
     */
    void addChild(sad::SceneNode* node) const;
    /*! Inserts child to a cell. Note, that this function DOES NOT emit
        a signal
        \param[in] node a node
        \param[in] pos a position
     */
    void insertChild(sad::SceneNode* node, size_t pos) const;
    /*! Clears children of cell. Note, that this function DOES NOT emit 
        a signal
     */
    void clearChildren() const;
    /*! Removes a child from a cell. Note, that this function DOES NOT emit
        a signal
        \param[in] pos position
     */
    void removeChild(size_t pos);
    /*! Swaps children of cell. Note, that this function DOES NOT emit
        a signal
        \param[in] pos1 first position
        \param[in] pos2 second position
     */
    void swapChildren(size_t pos1, size_t pos2) const;
    /*! Updates child name, according to current position
        \param[in] pos position
        \param[in] name a name of child
     */
    void updateChildName(size_t pos, const QString& name) const;
    /*! Sets children provider for editor
        \param[in] p a provider
     */
    void setChildrenProvider(gui::layouts::LayoutCellEdit::ChildrenProvider* p);
    /*! Toggles, whether child editing is enabled
        \param[in] enabled whether it's enabled
     */
    void setChildEditingEnabled(bool enabled);
signals:
    /*! Emitted, when width is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for width
     */
    void widthChanged(size_t row, size_t col, sad::layouts::LengthValue new_value);
    /*! Emitted, when width is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for width
     */
    void heightChanged(size_t row, size_t col, sad::layouts::LengthValue new_value);
    /*! Emitted, when horizontal alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for horizontal alignment
     */
    void horizontalAlignmentChanged(size_t row, size_t col, sad::layouts::HorizontalAlignment new_value);
    /*! Emitted, when vertical alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for vertical alignment
     */
    void verticalAlignmentChanged(size_t row, size_t col, sad::layouts::VerticalAlignment new_value);
    /*! Emitted, when stacking type is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for stacking type
     */
    void stackingTypeChanged(size_t row, size_t col, sad::layouts::StackingType new_value);
    /*! Emitted, when top padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void topPaddingChanged(size_t row, size_t col, double new_value);
    /*! Emitted, when bottom padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void bottomPaddingChanged(size_t row, size_t col, double new_value);
    /*! Emitted, when left padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void leftPaddingChanged(size_t row, size_t col, double new_value);
    /*! Emitted, when right padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void rightPaddingChanged(size_t row, size_t col, double new_value);
    /*! Emitted, when child is added
        \param[in] row a row
        \param[in] col a column
        \param[in] majorid a major id 
        \param[in] node_name a name for a node
     */
    void childAdded(size_t row, size_t col, unsigned long long majorid, QString node_name);
    /*! Emitted, when child is removed
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position, where child is added
     */
    void childRemoved(size_t row, size_t col, size_t pos);
    /*! Emitted, when clearing cell is called
        \param[in] row a row
        \param[in] col a column
     */
    void cleared(size_t row, size_t col);
    /*! Emitted, when children are swapped
        \param[in] row a row
        \param[in] col a column
        \param[in] pos1 a first position of item
        \param[in] pos2 a second position of item
     */
    void childrenSwapped(size_t row, size_t col, size_t pos1, size_t pos2);
public slots:
    /*! Called, when width value changed
        \param[in] new_value a new value for cell
     */
    void widthValueChanged(double new_value);
    /*! Called, when width unit changed
        \param[in] unit a new unit for cell
     */
    void widthUnitChanged(int unit);
    /*! Called, when height value changed
        \param[in] new_value a new height value for cell
     */
    void heightValueChanged(double new_value);
    /*! Called, when height unit changed
        \param[in] unit a new unit for cell
     */
    void heightUnitChanged(int unit);
    /*! Called, when horizontal alignment is changed by user
        \param[in] v new value
     */
    void horizontalAlignmentValueChanged(int v);
    /*! Called, when vertical alignment is changed by user
        \param[in] v new value
     */
    void verticalAlignmentValueChanged(int v);
    /*! Called, when stacking type alignment is changed by user
        \param[in] v new value
     */
    void stackingTypeValueChanged(int v);
    /*! Called, when top padding value changed
        \param[in] new_value a new value
     */
    void topPaddingValueChanged(double new_value);
    /*! Called, when bottom padding value changed
        \param[in] new_value a new value
     */
    void bottomPaddingValueChanged(double new_value);
    /*! Called, when left padding value changed
        \param[in] new_value a new value
     */
    void leftPaddingValueChanged(double new_value);
    /*! Called, when right padding value changed
        \param[in] new_value a new value
     */
    void rightPaddingValueChanged(double new_value);
    /*! Called, when "Add" button is clicked
     */
    void addChildClicked();
    /*! Called, when "Remove" button is clicked
     */
    void removeChildClicked();
    /*! Called, when "Clear" button is clicked
     */
    void clearClicked();
    /*! Called, when "Move back" button is clicked
     */
    void moveBackClicked();
    /*! Called, when "Move front" button is clicked
     */
    void moveFrontClicked();    
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
    /*! A widget for vertical alignment
     */
    QComboBox* m_vertical_alignment;
    /*! A widget for horizontal alignment
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
    /*! Clear button
     */
    QPushButton* m_clear;
    /*! Returns a children provider for cell  edit
     */
    gui::layouts::LayoutCellEdit::ChildrenProvider* m_children_provider;
};

}

}
