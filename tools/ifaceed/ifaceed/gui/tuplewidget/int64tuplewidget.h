/*! \file int64tuplewidget.h
    

    A widget for editing a pair of long long values
 */
#pragma once
#include <QTableWidget>
#include <QString>
#include <QPair>
#include "../../qwwlongspinbox.h"

namespace gui
{
    
namespace tuplewidget
{
    
/*! Creates new widget for editing  pair of long long values
 */
class Int64TupleWidget: public QTableWidget
{
Q_OBJECT
public:
    /*! Constructs new widget
        \param[in] parent a parent widget
     */
    Int64TupleWidget(QWidget* parent = nullptr);
    /*! Could be inherited
     */
    virtual ~Int64TupleWidget();
    /*! Sets labels for widget
        \param[in] first a label for first spin box
        \param[in] second a label for second spin box
     */
    void setLabel(const QString& first, const QString& second);
    /*! Returns label for widget
        \return label for widget
     */
    QPair<QString, QString> label() const;
    /*! Sets range for both spinboxes
        \param[in] minimum a minimum value
        \param[in] maximum a maximum value
     */
    void setRange(qlonglong minimum, qlonglong maximum);
    /*! Returns range for spinboxes
        \return range for spinboxes
     */
    QPair<qlonglong, qlonglong> range() const;
    /*! Sets both values for a widget
        \param[in] first a first value
        \param[in] second a second value
     */
    void setValue(qlonglong first, qlonglong second);
    /*! Returns a value for a widget
        \return value for a widget
     */
    QPair<qlonglong, qlonglong> value() const;
signals:
    /*! Emitted, when one of combo is changed
        \param[in] first a first value
        \param[in] second a second value
     */
    void valueChanged(qlonglong first, qlonglong second);
protected slots:
    /*! Called when first spin box is changed
        \param[in] v value
     */
    void firstChanged(qlonglong v);
    /*! Called when second spin box is changed
        \param[in] v value
     */
    void secondChanged(qlonglong v);
protected:
    /*! A first long spin box
     */
    QwwLongSpinBox * m_first;
    /*! A second long spin box
     */
    QwwLongSpinBox * m_second;
};

}

}