#include "ifaceeditor.h"
#include <ftfont.h>
#include <label.h>
#include <fontmanager.h>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include "core/fonttemplatesdatabase.h"

IFaceEditor::IFaceEditor()
{
	m_db = NULL;
}
IFaceEditor::~IFaceEditor()
{
	delete m_db;
}

void IFaceEditor::setDatabase(FontTemplateDatabase * db)
{
	delete m_db;
	m_db = db;
}

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

CommandLineOptions * IFaceEditor::createOptionParser()
{
	return new IFaceCmdOptions();
}

void IFaceEditor::onFullAppStart()
{
	if (this->cmdLineOptions()->hasConfig() == false)
	{
		QMessageBox::warning(NULL, "IFace Editor", "Config file is not specified. You must choose it now");
		QString config = QFileDialog::getOpenFileName(this->panel(),"Choose a config file",QString(),
													  ("Configs (*.xml)"));
		if (config.length() == 0) 
		{
			QMessageBox::critical(NULL, "IFace Editor", "Config file is not specified. Quitting...");
			QTimer::singleShot(0, this->panel(), SLOT(close()));
			return;
		} 
		else 
		{
			this->cmdLineOptions()->setConfig(config);
		}
	}
	bool success = true;
	// Load first stage - a maps of handling all of data
	FontTemplatesMaps maps;
	if (maps.load(this->cmdLineOptions()->config()))
	{
		FontTemplateDatabase * db = new FontTemplateDatabase();
		if (db->load(maps))
		{
			this->setDatabase(db);
		}
		else
		{
			success = false;
			QTimer::singleShot(0, this->panel(), SLOT(close()));
			delete db;
		}
	}
	else 
	{
		success = false;
		QMessageBox::critical(NULL, "IFace Editor", "Can\'t load config file");
		QTimer::singleShot(0, this->panel(), SLOT(close()));
	}
	if (success) {
		this->panel()->setEditor(this);
	}
}

FontTemplateDatabase * IFaceEditor::database()
{
	return m_db;
}
