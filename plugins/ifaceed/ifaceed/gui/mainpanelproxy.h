/*! \file mainpanelproxy.h
	
	Defines a simple proxy to ensure, that changing MainPanel would not affect other parts of code
 */
#pragma once
#include <QList>
#include <QColor>

namespace sad
{
class String;	
class Scene;
class SceneNode;
}

namespace core
{
class Editor;
}

namespace gui
{

namespace table
{

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
protected:
	/*! An editor part
	 */
	core::Editor* m_editor;
};

}
