/*! \file   mainpanel.h
 *  \author HiddenSeeker
 *
 *  Here described main editor window
 */
#ifndef MAINPANEL_H
#define MAINPANEL_H


#include <QtGui/QMainWindow>
#include "ui_mainpanel.h"

#include <sadstring.h>
#include <sadptrhash.h>

#include <db/dbvariant.h>
#include <db/dbstoredpropertyfactory.h>

#include "gui/table/delegatefactory.h"

#include <input/events.h>

#pragma once

namespace core
{
class Editor;
}
namespace gui
{
class SceneNodeActions;
class LabelActions;
class Sprite2DActions;
class CustomObjectActions;
}
class AbstractScreenObject;


/*! A main window to edit level objects
 */
class MainPanel : public QMainWindow
{
Q_OBJECT
public:
	/*! Creates new window
		\param[in] parent a parent widget
		\param[in] flags a panel flags
	 */
	MainPanel(QWidget *parent = 0, Qt::WFlags flags = 0);
	/*! Currently does nothing
	 */
	~MainPanel();
	/*! Sets enabled property for editing buttons on panel to specified property
		\param[in] enabled whether buttons are enabled
	 */
	void toggleEditingButtons(bool enabled);
	/*! Determines, whether editing is enabled in main panel
	 */
	bool isEditingEnabled() const;
    /*! Sets an editor to work with
		\param[in] editor editor
	 */
	void setEditor(core::Editor * editor);
    /*! Returns editor
        \return editor
     */
    core::Editor* editor() const;
	/*! Returns actions for scene nodes
	    \return actions for scene nodes
	 */
	gui::SceneNodeActions* sceneNodeActions() const;
    /*! Returns label actions
     *  \return label actions 
     */
    gui::LabelActions* labelActions() const;
	/*! Returns sprite actions
     *  \return sprite actions 
     */
	gui::Sprite2DActions* sprite2DActions() const;
	/*! Returns actions for custom object
     *  \return actions for custom object 
     */
	gui::CustomObjectActions* customObjectActions() const;
    /*! Returns ui for main panel
        \return main panel's ui
     */
    Ui::MainPanelClass* UI();
	/*! Check database for consistency and set palettes, after database was loaded
	 */
	void viewDatabase();
	/*! Returns palette for main palette
		\return palette
	 */
	QList<QList<QColor> > colorPalette() const;
	/*! Sets palette for node's palette
	 */
	void setColorPalette(const QList<QList<QColor> >& palette);
	/*! Takes delegate ownership by propert name
		\param[in] name a name of delegate by property
		\return if ownership is taken - returns true, if panel does not own it - false
	 */
	bool takeDelegateByPropertyName(const QString & name);
	/*! Adds scene to scene list
		\param[in] s scene 
	 */
	void addSceneToSceneList(sad::Scene* s);
	/*! Removes last scene from scene list
	 */
	void removeLastSceneFromSceneList();
	/*! Inserts scene to a scene list
		\param[in] s scene
		\param[in] position a position in scene list
	 */
	void insertSceneToSceneList(sad::Scene* s, int position);
	/*! Removes scene from a scene list
		\param[in] position a position, where scene must be removed
	 */
	void removeSceneFromSceneList(int position);
	/*! Returns current scene for main panel
		\return current scene
	 */
	sad::Scene* currentScene();
	/*! Updates scene name, finding it in list, settting it to current name
		\param[in] s scene
	 */
	void updateSceneName(sad::Scene* s);
	/*! Finds scene in scene list
		\param[in] s scene
		\return scene row
	 */
	int findSceneInList(sad::Scene* s);
	/*! Sets scenes' positions in list
		\param[in] s1 first scene
		\param[in] s2 second scene
		\param[in] pos1 position of first scene
		\param[in] pos2 position of second scene
	 */
	void setScenesInList(sad::Scene* s1, sad::Scene* s2, int pos1, int pos2);
	/*! Updates labels with coordinates on mouse move
		\param[in] e event objects
	 */
	void updateMousePosition(const sad::input::MouseMoveEvent & e);
    /*! Makes all resource tree views view item to currrent tree. Must be called
        after database is updated
     */
    void updateResourceViews();
    /*! Sets state label's text to specified value
		\param[in] text a label's text
	 */
	void highlightState(const sad::String & text);
    /*! Highlights idle state
     */
    void highlightIdleState();
	/*! Highlights selected state
	 */
	void highlightSelectedState();
	/*! Highlights state, when label is adding
	 */
	void highlightLabelAddingState();
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
	/*! Updates scene node name, finding it in list, settting it to current name
		\param[in] s scene
	 */
	void updateSceneNodeName(sad::SceneNode* s);
	/*! Returns check box for visibility property
		\return checkbox
	 */
	QCheckBox* visibilityCheckbox() const;
	/*! Returns check box for flipX property
		\return checkbox
	 */
	QCheckBox* flipXCheckbox() const;
	/*! Returns check box for flipY property
		\return checkbox
	 */
	QCheckBox* flipYCheckbox() const;
    /*! Cleans table of properties of custom object
     */
    void clearCustomObjectPropertiesTable();
    /*! Finds delegate for custom object property in table
     * \param[in] name a name for delegate for custom object
     * \return found delegate, or NULL
     */
    gui::table::Delegate* delegateForCustomObjectProperty(const QString& name);
    /*! Updates custom object property value in UI, if it's selected
     *  \param[in] node a node, which must be selected
     *  \param[in] name a name of property
     *  \param[in] value a value for a property
     */
    void updateCustomObjectPropertyValue(
            sad::SceneNode* node,
            const sad::String& name,
            const sad::db::Variant& value
     );
	/*! Fills list of properties for custom objects
		\param[in] node a node object
	 */
	void fillCustomObjectProperties(
		sad::SceneNode* node	
	);
	/*! Selects last scene node in panel
	 */
	void selectLastSceneNode();
	/*! Synchronizes database with an editor
	 */
	void synchronizeDatabase();
	/*! Sets, whether adding or removing is enabled
		\param[in] enabled enabled flag
	 */
	void setAddingEnabled(bool enabled);
	/*! Sets, sprite changing enabled
		\param[in] enabled enabled flag
	 */
	void setSpriteChangingEnabled(bool enabled);
	/*! Enables angle changes. If false, also sets angle to zero
		\param[in] enabled 
	 */
	void setAngleChangingEnabled(bool enabled);	
	/*! Clears database properties
	 */
	void clearDatabaseProperties();
public slots:
	/*! Fires signal for updating UI to selected item property values
	 */
	void updateUIForSelectedItem();
	/*! Updates values for updating UI to selected item property values
	 */
	void updateUIForSelectedItemNow();
    /*! Updates value for updating custom object property in UI now
     */
    void updateCustomObjectPropertyValueNow();
protected:
    /*! Stores all widget for main panel
     */
    Ui::MainPanelClass ui;
    /*! Current editor, used to work width
     */
    core::Editor*   m_editor;
    /*! A property delegates, which belongs only to a loaded database and were here
        before user added any property
     */
    sad::PtrHash<sad::String, gui::table::Delegate> m_property_delegates;
    /*! A delegate factory for creating rows in db
     */
    gui::table::DelegateFactory m_dbdelegate_factory;
	/*! An actions, linked to scene nodes editing
	 */
	gui::SceneNodeActions* m_scene_node_actions;
	/*! An actions, linked to label editing
	 */
	gui::LabelActions* m_label_actions;
	/*! An actions, linked to sprite editing
	 */
	gui::Sprite2DActions* m_sprite2d_actions;
	/*! An actions, linked to custom object editing
	 */
	gui::CustomObjectActions* m_custom_object_actions;
    /*! A factory for creating propertis in database
     */
    sad::db::StoredPropertyFactory m_property_factory;
    /*! A point, which coordinates must be assigned to labels, when called
        MainPanel::updateMousePositionNow
     */
    sad::Point2D m_mousemove_point;
    /*! State of main panel, which must be set, when called MainPanel::highlightStateNow
     */
    QString m_highlight_state;
    /*! A temporary value for custom object property name slot, which accessed in
        MainPanel::updateCustomObjectPropertyValueNow
     */
    sad::String m_custom_object_property_name;
    /*! A temporary value for custom object propert value slot, which accessed in
        MainPanel::updateCustomObjectPropertyValueNow
     */
    sad::db::Variant m_custom_object_property_value;
    /*! An utility flags, which could be used to prevent recursive calls of slots, due to
        widget value changes
     */
    bool m_selfchanged;

