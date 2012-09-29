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


// A future result of loading database
class DBLoadingTaskFuture
{
 protected:
	 bool m_result;
	 bool m_computed;
 public:
	 inline DBLoadingTaskFuture()
	 {
		 m_result = false;
		 m_computed = false;
	 }

	 inline void setResult(bool result) 
	 {
		 m_result = result;
		 m_computed = true;
	 }

	 inline bool result() 
	 {
		 while (!m_computed) {}
		 return m_result;
	 }
};


// An aynchronous task used to loading some sprite database. Because loading must be performed in
// a rendering thread, because no OpenGL context is available to other threads
class DBLoadingTask: public sad::CountableTask
{
 protected:
	 FontTemplatesMaps * m_maps; //!< Maps data
	 FontTemplateDatabase * m_db;  //!< Database for loading
	 DBLoadingTaskFuture * m_future; //!< Future for computing
 public:
	 /** Constructs new tasks
	  */
	 inline DBLoadingTask(FontTemplatesMaps * maps, FontTemplateDatabase * db, DBLoadingTaskFuture * f)
	 {
		 m_maps = maps;
		 m_db = db;
		 m_future = f;
	 }
	 // Loads a db
	 virtual void perform()
	 {
		m_future->setResult(m_db->load(*m_maps));
	 }
	 virtual ~DBLoadingTask()
	 {
	 }
};

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
		DBLoadingTaskFuture * future = new DBLoadingTaskFuture();
		DBLoadingTask * task = new DBLoadingTask(&maps,db,future);
		// Locking rendering due to adding of new task
		this->lockRendering();
		sad::Input::inst()->addPreRenderTask(task);
		this->unlockRendering();
		if (future->result())
		{
			this->setDatabase(db);
		}
		else
		{
			success = false;
			QTimer::singleShot(0, this->panel(), SLOT(close()));
			delete db;
		}
		delete future;
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
