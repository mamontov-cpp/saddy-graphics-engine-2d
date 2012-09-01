#include "ifaceeditor.h"
#include <ftfont.h>
#include <label.h>
#include <fontmanager.h>



void IFaceEditor::initSaddyRendererOptions()
{
	this->Editor::initSaddyRendererOptions();
	sad::Renderer::instance().setWindowTitle("Saddy Interface Editor");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC, new IFaceEditorHandler(this,&IFaceEditor::quit));

	this->assertSaddyInit(true);
}

QMainWindow * IFaceEditor::createQtWindow()
{
	return new MainPanel();
}

MainPanel * IFaceEditor::panel()
{
	return static_cast<MainPanel*>(this->qtWindow());
}

void IFaceEditor::bindQtSlots()
{

}

void IFaceEditor::onQtWindowDestroy()
{

}

void IFaceEditor::onSaddyWindowDestroy()
{

}
void IFaceEditor::quitQtActions()
{

}
void IFaceEditor::quitSaddyActions()
{

}

void IFaceEditor::quit(const sad::Event & ev)
{
	sad::Renderer::instance().quit();
}