	/*! Fills combo box with database property types
	 */
	void fillDatabasePropertyTypesCombo();	
	/*!
	 * Whether panel is closed it must close a dialogs if present
	 */
	void closeEvent(QCloseEvent*);
	/*! Fixes database scenes and scene nodes tables and palette if need to
	 */
	void fixDatabase();
	/*! Fetches object name, which can be used in object list
		\param[in] o object
		\return object name
	 */
	QString viewableObjectName(sad::db::Object* o);
protected slots:
	/*! Adds a property to database slot
	 */
	void addDatabaseProperty();
	/*! Adds new scene to a database
	 */
	void addScene();
	/*! Emitted, when current scene is chaned
		\param[in] index index of scene in list
	 */
	void currentSceneChanged(int index);
	/*! Emitted, when scene name is changed
		\param[in] name new scene name
	 */
	void sceneNameChanged(const QString& name);
	/*! Emitted, when current scene node is changed
		\param[in] index a new index for node in list
	 */
	void currentSceneNodeChanged(int index);
	/*! Removes scene from a database (making it inactive)
	 */
	void removeScene();
	/*! Moves scene back
	 */
	void sceneMoveBack();
	/*! Moves scene front
	 */
	void sceneMoveFront();
	/*! Clears scene
	 */
	void clearScene();
	/*! A slot version of MainPanel::updateMousePosition, which takes value 
		from MainPanel::m_mousemove_point
	 */
	void updateMousePositionNow();
	/*! A slot version of MainPanel::highlightState, which takes value 
		from MainPanel::m_highlight_state
	 */
	void highlightStateNow();
	/*! Undoes editor actions 
	 */
	void undo();
	/*! Redoes editor actions
	 */
	void redo();
	/*! Moves scene back
	 */
	void sceneNodeMoveBack();
	/*! Moves scene front
	 */
	void sceneNodeMoveFront();
	/*! Tries to save database
	 */
	void save();
	/*! Tries to save database with prompting user
	 */
	void saveAs();
	/*! Tries to load database
	 */
	void load();
	/*! Cleans database and loads new resources
	 */
	void loadResources();
	/*! Tries to reload resources
	 */
	void reloadResources();
	/*! Emitted, when type of editing objects is changed
		\param[in] index index of new tab
	 */
	void tabTypeChanged(int index);
};

#endif // MAINPANEL_H
