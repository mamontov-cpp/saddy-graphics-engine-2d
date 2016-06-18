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

#include <ui_mainpanel.h>

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

#include "gridandoffset.h"


#pragma once

namespace core
{
class Editor;
}
namespace gui
{
class WayActions;
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
        \returns key-value storage for database properties delegates
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
    /*! Takes delegate ownership by property name
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
    /*! Toggles, whether animation is editable
        \param[in] flag a flag
     */
    void toggleAnimationPropertiesEditable(bool flag);
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
	/*! Returns grid and offsets
		\return grid and offsets window
	 */ 
	GridAndOffsets* gridAndOffset() const;
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
	/*! A grid and offsets window
	 */ 
	GridAndOffsets* m_offsets_window;

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
    /*! Inits autocompletion items for console text area
     */
    void initConsoleAutocompletion();
public slots:
    /*! Clears selection
     */
    void clearObjectSelection();
    /*! Fixes texture coordinates in all objects
     */
    void fixTextureCoordinates();
    /*! Shows help on fast mode
     */
    void showFastModeHelp();
    /*! Shows grid and offsets window
     */ 
    void showGridAndOffsetWindow() const;
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
