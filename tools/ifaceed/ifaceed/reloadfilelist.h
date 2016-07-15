/*! \file reloadfilelist.h
    

    Describes a dialog for picking file for reloading
 */
#pragma once

#include <QDialog>
#include <QString>

#include "ui_reloadfilelist.h"

#include <resource/physicalfile.h>

/*! A dialog for picking color with alpha
 */
class ReloadFileList: public QDialog
{
Q_OBJECT
public:
    /*! Creates new file list for reloading
        \param[in] parent a parent widget for selecting
     */
    ReloadFileList(QWidget* parent = NULL);
    /*! Returns a list of selected file
     */
    sad::resource::PhysicalFile* selectedFile();
    /*! Could be inherited
     */
    virtual ~ReloadFileList();
protected:
    /*! UI for reloading file list
     */
    Ui::ReloadFileList  m_ui;
};