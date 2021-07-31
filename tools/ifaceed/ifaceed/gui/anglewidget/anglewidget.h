/*! \file anglewidget\anglewidget.h
    

    Describes a widget, which displays angle for an item
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

namespace gui
{

namespace anglewidget
{

/*! This widget is used to pick and display data for font sizes
 */ 
class AngleWidget: public QWidget
{
    Q_OBJECT
public:
    /*! Constructs new widget, placing horizontal layout, creating label and combo box
        \param[in] parent a parent widget
     */
    AngleWidget(QWidget * parent = nullptr);
    /*! Sets new value for a widget, in radians. 
        \param[in] value a value for a widget
     */
    void setValue(double value);
    /*! Returns a value, picked in spin box (in radians)
        \return a value 
     */
    double value() const;
    /*! Increments angle in radians, sets it to self and returns new value
        \param[in] angle in radians
        \param[in] increment an increment in degrees
        \return new angle in radians
     */
    double incrementInDegrees(double angle, int increment);
signals:
    /*! Emitted, when new value is picked in spin box (in radians)
     */
    void valueChanged(double a);
protected slots:
    /*! Emitted, when value in spin box is changed
        \param[in] a value
     */
    void angleChanged(int a);
protected:	
    /*! Converts degrees to radians
        \param[in] v value
        \return value in radians
     */
    static double deg2rad(double v);
    /*! Converts radians to degrees
        \param[in] v value
        \return value in degrees
     */
    static double rad2deg(double v);
    /*! A layout
     */
    QHBoxLayout * m_layout;
    /*! A label
     */
    QLabel * m_label;
    /*! An angle part
     */
    QSpinBox * m_angle;
};

}

}
