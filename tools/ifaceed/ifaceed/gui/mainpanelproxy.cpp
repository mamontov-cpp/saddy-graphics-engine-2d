#include "mainpanelproxy.h"

#include "../core/editor.h"
#include "../mainpanel.h"
#include "../acolordialog.h"

#include "../blockedclosuremethodcall.h"


gui::MainPanelProxy::MainPanelProxy() : m_editor(nullptr)
{
    
}

void gui::MainPanelProxy::setEditor(core::Editor* editor)
{
    m_editor = editor;
}

core::Editor* gui::MainPanelProxy::editor() const
{
    return m_editor;
}

gui::MainPanelProxy::~MainPanelProxy()
{
    
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::lockTypesTab(bool lock)
{
    m_editor->panel()->lockTypesTab(lock);
}

QList<QList<QColor> > gui::MainPanelProxy::colorPalette() const
{
    return m_editor->panel()->colorPalette();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::setColorPalette(const QList<QList<QColor> >& palette)
{
    m_editor->panel()->setColorPalette(palette);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::highlightState(const sad::String & text)
{
    m_editor->panel()->highlightState(text);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::toggleAnimationPropertiesEditable(bool flag)
{
    m_editor->panel()->toggleAnimationPropertiesEditable(flag);
}

gui::table::DelegateFactory* gui::MainPanelProxy::delegateFactory() const
{
    return m_editor->panel()->delegateFactory();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::clearObjectSelection()
{
    m_editor->panel()->clearObjectSelection();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::setCurrentTabWidgetIndex(int index)
{
    m_editor->panel()->UI()->tabTypes->setCurrentIndex(index);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::setCurrentTabObjectsIndex(int index)
{
    m_editor->panel()->UI()->tabObjects->setCurrentIndex(index);
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool gui::MainPanelProxy::takeDelegateByPropertyName(const QString & name)
{
    return m_editor->panel()->takeDelegateByPropertyName(name);	
}

// ReSharper disable once CppMemberFunctionMayBeConst
sad::Hash<sad::String, gui::table::Delegate*>& gui::MainPanelProxy::delegatesByName()
{
    return m_editor->panel()->delegatesByName();		
}	

// ReSharper disable once CppMemberFunctionMayBeConst
bool gui::MainPanelProxy::scriptableAddProperty(const sad::String& property_type, const sad::String& property_name, bool from_editor)
{
    return m_editor->panel()->scriptableAddProperty(property_type, property_name, from_editor);
}

bool gui::MainPanelProxy::isFastModeEnabled() const
{
    return m_editor->panel()->UI()->cbFastMode->isChecked();
}
 
QString gui::MainPanelProxy::getSceneNodeNameFromUI() const
{
    return m_editor->panel()->UI()->txtObjectName->text();
}

void gui::MainPanelProxy::setSceneNodeName(const QString& name) const
{
   m_editor->emitClosure(blocked_bind(
       m_editor->panel()->UI()->txtObjectName,
       &QLineEdit::setText,
       name
   ));
}

sad::Maybe<QColor>  gui::MainPanelProxy::trySelectColor(const QColor& old) const
{
    sad::Maybe<QColor> color;

    AColorDialog dlg;
    QList<QList<QColor> > palette = m_editor->panelProxy()->colorPalette();
    dlg.setColorPalette(palette);
    dlg.setSelectedColor(old);

    if (dlg.exec() == QDialog::Accepted)
    {
        QColor i = dlg.selectedColor();
        m_editor->panelProxy()->setColorPalette(dlg.colorPalette());
        color.setValue(i);
    }

    return color;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::MainPanelProxy::enterGlobalOffsetEditingState(const sad::Point2D& p)
{
    m_editor->panel()->enterGlobalOffsetEditingState(p);
}

void gui::MainPanelProxy::handleGlobalOffsetChange(const sad::input::KeyPressEvent& ev) const
{
    m_editor->panel()->handleGlobalOffsetChange(ev);    
}