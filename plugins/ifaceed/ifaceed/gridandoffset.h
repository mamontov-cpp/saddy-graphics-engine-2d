/*! \file gridandoffsets.h
    

    Describes a dialog for setting display grid and global offset of position of camera
 */
#pragma once

#include <QWidget>

#include "ui_gridandoffset.h"

/*! A dialog for picking color with alpha
 */
class GridAndOffsets: public QWidget
{
Q_OBJECT
public:
    /*! Creates new window dialog
		\param[in] parent a parent widget
     */
    GridAndOffsets(QWidget* parent =  NULL);
    /*! Does nothing at the moment
     */
    virtual ~GridAndOffsets();
protected:
    Ui::frmGridAndOffset  m_ui;
};

