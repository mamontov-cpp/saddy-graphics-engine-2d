/*! \file   shared.h
    

    Defines an editor's data, shared between all editor's states
 */
#pragma once

#include <3rdparty/picojson/picojson.h>

#include <sadstring.h>
#include <scenenode.h>
#include <sadrect.h>

#include <hfsm/hfsmshared.h>

#include <log/log.h>

#include <p2d/vector.h>
#include <p2d/app/way.h>

#include <dialogue/dialogue.h>

#include <animations/animationsanimation.h>
#include <animations/animationsinstance.h>
#include <animations/animationsgroup.h>

#include <layouts/grid.h>

namespace core
{

class Editor;

/*! An editor's data. shared between all editor's state. Contains active and selected objects.
    Active object is an object, which will be placed (in case of state, of object adding)
 */
class Shared: public sad::hfsm::Shared
{
public:	 
    /*! Creates a default empty shared data
     */
    Shared();
    /*! This class could be inherited
     */
    virtual ~Shared();
    /*! Sets editor, which data belong to
        \param[in] e editor
     */
    void setEditor(core::Editor* e);
    /*! Returns an editor, which data belongs to
        \return editor
     */
    core::Editor* editor() const;
    /*! Sets selected object
        \param[in] o object
     */
    void setSelectedObject(sad::SceneNode* o);
    /*! Sets active object
        \param[in] o object
     */
    void setActiveObject(sad::SceneNode* o);
    /*! Returns selected object
        \return selected object
     */
    sad::SceneNode* selectedObject() const;
    /*! Returns active object
        \return active object
     */
    sad::SceneNode* activeObject() const;
    /*! Whether we can show active border
        \return whether we can show active border;
     */
    bool canShowActiveBorder() const;
    /*! Could force hiding active border if set to false. You can disable force hiding by setting it to
        true
        \param[in] flag a flag value
     */
    void toggleActiveBorder(bool flag);
    /*! Sets pivot point for moving or resizing element
        \param[in] p pivot point
     */
    void setPivotPoint(const sad::Point2D& p);
    /*! Returns a pivot point for moving or resizing element
        \return pivot point
     */
    const sad::Point2D& pivotPoint() const;
    /*! Sets old area for  moving or resizing elements
        \param[in] r rectangle
     */
    void setOldArea(const sad::Rect2D& r);
    /*! Returns old area of element for moving or resizing elements
        \return rectangle
     */
    const sad::Rect2D& oldArea() const;
    /*! Whether selected object is resizeable
        \return whether it can be resized
     */
    bool isSelectionResizeable() const;
    /*! Sets indexes of source area for resizing element
        \param[in] indexes an indexes to be resized
     */
    void setResizingIndexes(const sad::Pair<int, int>& indexes);
    /*! Returns indexes of source area for resizing element
        \return indexes of resizing elements
     */
    const sad::Pair<int, int>& resizingIndexes() const;
    /*! Sets a direction for resizing element
        \param[in] direction a direction for resizing
     */
    void setResizingDirection(const sad::p2d::Vector& direction);
    /*! Returns direction for resizing element
        \return direction for resizing element
     */
    const sad::p2d::Vector& resizingDirection() const;
    /*! Sets a normalized direction for resizing element
        \param[in] direction a direction for resizing
     */
    void setNormalizedResizingDirection(const sad::p2d::Vector& direction);
    /*! Returns normalized direction for resizing element
        \return direction for resizing element
     */
    const sad::p2d::Vector& normalizedResizingDirection() const;
    /*! Sets file name for current database
        \param[in] name a name for editor
     */ 
    void setFileName(const QString& name);
    /*! Returns file name for saved database
        \return file name
     */
    const QString& fileName() const;
    /*! Sets old region
        \param[in] r region
     */
    void setOldRegion(const sad::Rect2D& r);
    /*! Returns old region for object
        \return old region
     */
    const sad::Rect2D& oldRegion() const;
    /*! Sets selected way
        \param[in] way a way to be set
     */ 
    void setSelectedWay(sad::p2d::app::Way* way);
    /*! Returns selected way
        \return selected way
     */
    sad::p2d::app::Way* selectedWay() const;
    /*! Sets way point position in way
        \param p position
     */
    void setWayPointPosition(int p);
    /*! Returns way point position
        \return way point position
     */
    int wayPointPosition() const;
    /*! Sets old point value
        \param p point
     */
    void setOldPoint(const sad::Point2D& p);
    /*! Returns old point value
        \return old point
     */
    const sad::Point2D& oldPoint() const;
    /*! Sets selected dialogue
        \param[in] d dialogue
     */ 
    void setSelectedDialogue(sad::dialogue::Dialogue* d);
    /*! Returns selected dialogue
        \return selected dialogue
     */
    sad::dialogue::Dialogue* selectedDialogue() const;
    /*! Sets selected animation
        \param[in] a animation
     */ 
    void setSelectedAnimation(sad::animations::Animation* a);
    /*! Returns selected animation
        \return selected animation
     */
    sad::animations::Animation* selectedAnimation() const;
    /*! Sets selected instance
        \param[in] a an instance instance to be set
     */ 
    void setSelectedInstance(sad::animations::Instance* a);
    /*! Returns selected instance
        \return selected instance
     */
    sad::animations::Instance* selectedInstance() const;
    /*! Sets selected group
        \param[in] a a group
     */ 
    void setSelectedGroup(sad::animations::Group* a);
    /*! Returns selected group
        \return selected group
     */
    sad::animations::Group* selectedGroup() const;
    /*! Sets selected grid
        \param g grid
     */
    void setActiveGrid(sad::layouts::Grid* g);
    /*! Returns selected grid
        \return selected grid
     */
    sad::layouts::Grid* activeGrid() const;
    /*! Sets selected grid 
        \param g grid
     */
    void setSelectedGrid(sad::layouts::Grid* g);
    /*! Returns selected grid
        \return selected grid
     */
    sad::layouts::Grid* selectedGrid() const;
    /*! Tests, whether user is clicked  "Start" on animation, animation instance or animation group
        \return result
     */
    bool isAnyKindOfAnimationIsRunning() const;
    /*! Sets whether animation is running
        \param[in] flag a flag
     */
    void setAnimationIsRunning(bool flag);
    /*! Sets whether animation instance is running
        \param[in] flag a flag
     */
    void setAnimationInstanceIsRunning(bool flag);
    /*! Sets whether animation group is running
        \param[in] flag a flag
     */
    void setAnimationGroupIsRunning(bool flag);
    /*! Sets editing  property for process of editing of SimpleMovement animations
        \param[in] name a name for property
     */
    void setEditingSimpleMovementProperty(const sad::String& name);
    /*! Returns editing  property of SimpleMovement animations
        \return name
     */
    const sad::String& editingSimpleMovementProperty() const;
    /*! Sets old angle of object, for backing it up
        \param[in] angle old angle
     */
    void setOldAngle(double angle);
    /*! Returns old angle
        \return old angle
     */
    double oldAngle() const;
    /*! Sets name for fast mode
        \param[in] name a name for fast mode
     */
    void setNameForFastMode(
        const QString& name
    );
    /*! Returns name for fast mode
        \return name for fast mode
     */
    const QString& nameForFastMode() const;
    /*! Sets, whether action is triggered by fast mode
        \param[in] flag fast mode flag value
     */
    void setTriggeredByFastMode(bool flag);
    /*! Returns, whether action is triggered by fast mode 
        (for those actions, which are triggered by fast mode)
     */
    bool triggeredByFastMode() const;
    /*! Sets old state
        \param[in] state an old state
     */
    void setOldState(const picojson::value& state);
    /*!  Returns old state
         \return old state
     */
    const picojson::value& oldState() const;
    /*! An old global offset for a rendered
     */
    sad::Vector3D OldGlobalOffset;
    /*! An old highlight string for a main panel
     */
    QString OldHighlightString;
    /*! An old editor state
     */
    sad::String OldEditorState;
private:
    /*! A scene node, selected by user
     */ 
    sad::SceneNode* m_selected;
    /*! An active scene node
     */
    sad::SceneNode* m_active;
    /*! A selected way
     */
    sad::p2d::app::Way* m_selected_way;
    /*! A selected dialogue
     */
    sad::dialogue::Dialogue* m_selected_dialogue;
    /*! A selected animation
     */
    sad::animations::Animation* m_selected_animation;
    /*! A selected animation instance
     */
    sad::animations::Instance* m_selected_instance;
    /*! A selected group
     */
    sad::animations::Group* m_selected_group;
    /*! An active grid
     */
    sad::layouts::Grid* m_active_grid;
    /*! A selected grid
     */
    sad::layouts::Grid* m_selected_grid;
    /*! A way point position, when moving point
     */
    int m_way_point_position;
    /*! A temporary old point value
     */
    sad::Point2D m_old_point;
    /*! A linked editor, which shared data belongs to
     */
    core::Editor* m_editor;
    /*! Should we show or hide active border
     */
    bool  m_show_active_border;  
    /*! A pivot point for moving or resizing items
     */
    sad::Point2D m_pivot_point;
    /*! An old area for moving or resizing items
     */
    sad::Rect2D m_old_area;
    /*! An old region for object
     */
    sad::Rect2D m_old_region;
    /*! A node types, which can't be resized
     */
    sad::Vector<sad::String> m_non_resizeable_node_types;
    /*! An indexes, which should be used for resizing objects 
     */
    sad::Pair<int, int> m_resizing_indexes;
    /*! A direction for resizing indexes
     */	 
    sad::p2d::Vector m_resizing_direction;
    /*! A normalized resizing direction
     */
    sad::p2d::Vector m_normalized_resizing_direction;
    /*! A file name
     */
    QString m_filename;
    /*! Set to true, when user clicks "Start" on animation tab
     */
    bool m_animation_is_running;
    /*! Set to true, when user clicks "Start" on animation instance tab
     */
    bool m_animation_instance_is_running;
    /*! Set to true, when user clicks "Start" on animation group tab
     */
    bool m_animation_group_is_running;
    /*! A property for simple movement editing
     */
    sad::String m_editing_simple_movement_property;
    /*! Old angle for all changes of various angle
     */
    double m_old_angle;
    /*! Saves a name for fast mode
     */
    QString m_name_for_fast_mode;
    /*! Whether action is triggered by fast mode
     */
    bool m_triggered_by_fast_mode;
    /*! An od serialized value
     */
    picojson::value m_old_state;
};

}
