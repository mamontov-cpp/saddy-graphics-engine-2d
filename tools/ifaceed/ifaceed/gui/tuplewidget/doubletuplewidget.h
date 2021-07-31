/*! \file doubletuplewidget.h
    

    A widget for editing a pair of double values
 */
#pragma once
#include <QTableWidget>
#include <QString>
#include <QPair>
#include <QDoubleSpinBox>

namespace gui
{
    
namespace tuplewidget
{
    
/*! Creates new widget for editing  pair of double values
 */
class DoubleTupleWidget: public QTableWidget
{
Q_OBJECT
public:
    /*! Constructs new widget
        \param[in] parent a parent widget
     */
    DoubleTupleWidget(QWidget* parent = nullptr);
    /*! Could be inherited
     */
    virtual ~DoubleTupleWidget();
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
    void setRange(double minimum, double maximum);
    /*! Returns range for spinboxes
        \return range for spinboxes
     */
    QPair<double, double> range() const;
    /*! Sets both values for a widget
        \param[in] first a first value
        \param[in] second a second value
     */
    void setValue(double first, double second);
    /*! Returns a value for a widget
        \return value for a widget
     */
    QPair<double, double> value() const;
signals:
    /*! Emitted, when one of combo is changed
        \param[in] first a first value
        \param[in] second a second value
     */
    void valueChanged(double first, double second);
protected slots:
    /*! Called when first spin box is changed
        \param[in] v value
     */
    void firstChanged(double v);
    /*! Called when second spin box is changed
        \param[in] v value
     */
    void secondChanged(double v);
protected:
    /*! A first long spin box
     */
    QDoubleSpinBox * m_first;
    /*! A second long spin box
     */
    QDoubleSpinBox * m_second;
};

}

}