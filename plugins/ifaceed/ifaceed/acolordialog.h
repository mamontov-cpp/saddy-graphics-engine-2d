/*! \file acolordialog.h
    

    Describes a dialog for picking color with alpha, since standard dialog is not enough
 */
#pragma once

#include <QDialog>

#include "gui/colorpicker/colorpicker.h"

#include "ui_acolordialog.h"

/*! A dialog for picking color with alpha
 */
class AColorDialog: public QDialog
{
Q_OBJECT
public:
    /*! Creates new color
     */
    AColorDialog();
    /*! Sets palette for color dialog
        \param[in] palette a palette
     */
    void setColorPalette(const QList<QList<QColor> > & palette);
    /*! Returns palette from dialog
        \return palette from dialog
     */
    QList<QList<QColor> > colorPalette() const;
    /*! Sets selected color
        \param[in] color a selected color in dialog
     */
    void setSelectedColor(const QColor & color);
    /*! Returns selected color
        \return selected color
     */
    QColor selectedColor() const;
    /*! Destructs color
     */
    virtual ~AColorDialog();
protected:
    Ui::Dialog  m_ui;
};

