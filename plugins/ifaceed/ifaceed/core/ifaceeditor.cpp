#include "ifaceeditor.h"
#include <ftfont.h>
#include <label.h>
#include <fontmanager.h>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include "core/fonttemplatesdatabase.h"
#include "../editorcore/editorbehaviour.h"
#include "../objects/screentemplate.h"
#include "../objects/screenlabel.h"
#include "states/idlestate.h"
#include "states/labeladdingstate.h"
#include "states/selectedstate.h"
#include "objectborders.h"
#include <QTimer>

IFaceEditor::IFaceEditor()
{
	m_db = NULL;
	m_counter = 0;
	m_result = new ScreenTemplate();
	m_selection_border = NULL;

	EditorBehaviour * behaviour = new EditorBehaviour(this,"idle");
	behaviour->addState("idle", new IdleState());
	behaviour->addState("label_adding", new LabelAddingState());
	behaviour->addState("selected", new SelectedState());

	this->behaviours().insert("main", behaviour);
}
IFaceEditor::~IFaceEditor()
{
	delete m_db;
	delete m_result;
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
	 bool m_result;   //!< Result of computation
	 bool m_computed; //!< Computed result
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
	 DBCriticalLogger * m_logger;    //!< Logger for logging data
 public:
	 /** Constructs new tasks
	  */
	 inline DBLoadingTask(FontTemplatesMaps * maps, 
						  FontTemplateDatabase * db, 
						  DBLoadingTaskFuture * f, 
						  DBCriticalLogger * logger)
	 {
		 m_maps = maps;
		 m_db = db;
		 m_future = f;
		 m_logger = logger;
	 }
	 // Loads a db
	 virtual void perform()
	 {
	    bool data = m_db->load(*m_maps, m_logger);
		m_future->setResult(data);
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
		FontTemplateDatabase * db = new FontTemplateDatabase(&m_counter);
		
		
		DBLoadingTaskFuture * future = new DBLoadingTaskFuture();
		DBLoadingTask * task = new DBLoadingTask(&maps,db,future,this->logger());
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
		this->panel()->synchronizeDatabase();
		this->Editor::onFullAppStart();
		
		
		class IFaceMouseMoveHandler : public EditorEventHandler
		{
		 public:
			IFaceMouseMoveHandler(Editor * ed) : EditorEventHandler(ed, &EditorBehaviour::onMouseMove)
			{
			}
			virtual void operator()(const sad::Event & ev)
			{
				if (m_editor) 
				{
					MainPanel * p = static_cast<IFaceEditor*>(m_editor)->panel();
					p->setMouseMovePosView(ev.x,ev.y);
				}
				this->EditorEventHandler::operator()(ev);
			}
		} * handler = new IFaceMouseMoveHandler(this);
		class IFaceKeyDownHandler : public EditorEventHandler
		{
		 public:
			IFaceKeyDownHandler(Editor * ed) : EditorEventHandler(ed, &EditorBehaviour::onKeyDown)
			{
			}
			virtual void operator()(const sad::Event & ev)
			{
				bool handled = false;
				if (m_editor) 
				{
					if (m_editor->behaviourSharedData()->activeObject() == NULL)
					{
						if (ev.key == KEY_ESC)
						{
							handled = true;
							static_cast<IFaceEditor*>(m_editor)->quit(ev);
						}
					}
				}
				if (!handled)
					this->EditorEventHandler::operator()(ev);
			}
		} * kbdhandler = new IFaceKeyDownHandler(this);


		sad::Input::inst()->setMouseMoveHandler(handler);
		sad::Input::inst()->setKeyDownHandler(kbdhandler);
		m_selection_border = new SelectedObjectBorder(this->behaviourSharedData());
		sad::Input::inst()->addPostRenderTask( new ActiveObjectBorder(this->behaviourSharedData()) );
		sad::Input::inst()->addPostRenderTask( m_selection_border );

		this->setBehaviour("main");
		this->highlightState("Idle");

	}
}

FontTemplateDatabase * IFaceEditor::database()
{
	return m_db;
}

void IFaceEditor::highlightState(const hst::string & hint)
{
	this->panel()->highlightState(hint);
}

