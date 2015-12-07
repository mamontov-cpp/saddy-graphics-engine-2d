#include "mainpanelproxy.h"

#include "../core/editor.h"
#include "../mainpanel.h"


gui::MainPanelProxy::MainPanelProxy() : m_editor(NULL)
{
	
}

void gui::MainPanelProxy::setEditor(core::Editor* editor)
{
	m_editor = NULL;
}

core::Editor* gui::MainPanelProxy::editor() const
{
	return m_editor;
}

gui::MainPanelProxy::~MainPanelProxy()
{
	
}

void gui::MainPanelProxy::lockTypesTab(bool lock)
{
	m_editor->panel()->lockTypesTab(lock);
}

QList<QList<QColor> > gui::MainPanelProxy::colorPalette() const
{
	return m_editor->panel()->colorPalette();
}

void gui::MainPanelProxy::setColorPalette(const QList<QList<QColor> >& palette)
{
	m_editor->panel()->setColorPalette(palette);
}

void gui::MainPanelProxy::highlightState(const sad::String & text)
{
	m_editor->panel()->highlightState(text);
}

void gui::MainPanelProxy::toggleAnimationPropertiesEditable(bool flag)
{
	m_editor->panel()->toggleAnimationPropertiesEditable(flag);
}

void gui::MainPanelProxy::clearCustomObjectPropertiesTable()
{
	m_editor->panel()->clearCustomObjectPropertiesTable();
}

void gui::MainPanelProxy::updateUIForSelectedItem()
{
	m_editor->panel()->updateUIForSelectedItem();
}
