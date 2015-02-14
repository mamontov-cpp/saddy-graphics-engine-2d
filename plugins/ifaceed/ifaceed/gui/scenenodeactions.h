/*! \file scenenodeactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to scene node
 */
#pragma once
#include <QObject>
#include <QColor>
#include <QRectF>

#include <scenenode.h>

#include <input/events.h>

class MainPanel;

namespace gui
{
class RotationProcess;
/*! A group of actions, linked to scene nodes
 */
class SceneNodeActions: public QObject
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
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
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
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
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
};

}
