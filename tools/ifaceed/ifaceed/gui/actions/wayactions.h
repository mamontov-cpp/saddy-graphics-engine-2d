/*! \file wayactions.h
    

    Describes a group of actions, linked to ways
 */
#pragma  once

#include <QObject>

#include <input/events.h>

#include <p2d/app/way.h>

#include "abstractactions.h"

namespace gui
{

namespace actions
{

/*! A group of actions, linked to ways
 */	
class WayActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new way actions
        \param[in] parent a parent object
     */
    WayActions(QObject* parent = nullptr);
    /*! This class could be inherited
     */
    virtual ~WayActions();  
    /*! Moves way point, according to pivot point (used in moving sub-state)
        \param[in] e event object
     */
    void moveWayPoint(const sad::input::MouseMoveEvent& e);
    /*! Commits moving of way point,
        according to pivot point (used in moving sub-state)
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
        \param[in] from_editor whether it's being removed from editor
        \param[in] row a row to be found in editor
     */
    void removeWayFromDatabase(sad::p2d::app::Way* w, bool from_editor, int row = -1);
    /*! Adds last way to end of items
        \param[in] way a selected way
     */
    void addLastWayToEnd(sad::p2d::app::Way* way);
    /*! Removes last way from way list
     */
    void removeLastWayFromWayList();
    /*! Inserts way to a way list
        \param[in] s scene node
        \param[in] position a position in scene list
     */
    void insertWayToWayList(sad::p2d::app::Way* s, int position);
    /*! Removes way from a way list
        \param[in] position a position, where scene must be removed
     */
    void removeWayFromWayList(int position);
    /*! Removes way from a way list
        \param[in] s way
     */
    void removeWayFromWayList(sad::p2d::app::Way* s);
    /*! Finds way in way list
        \param[in] s scene
        \return scene row (-1 if not found)
     */
    int findWayInList(sad::p2d::app::Way* s);
    /*!  Updates way name in list
        \param[in] s a way
     */
    void updateWayName(sad::p2d::app::Way* s);
    /*!  Removes row from way point list
         \param[in] row a row index
     */
    void removeRowInWayPointList(int row);
    /*! Returns viewable name for a point
        \param[in] p point
        \return name for point
     */
    QString nameForPoint(const sad::Point2D& p) const;
    /*! Tries to move way by specified vector
        \param[in] ev event
        \param[in] v vector
     */
    void tryMoveWayByVector(const sad::input::KeyPressEvent& ev, const sad::Point2D& v) const;
    /*! Tries to move way by keys
        \param[in] ev event
     */ 
    void tryMoveWayByKeys(const sad::input::KeyPressEvent& ev) const;
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
    /*! Fires signal for updating UI from selected way
     */
    void updateUIForSelectedWay();
    /*! Updates UI, using properties from current way
     */
    void updateUIForSelectedWayNow();
};

}

}
