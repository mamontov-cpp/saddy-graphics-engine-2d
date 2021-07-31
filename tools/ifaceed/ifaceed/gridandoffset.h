/*! \file gridandoffset.h
    

    Describes a dialog for setting display grid and global offset of position of camera
 */
#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <QWidget>
#include <QPointF>

#include "ui_gridandoffset.h"

#include <sadpoint.h>
#include <sadcolor.h>

namespace sad
{

namespace db
{

class Database;

}

}

namespace gui
{
class MainPanelProxy;
}

/*! A dialog for picking color with alpha
 */
class GridAndOffsets: public QWidget
{
Q_OBJECT
public:
    /*! Creates new window dialog
        \param[in] parent a parent widget
     */
    GridAndOffsets(QWidget* parent =  nullptr);
    /*! Updates global offset in ui
        \param[in] x x coordinate
        \param[in] y y coordinate
     */
    void updateGlobalOffsetInUI(double x, double y) const;
    /*! Updates global offset in ui
        \param[in] p point
     */
    void updateGlobalOffsetInUI(const sad::Point3D& p) const;
    /*! Returns global offset
        \return global offset
     */
    sad::Point3D globalOffset() const;
    /*! Updates, whether grid is enabled
        \param[in] enabled new flag value
     */
    void updateGridEnabled(bool enabled) const;
    /*! Returns, whether grid is enabled
        \return whether grid is enabled
     */
    bool gridEnabled() const;
    /*! Updates grid settings
        \param[in] x x coordinate
        \param[in] y y coordinate
     */
    void updateGridSettings(int x, int y) const;
    /*! Updates grid settings
        \param[in] p settings
     */
    void updateGridSettings(const sad::Point2I& p) const;
    /*! Returns grid settings
        \return grid settings
     */
    sad::Point2I gridSettings() const;
    /*! Does nothing at the moment
     */
    virtual ~GridAndOffsets();
    /*! Set proxy for window
        \param[in] proxy proxy window
     */
    void setProxy(gui::MainPanelProxy* proxy);
    /*! Returns grid color
        \return grid color
     */
    sad::AColor gridColor() const;
    /*! Sets grid color
        \param[in] clr color
     */
    void setGridColor(const sad::AColor& clr) const;    
    /*! Returns default database
        \return database
     */
    static sad::db::Database* database();
public slots:
    /*! Called, when X offset is changed
        \param[in] new_value a new value
     */
    void offsetXChanged(double new_value);
    /*! Called, when Y offset is changed
        \param[in] new_value a new value
     */
    void offsetYChanged(double new_value);
    /*! Called, when enabled checkbox is changed
        \param[in] new_value a new value
     */
    void enabledCheckboxChanged(bool new_value);
    /*! Called, when grid X is changed
        \param[in] new_value a new value
     */
    void gridXChanged(int new_value);
    /*! Called, when grid Y is changed
        \param[in] new_value a new value
     */
    void gridYChanged(int new_value);
    /*! Called, when changing grid color is clicked
     */
    void changeGridColorClicked();
    /*! Handles changing offset for grid
        \param p new value of offset
     */
    void handleChangeOffset(QPointF p);
    /*! Handles changing, whether grid is enabled
        \param v new flag value
     */
    void handleChangeGridEnabled(bool v);
    /*! Handles changing grid settings
        \param p new grid settings
     */
    void handleChangeGridSettings(QPointF p);
    /*! Handles changing grid color
        \param clr grid color
     */
    void handleChangeGridColor(sad::AColor clr);
signals:
    /*! Signal, which is emitted, when global offset is changed
        \param[in] p point
     */ 
    void globalOffsetChanged(QPointF p);
    /*! Emitted, when grid enabled state is changed
        \param[in] new_value a flag state
     */
    void gridEnabledChanged(bool new_value);
    /*! Signal, which is emitted, when grid settings is changed
        \param[in] p point
     */
    void gridSettingsChanged(QPointF p);
    /*! Signal, which is emitted, when grid color is changed
        \param[in] clr color
     */
    void gridColorChanged(sad::AColor clr);
protected:
    /*! UI of window
     */
    Ui::frmGridAndOffset  m_ui;
    /*! A proxy value
     */
    gui::MainPanelProxy* m_proxy;
};

