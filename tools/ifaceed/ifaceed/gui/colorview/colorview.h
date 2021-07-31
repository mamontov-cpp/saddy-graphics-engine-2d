/*! \file colorview.h
    

    Implements a simple widget for displaying custom color. Used in cells of main panel
    for displaying color
 */
#pragma once
#include <QLabel>

namespace  gui
{
    
namespace colorview
{
    
class ColorView: public QWidget
{
Q_OBJECT
public:
    /*! Creates new view with default color
     */
    ColorView(QWidget * parent = nullptr);
    /*! Could be inherited
     */
    virtual ~ColorView();
    /*! Sets background color for color view
        \param[in] c color
     */
    void setBackgroundColor(const QColor& c);
    /*! Returns background color of color view
        \return color
     */
    const QColor& backgroundColor() const;
signals:
    /*! Emitted, when user is clicked on item
     */
    void clicked();
protected:
    /*! A background color for widget
     */
    QColor m_background_color;
    /*! Paints color picker's palette and parts near palette
        \param[in] e event
     */
    virtual void paintEvent(QPaintEvent * e) override;
    /*! Used to emit clicked to on click
        \param[in] e event object
     */
    virtual void mousePressEvent( QMouseEvent * e ) override;
};

}

}
