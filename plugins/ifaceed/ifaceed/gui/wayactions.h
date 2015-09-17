/*! \file wayactions.h
    

    Describes a group of actions, linked to ways
 */
#pragma  once

#include <QObject>

#include <input/events.h>

#include <p2d/app/way.h>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to ways
 */	
class WayActions: public QObject
{
Q_OBJECT
public:
    /*! Creates new way actions
        \param[in] parent a parent object
     */
    WayActions(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~WayActions();
    /*! Sets panel, where actions belong to
     */
    void setPanel(MainPanel* e);
    /*! Returns panel, where actions belong to
     */
    MainPanel* panel() const;    
    /*! Moves way point, according to pivot point (used in moving substate)
        \param[in] e event object
     */
    void moveWayPoint(const sad::input::MouseMoveEvent& e);
    /*! Commits moving of way point,
        according to pivot point (used in moving substate)
        \param[in] e event object
     */
    void commitWayPointMoving(const sad::input::MouseReleaseEvent& e);
    /*! Updates way point
        \param[in] row a row
        \param[in] p a point for row
     */
    void updateWayPoint(int row, const sad::Point2D& p);
    /*! Removes way from database, adding new command
        \param[in] w a way to be removed 
        \param[in] fromeditor whether it's being removed from editor
        \param[in] row a row to be found in editor
     */
    void removeWayFromDatabase(sad::p2d::app::Way* w, bool fromeditor, int row = -1);
public slots:
    /*! Adds new way to a list
     */
    void addWay();
    /*! Remove ways from a list
     */
    void removeWay();
    /*! Called, when a way changed in list
        \param[in] i new row index
     */
    void wayChanged(int i);
    /*! Called, when way name is edited
        \param[in] name a new name
     */
    void nameEdited(const QString& name);
    /*! Called, when user changes closedness of way
        \param[in] state a state
     */
    void closednessChanged(bool state);
    /*! Called, when user changed total time of way
        \param[in] value value
     */
    void totalTimeChanged(double value);
    /*! Adds new way point to a current way
     */
    void addWayPoint();
    /*! Removes way point from a current way
     */
    void removeWayPoint();
    /*! Forces editor to view point
        \param[in] i point index
     */
    void viewPoint(int i);
    /*! Called, when X coordinate is changed for point
        \param[in] value a new value for X coordinate
     */
    void wayPointXChanged(double value);
    /*! Called, when Y coordinate is changed for point
        \param[in] value a new value for Y coordinate
     */
    void wayPointYChanged(double value);
    /*! Moves back current way point
     */
    void wayPointMoveBack();
    /*! Moves front current way point
     */
    void wayPointMoveFront();    
private:
    /*! An panel, which actions are belong to
     */
    MainPanel* m_panel;
};

}
