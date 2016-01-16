/*! \file scenenodeactions.h
    

    Describes a group of actions, linked to scene node
 */
#pragma once
#include <QObject>
#include <QColor>
#include <QRectF>

#include <scenenode.h>

#include <input/events.h>

#include "abstractactions.h"

class MainPanel;

namespace gui
{

class RotationProcess;

namespace actions 
{

/*! A group of actions, linked to scene nodes
 */
class SceneNodeActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new action for scene nodes
        \param[in] parent a parent object
     */
    SceneNodeActions(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~SceneNodeActions();
    /*! Moves object, according to pivot point (used in moving substate)
        \param[in] e event object
     */
    void moveObject(const sad::input::MouseMoveEvent& e);
    /*! Commits moving of object, according to pivot point (used in moving substate)
        \param[in] e event object
     */
    void commitObjectMoving(const sad::input::MouseReleaseEvent& e);
    /*! Resizes object, according to pivot point (used in resizing substate)
        \param[in] e event object
     */
    void resizeObject(const sad::input::MouseMoveEvent& e);
    /*! Commits resizing of object, according to pivot point (used in moving substate)
        \param[in] e event object
     */
    void commitObjectResizing(const sad::input::MouseReleaseEvent& e);
    /*! Tries to navigate selection or rotate object, when user uses mouse wheel
        \param[in] e event
     */
    void navigateOrRotate(const sad::input::MouseWheelEvent& e);
    /*! Tries to rotate object, when user uses mouse wheel
        \param[in] e event
     */
    void rotate(const sad::input::MouseWheelEvent& e);
    /*! Cancels selection of object
     */
    void cancelSelection();
    /*! Updates panel's region for active node
     */
    void updateRegionForNode();
    /*! Removes scene node
        \param[in] node a node to be removed
        \param[in] from_editor true, if node is removed from editor, false if from scripting part
     */
    void removeSceneNode(sad::SceneNode* node, bool from_editor); 
    /*! Selects last scene node in list
     */
    void selectLastSceneNode();
    /*! Updates scene node name, finding it in list, settting it to current name
        \param[in] s scene
     */
    void updateSceneNodeName(sad::SceneNode* s);
    /*! Returns full name for a node
        \param[in] node node, which shoulde be found
        \return name for node
     */
    QString fullNameForNode(sad::SceneNode* node);
    /*! Adds scene to scene node list
        \param[in] s scene node
     */
    void addSceneNodeToSceneNodeList(sad::SceneNode* s);
    /*! Removes last scene node from scene list
     */
    void removeLastSceneNodeFromSceneNodeList();
    /*! Inserts scene node to a scene node list
        \param[in] s scene node
        \param[in] position a position in scene list
     */
    void insertSceneNodeToSceneNodeList(sad::SceneNode* s, int position);
    /*! Removes scene node from a scene node list
        \param[in] position a position, where scene must be removed
     */
    void removeSceneNodeFromSceneNodeList(int position);
    /*! Removes scene node from a scene node list
        \param[in] s scene node
     */
    void removeSceneNodeFromSceneNodeListByNode(sad::SceneNode* s);
    /*! Sets scene nodes' positions in list
        \param[in] n1 first node
        \param[in] n2 second node
        \param[in] pos1 position of first node
        \param[in] pos2 position of second node
     */
    void setSceneNodesInList(sad::SceneNode* n1, sad::SceneNode* n2, int pos1, int pos2);
    /*! Finds scene node in scene list
        \param[in] s scene
        \return scene row (-1 if not found)
     */
    int findSceneNodeInList(sad::SceneNode* s);
    /*! Forces placing first point for spanning object between two points
        \param[in] e event object
     */
    void placeFirstPointForSpanning(const sad::input::MouseReleaseEvent& e);
    /*! Emitted, when user moves second point for spaning object
        \param[in] e event object
     */
    void moveSecondPointForSpanning(const sad::input::MouseMoveEvent& e);
    /*! Emitted, when places second point for spaning object
        \param[in] e event object
     */
    void commitSecondPointForSpanning(const sad::input::MouseReleaseEvent& e);
    /*! Cancel spanning object between two points forcing it to enter object
        \param[in] e event
     */ 
    void cancelSpanningObject(const sad::input::KeyPressEvent& e);
	/*!
	 * Spans selected object in editor, between two points (if can)
	 * \param[in] p1 first point
	 * \param[in] p2 second point
	 */
	void spanObjectBetweenTwoPoints(
		sad::SceneNode* node,
		sad::Point2D p1,
		sad::Point2D p2
	);
	/*! Updates angle for scene node
	 */
	void updateAngleForNode();
public slots:
    /*! Called, when node name is edited
        \param[in] name a name for action
     */
    void nameEdited(const QString& name);
    /*! Called, when user changes visibility of node
        \param[in] state a state
     */
    void visibilityChanged(bool state);
    /*!
     * Called, when user changes color of node
     * \param[in] newcolor a new color
     */
    void colorChanged(QColor newcolor);
    /*!
     * Called, when user changes area of node
     * \param newarea
     */
    void areaChanged(QRectF newarea);
    /*!
     * Called, when user changes angle of node
     * \param newvalue a new value for angle
     */
    void angleChanged(double newvalue);
    /*!
     * Called, when user removes scene node
     */
    void removeSceneNode();
    /*! A slot for selecting last scene node slot
     */
    void selectLastSceneNodeSlot();
    /*! Emitted, when current scene node is changed
        \param[in] index a new index for node in list
     */
    void currentSceneNodeChanged(int index);
    /*! Moves scene back
     */
    void sceneNodeMoveBack();
    /*! Moves scene front
     */
    void sceneNodeMoveFront();
    /*! Fires signal for updating UI from selected item
     */
    void updateUIForSelectedSceneNode();
    /*! Updates UI views with values from selected item
     */
    void updateUIForSelectedSceneNodeNow();
    /*! Makes actions enter state for spanning object between two points
     */
    void enterSpanningObjectBetweenTwoPoints();
private:
    /*!
     * A rotation process to work with
     */
    gui::RotationProcess* m_rotation;
    /*!
     * Computes angle, after mouse wheel
     * \param angle  a previous angle value
     * \param delta  a mouse wheel event delta
     * \return next angle
     */
    float computeChangedAngle(float angle, float delta);
	/*!
	 * Spans selected object in editor, between two points (if can)
	 * \param[in] p1 first point
	 * \param[in] p2 second point
	 */
	void spanSelectedObjectBetweenTwoPoints(
		sad::Point2D p1,
		sad::Point2D p2
	);
};

}

}
