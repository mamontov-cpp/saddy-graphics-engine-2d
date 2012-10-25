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
#include <QTimer>

IFaceEditor::IFaceEditor()
{
	m_db = NULL;
	m_counter = 0;
	m_result = new ScreenTemplate();
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


// Class for logging some db data
class EditorCriticalLogger: public DBCriticalLogger
{
 private:
	 Editor * m_ed; //!< Editor to work with
 public:
	inline EditorCriticalLogger(Editor * ed)
	{ m_ed = ed; }
	virtual void critical( const QString & str)
	{ 
	    CLOSURE
		CLOSURE_DATA( QString str; )
		CLOSURE_CODE( QMessageBox::critical(NULL, "IFace Editor", str); )
		INITCLOSURE( CLSET(str, str); )
		SUBMITCLOSURE( m_ed->emitClosure );		 
	}
	~EditorCriticalLogger()
	{ }
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
		EditorCriticalLogger * logger= new EditorCriticalLogger(this);
		DBLoadingTaskFuture * future = new DBLoadingTaskFuture();
		DBLoadingTask * task = new DBLoadingTask(&maps,db,future,logger);
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
		delete logger;
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

		sad::Input::inst()->setMouseMoveHandler(handler);
		this->eraseBehaviour();
		this->highlightState("Idle");

		// TODO: remove it
		this->lockRendering();

		ScreenLabel * label = new ScreenLabel();
		m_result->add(label);
		ActionContext c;
		label->getProperty("font")->set(sad::Variant(hst::string("Times New Roman")),&c);
		label->getProperty("size")->set(sad::Variant(16u),&c);
		label->getProperty("color")->set(sad::Variant(hst::color(0,255,0)),&c);
		label->getProperty("text")->set(sad::Variant(hst::string("Times New RomanN\nTimes New Roman  N")),&c);
		label->getProperty("angle")->set(sad::Variant(0.0f),&c);
		label->getProperty("pos")->set(sad::Variant(hPointF(300,400)),&c);
		label->tryReload(this->database());

		static_cast<InterlockedScene*>(this->scene())->add(label);


		label = new ScreenLabel();
		m_result->add(label);
		label->getProperty("font")->set(sad::Variant(hst::string("Times New Roman")),&c);
		label->getProperty("size")->set(sad::Variant(8u),&c);
		label->getProperty("color")->set(sad::Variant(hst::color(0,255,0)),&c);
		label->getProperty("text")->set(sad::Variant(hst::string("Times New RomanN\nTimes New Roman  N")),&c);
		label->getProperty("angle")->set(sad::Variant(1.57f),&c);
		label->getProperty("pos")->set(sad::Variant(hPointF(400,500)),&c);
		label->tryReload(this->database());


		static_cast<InterlockedScene*>(this->scene())->add(label);

		this->unlockRendering();
		// TODO: remove it end
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

