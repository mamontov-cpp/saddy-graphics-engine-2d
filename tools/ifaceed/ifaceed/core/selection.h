/*! \file selection.h
    

    Defines a selection for an editor
 */
#pragma once
#include <QObject>
#include <QTimer>

#include <sadvector.h>
#include <scenenode.h>

#include <layouts/grid.h>

#include <input/events.h>

namespace core
{

namespace borders
{
class ResizeHotspot;
}

class Editor;

/*! A selection algorithm for an editor
 */
class Selection: public QObject
{
Q_OBJECT
public:
    /*! Creates new unlinked selection
     */
    Selection();
    /*! Can be inherited
     */
    virtual ~Selection();
    /*! Sets editor for selection
        \param[in] e editor
     */
    void setEditor(core::Editor* e);
    /*! Returns editor
        \return editor
     */
    core::Editor* editor() const;
    /*! Defines a timeout for finishing selecting object in milliseconds
     */
    static const int TIMEOUT;
    /*! Tries to select an object on mouse press event
        \param[in] e event object
     */
    void trySelect(const sad::input::MousePressEvent& e);
    /*! Navigates selection on mouse wheel
        \param[in] e event object
     */
    void navigateSelection(const sad::input::MouseWheelEvent& e);
    /*! Navigates scene node selection on mouse wheel
        \param[in] e event object
     */
    void navigateSceneNodeSelection(const sad::input::MouseWheelEvent& e);
    /*! Navigates grid selection on mouse wheel
        \param[in] e event object
     */
    void navigateGridSelection(const sad::input::MouseWheelEvent& e);
    /*! Tries to enter to moving state, with selected object
        \param[in] e event object
     */
    void tryEnterToMovingStateWithWayObject(const sad::input::MousePressEvent& e);
    /*!  Is selection chain pending
     */
    bool isSelectionPending() const;
    /*! Cancels grid selection if any selected, or quits
     */
    void cancelGridSelectionOrQuit();
protected slots:
    /*! Disables navigation on scene nodes on selection
     */
    void disableSceneNodeSelectionNavigation();
    /*! Disables navigation on grids on selection
     */
    void disableGridSelectionNavigation();
    /*! Starts/restart timer for navigation on selection chain for scene nodes
     */
    void startSceneNodeNavigationTimer();
    /*! Starts/restart timer for navigation on selection chain for grids
     */
    void startGridNavigationTimer();
    /*! Forces editor to enter moving state
        \param[in] e event object
        \return whether we should stop handling selection
     */
    bool forceEditorEnterMovingState(const sad::input::MousePressEvent& e) const;
    /*! Forces editor to enter grid moving state
        \param[in] e event object
        \return whether we should stop handling selection
     */
    bool forceEditorEnterGridMovingState(const sad::input::MousePressEvent& e) const;
    /*! Forces editor to enter resizing state
        \param[in] h hotspot for resizing
        \param[in] e event object
     */
    void forceEditorEnterResizingState(
        core::borders::ResizeHotspot* h,
        const sad::input::MousePressEvent& e
    );
    /*! Forces editor to enter resizing state for grids
        \param[in] h hotspot for resizing
        \param[in] e event object
     */
    void forceEditorEnterGridResizingState(
        core::borders::ResizeHotspot* h,
        const sad::input::MousePressEvent& e
    );
    /*! Removes item from selection
     */
    void removeItem();
protected:
    /*! An editor, linked to selection
     */
    core::Editor* m_editor;
    /*! A timer for handling some navigation on nodes. Triggering this timer should disable
        navigation on them.
     */
    QTimer m_scene_node_nav_timer;
    /*! Whether we should change selected node on mouse wheel.
     */
    bool m_scene_node_selection_change;
    /*! A selection chain for editing items
     */
    sad::Vector<sad::SceneNode*> m_scene_node_selection_chain;
    /*! A current position of element in navigation chain
     */
    int m_scene_node_current_position;
    /*! A timer for handling some navigation on grids. Triggering this timer should disable
        navigation on them.
     */
    QTimer m_grid_nav_timer;
    /*! Whether we should change selected grid on mouse wheel.
     */
    bool m_grid_selection_change;
    /*! A selection chain for editing grids
     */
    sad::Vector<sad::layouts::Grid*> m_grid_selection_chain;
    /*! A position for selecting current grid
     */
    int m_current_grid_chain_position;  
    /*! Tries to select an object on mouse press event
        \param[in] e event object
     */
    void trySelectGrid(const sad::input::MousePressEvent& e);
    /*! Tries to select an object on mouse press event
        \param[in] e event object
     */
    void trySelectObject(const sad::input::MousePressEvent& e);
    /*! Tries to select a way on mouse press event
        \param[in] e event object
     */
    void trySelectWay(const sad::input::MousePressEvent& e);
    /*! Commits selection of way
        \param[in] i index of way
        \param[in] j  index of way point
     */
    void commitWaySelection(int i, int j);
    /*! Commits idle way selection
     */
    void commitIdleWaySelection();
};

}
