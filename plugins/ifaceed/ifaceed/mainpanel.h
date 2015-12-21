/*! \file   mainpanel.h
 *  
 *
 *  Here described main editor window
 */
#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QtGlobal>

#ifndef HAVE_QT5
    #define HAVE_QT5 (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#endif

#if  HAVE_QT5
    #include <QtWidgets/QMainWindow>
#else
    #include <QtGui/QMainWindow>
#endif


#ifdef HAVE_QT5
namespace Qt
{
    typedef Qt::WindowFlags WFlags;
}
#endif

#include "ui_mainpanel.h"

#include <sadstring.h>
#include <sadptrhash.h>

#include <maybe.h>

#include <db/dbvariant.h>
#include <db/dbstoredpropertyfactory.h>

#include <animations/animationsgroup.h>

#include "gui/table/delegatefactory.h"

#include <input/events.h>

#include <p2d/app/way.h>
#include <dialogue/dialogue.h>


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
class WayActions;
class DialogueActions;
class AnimationActions;
class InstanceActions;
class GroupActions;
}

namespace scripting
{
class Scripting;
}



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
    /*! Frees all inner data
     */
    virtual ~MainPanel();
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
    /*! A delegates by their property names
     */
    sad::Hash<sad::String, gui::table::Delegate*>& delegatesByName();
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
    /*! Adds group to group list
        \param[in] g group
     */
    void addGroupToList(sad::animations::Group* g);
    /*! Removes group from group list
     */
    void removeLastGroupFromList();
    /*! Insert group to group list
        \param[in] pos a position
        \param[in] g group
     */
    void insertGroupToList(int pos, sad::animations::Group* g);
    /*! Removes group from group list
     */
    void removeGroupFromList(int pos);
    /*! Returns name for animation instance
        \param[in] i instance
        \return name for instance
     */
    QString nameForInstance(sad::animations::Instance* i) const;
    /*! Returns name for animation group
        \param[in] g group
        \return name for group
     */
    QString nameForGroup(sad::animations::Group* g) const;
    /*! Toggles, whether animation is editable
        \param[in] flag a flag
     */
    void toggleAnimationPropertiesEditable(bool flag);
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
    /*! Locks types tab if it should be locked
        \param[in] lock whether it should be looked
     */
    void lockTypesTab(bool lock);
    /*! Tries to update combo boox with animations from tree with list of actual animations
     */
    void updateAnimationsListFromTree();    
    /*! Returns list of resources by filter
        \param[in] root a root folder
        \param[in] prefix a prefix
        \param[in] filter a filter for resources
     */
    static QStringList resourcesByFilter(
        sad::resource::Folder* root, 
        const QString& prefix, 
        const QString& filter
    );
    /*! Adds new scriptable property to a database
        \param[in] propertytype a type of property
        \param[in] propertyname a name of property
        \param[in] fromeditor a from editor
     */
    bool scriptableAddProperty(const sad::String& propertytype, const sad::String& propertyname, bool fromeditor);
	/*! Returns a delegate factory for a panel
		\return delegate factory
	 */
	gui::table::DelegateFactory* delegateFactory() const;
public slots:
    /*! Fires signal for updating UI from selected way
     */
    void updateUIForSelectedWay();
    /*! Updates UI, using properties from current way
     */
    void updateUIForSelectedWayNow();
    /*! Clears selection
     */
    void clearObjectSelection();
    /*! Fixes texture coordinates in all objects
     */
    void fixTextureCoordinates();
protected:
    /*! Stores all widget for main panel
     */
    Ui::MainPanelClass ui;
    /*! Current editor, used to work width
     */
    core::Editor*   m_editor;
    /*! Defines, whether tabTypes widget is locked, and a tab, on which it's locked
     */
    sad::Maybe<int> m_type_locked_tab;
    /*! A property delegates, which belongs only to a loaded database and were here
        before user added any property
     */
    sad::PtrHash<sad::String, gui::table::Delegate> m_property_delegates;
    /*! A list of delegates by their names
     */
    sad::Hash<sad::String, gui::table::Delegate*> m_delegates_by_names;
    /*! A delegate factory for creating rows in db
     */
    gui::table::DelegateFactory m_dbdelegate_factory;
    /*! An actions, linked to scene nodes editing
     */   
    /*! A main scripting capabilities and bindings
     */
    scripting::Scripting* m_scripting;
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
protected slots:
    /*! Adds a property to database slot
     */
    void addDatabaseProperty();
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
