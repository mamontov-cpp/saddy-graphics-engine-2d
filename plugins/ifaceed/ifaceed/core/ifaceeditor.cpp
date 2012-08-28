#include "ifaceeditor.h"

void IFaceEditor::initSaddyRendererOptions()
{
	this->Editor::initSaddyRendererOptions();
	sad::Renderer::instance().setWindowTitle("Saddy Interface Editor");
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