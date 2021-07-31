/*! \file fontsizewidget\fontsizewidget.h
    

    Describes a widget, which displays font sizes
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

namespace gui
{

namespace fontsizewidget
{

/*! This widget is used to pick and display data for font sizes
 */ 
class FontSizeWidget: public QWidget
{
    Q_OBJECT
public:
    /*! Constructs new widget, placing vertical layout and creating labels, button and combo box
        \param[in] parent a parent widget
     */
    FontSizeWidget(QWidget * parent = nullptr);
    /*! Sets new value for a widget. Adds it to combo box, if the value is absent int list
        \param[in] value a value for a widget
     */
    void setValue(unsigned int value);
    /*! Returns a value, picked in combo box. Returns 0 if nothing selected in combo box
        \return a value of size, picked in combo box
     */
    unsigned int value() const;
signals:
    /*! Emitted, when new value is picked in combo box
     */
    void valueChanged(unsigned int);
protected slots:
    /*! Emitted, when value in combo box with size is changed
     */
    void sizeChanged(int index);
    /*! Used to show dialog with picking some new size and adding it to combo box
     */
    void addSizeClicked();
protected:	
    /*! A layout
     */
    QVBoxLayout * m_layout;
    /*! "A pick other size" button
     */
    QPushButton * m_add;
    /*! A "size" label
     */
    QLabel * m_size_label;
    /*! A  sizes list
     */
    QComboBox * m_sizes;
};

}

}