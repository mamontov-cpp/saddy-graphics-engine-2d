/*! \file mainpanelproxy.h
    
    Defines a simple proxy to ensure, that changing MainPanel would not affect other parts of code
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QList>
#include <QColor>

#include <sadhash.h>
#include <sadpoint.h>
#include <maybe.h>

namespace sad
{
class String;	
class Scene;
class SceneNode;

namespace input
{
class KeyPressEvent;    
}

}

namespace core
{
class Editor;
}

namespace gui
{

namespace table
{
    
class Delegate;
class DelegateFactory;

}

/*! A proxy class for main panel to separate compilation of main panel from influence to other classes
 */
class MainPanelProxy
{
public:
    /*! Creates new actions
     */
    MainPanelProxy();
    /*! Links actions to editor
        \param[in] e editor
     */
    void setEditor(core::Editor* e);
    /*! Returns editor for proxy
        \return editor
     */
    core::Editor* editor() const; 
    /*! Could be inherited
     */
    virtual ~MainPanelProxy();
    /*! Locks types tab if it should be locked
        \param[in] lock whether it should be looked
     */
    void lockTypesTab(bool lock);
    /*! Returns palette for main palette
        \return palette
     */
    QList<QList<QColor> > colorPalette() const;
    /*! Sets palette for node's palette
     */
    void setColorPalette(const QList<QList<QColor> >& palette);
    /*! Sets state label's text to specified value
        \param[in] text a label's text
     */
    void highlightState(const sad::String & text);
    /*! Toggles, whether animation is editable
        \param[in] flag a flag
     */
    void toggleAnimationPropertiesEditable(bool flag);
    /*! Returns a delegate factory for a panel
        \return delegate factory
     */
    gui::table::DelegateFactory* delegateFactory() const;
    /*! Clears object selection
     */
    void clearObjectSelection();
    /*! Sets current tab index of main object type selection widget
        \param[in] index an index
     */
    void setCurrentTabWidgetIndex(int index);
    /*! Sets current tab index for scene node type selection widget
        \param[in] index an index
     */
    void setCurrentTabObjectsIndex(int index);
    /*! Takes delegate ownership by property name
        \param[in] name a name of delegate by property
        \return if ownership is taken - returns true, if panel does not own it - false
     */
    bool takeDelegateByPropertyName(const QString & name); 
    /*! A delegates by their property names
        \returns key-value storage for database properties delegates
     */
    sad::Hash<sad::String, gui::table::Delegate*>& delegatesByName();
    /*! Adds new scriptable property to a database
        \param[in] property_type a type of property
        \param[in] property_name a name of property
        \param[in] from_editor a from editor
     */
    bool scriptableAddProperty(const sad::String& property_type, const sad::String& property_name, bool from_editor);
    /*! Whether fast mode is enabled
        \return fast mode is enabled
     */
    bool isFastModeEnabled() const;
    /*! Fetches scene node name from user interface
        \return scene node name from ui
     */
    QString getSceneNodeNameFromUI() const;
    /*! Set scene nde name in UI
        \param[in] name a name part
     */ 
    void setSceneNodeName(const QString& name) const;
    /*! Invokes color selection dialog
        \param[in] old old value for color
        \return selected color
     */
    sad::Maybe<QColor> trySelectColor(const QColor& old) const;
    /*! Forces to enter global offset editing state
        \param p starting position
     */
    void enterGlobalOffsetEditingState(const sad::Point2D& p);
    /*! Handles global offset change via keyboard
        \param[in] ev event
     */
    void handleGlobalOffsetChange(const sad::input::KeyPressEvent& ev) const;
protected:
    /*! An editor part
     */
    core::Editor* m_editor;
};

}
