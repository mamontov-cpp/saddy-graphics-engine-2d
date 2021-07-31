/*! \file fontsizewidget\fontsizewidget.h
    

    Describes a widget, which displays font sizes
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

namespace gui
{

namespace rectwidget
{

/*! This widget is used to pick and maintain non-rotated rectangle
    for sprite and rectangle
 */ 
class RectWidget: public QWidget
{
    Q_OBJECT
public:
    /*! Constructs new widget, placing vertical layout and creating labels, button and combo box
        \param[in] parent a parent widget
     */
    RectWidget(QWidget * parent = nullptr);
    /*! Sets a value for a widget.
        \param[in] value a value for a widget
     */
    void setValue(const QRectF & value);
    /*! Returns a rectangle, described by data in widget
        \return a value of rectangle
     */
    QRectF value() const;
signals:
    /*! Emitted, when any of values are changed
        \param[in] rect a rectangle
     */
    void valueChanged(QRectF rect);
protected slots:
    /*! Emitted, when value of x coordinate of spin box changed
        \param[in] d a value of x
     */
    void xChanged(double d);
    /*! Emitted, when value of y coordinate of spin box changed
        \param[in] d a value of y
     */
    void yChanged(double d);
    /*! Emitted, when value of width in spin box changed
        \param[in] d a width
     */
    void widthChanged(double d);
    /*! Emitted, when value of height in spin box changed
        \param[in] d a width
     */
    void heightChanged(double d);
protected:	
    /*! A layout, where all elements are placed
     */
    QGridLayout * m_layout;
    /*! A full pack of labels in rectangle
     */
    QLabel * m_labels[4];
    /*! A widget, responsible for x coordinate 
     */
    QDoubleSpinBox * m_x;
    /*! A widget, responsible for y coordinate 
     */
    QDoubleSpinBox * m_y;
    /*! A widget, responsible for width 
     */
    QDoubleSpinBox * m_width;
    /*! A widget, responsible for height 
     */
    QDoubleSpinBox * m_height;
};

}

}
