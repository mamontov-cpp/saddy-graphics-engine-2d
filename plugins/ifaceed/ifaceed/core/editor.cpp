#include <QFontComboBox>
#include <QFontDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>

#include "editor.h"

#include <freetype/font.h>

#include <label.h>
#include <fontmanager.h>
#include <renderer.h>

#include <input/controls.h>
#include <keymouseconditions.h>

#include <pipeline/pipelinetask.h>
#include <pipeline/pipeline.h>

#include <db/load.h>
#include <db/save.h>
#include <db/dbdatabase.h>

#include <resource/tree.h>

#include "core/fonttemplatesdatabase.h"
#include "core/editorbehaviour.h"
#include "core/saddythread.h"
#include "core/xmlconfigloader.h"

#include "gui/eventfilter.h"

#include "../objects/screentemplate.h"
#include "../objects/screenlabel.h"
#include "../objects/screensprite.h"

#include "states/idlestate.h"
#include "states/labeladdingstate.h"
#include "states/spriteaddingstate.h"
#include "states/selectedstate.h"

#include "../history/propertychangecommand.h"
#include "../history/deletecommand.h"

#include "objectborders.h"
#include "objectxmlreader.h"
#include "sceneaddingtask.h"

#include "typeconverters/save.h"
#include "typeconverters/load.h"
#include "typeconverters/qcolortosadcolor.h"
#include "typeconverters/qcolortosadacolor.h"
#include "typeconverters/qstringtosadstring.h"
#include "typeconverters/qlistqlistqcolortosadvectorsadvectoracolor.h"
#include "typeconverters/sadcolortoqcolor.h"
#include "typeconverters/sadacolortoqcolor.h"
#include "typeconverters/sadstringtoqstring.h"
#include "typeconverters/sadvectorsadvectoracolortoqlistqlistqcolor.h"

core::Editor::Editor() : m_icons("editor_icons")
{
	// Add message data
	m_qttarget = new core::QtTarget(this);
	sad::Renderer::ref()->log()->addTarget(m_qttarget);

	// Add small user log
	sad::log::FileTarget * t = new sad::log::FileTarget("{0}: [{1}] {3}{2}{4}", sad::log::MESSAGE);
	t->open("user.txt");
	sad::log::Log::ref()->addTarget(t);

	// Add large debug log
	t = new sad::log::FileTarget();
	t->open("full.txt");
	sad::log::Log::ref()->addTarget(t);

	m_cmdargs = NULL;
	m_initmutex = new sad::Mutex();
	m_saddywaitmutex = new sad::Mutex();
	m_renderthread = new core::SaddyThread(this);
	m_waitforqt = false;
	m_waitforsaddy = false;
	m_qtapp = NULL;
	m_history = new history::History();

	m_machine = new sad::hfsm::Machine();
	m_machine->addState("idle", new sad::hfsm::State(), true);
	m_machine->enterState("idle");
	sad::String idle = "idle";
	sad::Renderer::ref()->controls()->add(*sad::input::ET_KeyPress & sad::Esc & (m_machine * idle), sad::Renderer::ref(), &sad::Renderer::quit);
	sad::Renderer::ref()->controls()->add(*sad::input::ET_KeyPress & sad::Z & sad::HoldsControl, this, &core::Editor::undo);
	sad::Renderer::ref()->controls()->add(*sad::input::ET_KeyPress & sad::R & sad::HoldsControl, this, &core::Editor::redo);


	m_handling_event = false;
	m_db = NULL;
	m_counter = 0;
	m_result = new ScreenTemplate();
	m_selection_border = NULL;

	core::EditorBehaviour * behaviour = new core::EditorBehaviour(this,"idle");
	behaviour->addState("idle", new IdleState());
	behaviour->addState("label_adding", new LabelAddingState());
	behaviour->addState("selected", new SelectedState());
	behaviour->addState("sprite_adding_simple", new SimpleSpriteAddingState());
	behaviour->addState("sprite_adding_diagonal", new DiagonalSpriteAddingState());
	this->behaviours().insert("main", behaviour);

	// Fill conversion table with converters
	this->initConversionTable();
}
core::Editor::~Editor()
{
	delete m_db;
	delete m_result;
	for (sad::Hash<sad::String, core::EditorBehaviour*>::iterator it = m_behaviours.begin();
		it!=m_behaviours.end();
		++it)
	{
		delete it.value();
	}
	delete m_cmdoptions;
	delete m_saddywaitmutex;
	delete m_qtapp;
	delete m_initmutex;
	delete m_renderthread;
	delete m_cmdargs;
	delete m_history;
	delete m_behavioursharedata;
	delete m_machine;
}

void core::Editor::init(int argc,char ** argv)
{
	// Create data, shared between behaviours
	m_behavioursharedata = new core::Shared();
	m_behavioursharedata->setEditor(this);

	// Create and parse command line arguments
	m_cmdargs = new sad::cli::Args(argc, argv);
	m_cmdoptions = new sad::cli::Parser();
	m_cmdoptions->addSingleValuedOption("resources");
	m_cmdoptions->addSingleValuedOption("width");
	m_cmdoptions->addSingleValuedOption("height");
	m_cmdoptions->addFlag("debug");
	m_cmdoptions->parse(argc, const_cast<const char **>(argv));
	
	// This thread only runs qt event loop. SaddyThread runs only event loop of renderer of Saddy.
	m_waitforsaddy = true;
	m_renderthread->start();
	// Wait for Saddy's window to show up
	this->waitForSaddyThread();
	this->runQtEventLoop();
	m_renderthread->wait();
}

MainPanel* core::Editor::panel()
{
	return m_mainwindow;
}

sad::hfsm::Machine* core::Editor::machine()
{
	return m_machine;
}

void core::Editor::start()
{
	SL_SCOPE("core::Editor::start()");
	bool mustquit = false;

	// Try load icons resources
	sad::Renderer::ref()->addTree("icons", new sad::resource::Tree());
	sad::Renderer::ref()->tree("icons")->factory()->registerResource<sad::freetype::Font>();
	sad::Vector<sad::resource::Error * > errors;
	errors = sad::Renderer::ref()->tree("icons")->loadFromFile(ICONS_PATH);
	if (errors.size())
	{
		mustquit = true;
		this->reportResourceLoadingErrors(errors, ICONS_PATH);
	}

	// Try load specified resources, if need to
	sad::Renderer::ref()->tree("")->factory()->registerResource<sad::freetype::Font>();
	sad::Maybe<sad::String> maybefilename = this->parsedArgs()->single("resources");
	if (maybefilename.exists() && this->parsedArgs()->specified("resources"))
	{
		errors = sad::Renderer::ref()->tree("")->loadFromFile(maybefilename.value());
		if (errors.size())
		{
			mustquit = true;
			this->reportResourceLoadingErrors(errors, maybefilename.value());
		}
	}
	else
	{
		this->m_mainwindow->toggleEditingButtons(false);
	}
	
	bool database_loaded = false;
	if (this->m_mainwindow->isEditingEnabled() && this->parsedArgs()->defaultOption().exists())
	{
		// Try load database	
	}

	// If no database loaded, init default database, add a palette to it.
	if (database_loaded == false)
	{
		// Create a default database
		sad::db::Database * db = new sad::db::Database();
		// Default database has empty name
		sad::Renderer::ref()->addDatabase("", db);
	}
	this->m_mainwindow->viewDatabase();

	if (mustquit)
	{
		 QTimer::singleShot(0, this->panel(), SLOT(close()));
	}
}

void core::Editor::undo()
{
	m_history->rollback(this);
}

void core::Editor::redo()
{
	m_history->commit(this);
}


void core::Editor::reportResourceLoadingErrors(
		sad::Vector<sad::resource::Error *> & errors,
		const sad::String& name
)
{
	sad::String errorlist = sad::resource::format(errors);
	sad::String resultmessage = "There was errors while loading ";
	resultmessage += name;
	resultmessage += ":\n";
	resultmessage += errorlist;
	sad::util::free(errors);
	errors.clear();
	SL_FATAL(resultmessage);
}

void core::Editor::initConversionTable()
{
	sad::db::ConversionTable::ref()->add(
		"sad::Color", 
		"QColor", 
		new core::typeconverters::SadColorToQColor()
	);
	sad::db::ConversionTable::ref()->add(
		"sad::AColor", 
		"QColor", 
		new core::typeconverters::SadAColorToQColor()
	);
	sad::db::ConversionTable::ref()->add(
		"sad::String", 
		"QString", 
		new core::typeconverters::SadStringToQString()
	);
	sad::db::ConversionTable::ref()->add(
		"sad::Vector<sad::Vector<sad::AColor> >", 
		"QList<QList<QColor> >", 
		new core::typeconverters::SadVectorSadVectorToAColorToQListQListQColor()
	);
	sad::db::ConversionTable::ref()->add(
		"QColor", 
		"sad::Color", 		
		new core::typeconverters::QColorToSadColor()
	);
	sad::db::ConversionTable::ref()->add(
		"QColor", 		
		"sad::AColor", 
		new core::typeconverters::QColorToSadAColor()
	);
	sad::db::ConversionTable::ref()->add(
		"QString",
		"sad::String",  
		new core::typeconverters::QStringToSadString()
	);
	sad::db::ConversionTable::ref()->add(
		"QList<QList<QColor> >", 
		"sad::Vector<sad::Vector<sad::AColor> >", 
		new core::typeconverters::QListQListQColorToSadVectorSadVectorToAColor()
	);
}

void core::Editor::runQtEventLoop()
{
	m_qtapp = new QApplication(m_cmdargs->count(), m_cmdargs->arguments());

	m_mainwindow = new MainPanel();
	m_mainwindow->setEditor(this);


    gui::EventFilter* filter = new gui::EventFilter();
    filter->setPanel(m_mainwindow);
    QCoreApplication::instance()->installEventFilter(filter);

	QObject::connect(
		this->m_qtapp,
		SIGNAL(lastWindowClosed()),
		this,
		SLOT(qtQuitSlot()));
	this->m_mainwindow->show();

	QObject::connect(
		this, 
		SIGNAL(closureArrived(sad::ClosureBasic*)), 
		this, 
		SLOT(onClosureArrived(sad::ClosureBasic*)) 
	);
	m_qttarget->enable();
	QTimer::singleShot(0, this, SLOT(start()));
	this->m_qtapp->exec();
	m_qttarget->disable();
}

void core::Editor::runSaddyEventLoop() 
{
	m_quit_reason = core::QR_NOTSET;
	sad::Renderer::ref()->run();
	// Quit reason can be set by main thread, when window is closed
	if (m_quit_reason == core::QR_NOTSET)
		this->saddyQuitSlot();
}

void core::Editor::saddyQuitSlot()
{
	if (m_quit_reason == core::QR_NOTSET) {
		m_quit_reason = core::QR_SADDY;
		QTimer::singleShot(0,this,SLOT(onQuitActions()));
	}
}
void core::Editor::qtQuitSlot()
{
	if (m_quit_reason == core::QR_NOTSET) {
		m_quit_reason = core::QR_QTWINDOW;
		this->onQuitActions();
	}
}
void core::Editor::onQuitActions()
{
	if (m_quit_reason == core::QR_SADDY) {
		this->m_mainwindow->close();
	}
	if (m_quit_reason == core::QR_QTWINDOW) {
		sad::Renderer::ref()->quit();
	}
}

void core::Editor::setDatabase(FontTemplateDatabase * db)
{
	delete m_db;
	m_db = db;
}

void core::Editor::quit()
{
	sad::Renderer::ref()->quit();
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
class DBLoadingTask: public sad::pipeline::AbstractTask
{
 protected:
	 FontTemplatesMaps * m_maps; //!< Maps data
	 FontTemplateDatabase * m_db;  //!< Database for loading
	 DBLoadingTaskFuture * m_future; //!< Future for computing
	 sad::log::Log * m_log;    //!< Logger for logging data
 public:
	 /** Constructs new tasks
	  */
	 inline DBLoadingTask(FontTemplatesMaps * maps, 
						  FontTemplateDatabase * db, 
						  DBLoadingTaskFuture * f, 
						  sad::log::Log * log)
	 {
		 m_maps = maps;
		 m_db = db;
		 m_future = f;
		 m_log = log;
	 }
	 // Loads a db
	 virtual void _process()
	 {
	    bool data = m_db->load(*m_maps, m_log);
		m_future->setResult(data);
	 }
	 virtual ~DBLoadingTask()
	 {
	 }
};

FontTemplateDatabase * core::Editor::database()
{
	return m_db;
}

void core::Editor::highlightState(const sad::String & hint)
{
	this->panel()->highlightState(hint);
}


void core::Editor::tryRenderActiveObject()
{
	AbstractScreenObject * o =	this->behaviourSharedData()->activeObject();
	if (o)
		o->render();
}
void core::Editor::tryEraseObject()
{
	sad::String state = this->currentBehaviour()->state(); 
	if (state == "label_adding" 
		|| state == "sprite_adding_simple" 
		|| state == "sprite_adding_diagonal")
	{
		AbstractScreenObject * o =	this->behaviourSharedData()->activeObject();
		delete o;
		this->behaviourSharedData()->setActiveObject(NULL);
		this->currentBehaviour()->cancelState();
	}
	if (state == "selected")
	{
		AbstractScreenObject * o =	this->behaviourSharedData()->selectedObject();
		this->behaviourSharedData()->setSelectedObject(NULL);
		DeleteCommand * cmd = new DeleteCommand(this->result(), o);
		this->history()->add(cmd);
		cmd->commit(this);
		this->currentBehaviour()->enterState("idle");
	}
}

void core::Editor::submitEvent(UNUSED const sad::String & eventType,UNUSED const sad::db::Variant & v)
{
	CLOSURE
	CLOSURE_DATA( core::Editor * me; )
	CLOSURE_CODE( 
		SL_SCOPE("core::Editor::submitEvent()::closure");
		if (me->m_handling_event)
			return;
		me->m_handling_event = true;
		me->panel()->updateList(); 
		if (me->behaviourSharedData()->selectedObject() != NULL )
		{
			if (me->behaviourSharedData()->activeObject() == NULL)
			{
				SL_SCOPE("core::Editor::submitEvent()::closure::callUpdateObjectStats()");
				me->panel()->updateObjectStats(me->behaviourSharedData()->selectedObject());
			}
			// Remove order, if selected removed
			sad::log::Log* lg = sad::log::Log::ref();
			if (me->shdata()->selectedObject()->prop<bool>("activity",lg) == false
			   )
			{
				SL_SCOPE("core::Editor::submitEvent()::closure::fixingSelected()");
				if (me->currentBehaviour()->state() == "selected")
				{
					SL_DEBUG("Entering idle state");
					me->currentBehaviour()->enterState("idle");
				}
				SL_DEBUG("Unselecting object to null");
				me->shdata()->setSelectedObject(NULL);
			}
		}
		me->m_handling_event = false;
	)
	INITCLOSURE ( CLSET(me, this) )
	SUBMITCLOSURE( this->emitClosure );
}

core::Shared* core::Editor::shdata()
{
	return this->behaviourSharedData();
}

void core::Editor::appendRotationCommand()
{
	float new_angle = 0.0f;
	float old_angle = 0.0f;
	AbstractScreenObject * o = NULL;
	sad::log::Log* lg = sad::log::Log::ref();
	this->shdata()->getAndDismissRotationCommand(o, new_angle, old_angle);
	this->history()->add(new PropertyChangeCommand<float>(o, "angle", new_angle, old_angle, lg));
}


void core::Editor::reload()
{
   SL_SCOPE("core::Editor::reload()");
   // 1. Load maps
   FontTemplatesMaps * maps =  new FontTemplatesMaps(); 
   sad::String filename = this->parsedArgs()->single("ifaceconfig").value();
   sad::log::Log* lg = sad::log::Log::ref();
   if (maps->load(filename.data(), lg) 
	   == false) {
		// 2. If map loading failed, stop right there
	    // 2.1. Report   error
	    delete maps;
		SL_WARNING(str(fmt::Print("Map file \"{0}\": loading failed") << filename.data()));
		return;
   }
   // 3. Load texture database
   m_counter++;
   FontTemplateDatabase * db = new FontTemplateDatabase(&m_counter);		
   DBLoadingTaskFuture * future = new DBLoadingTaskFuture();
   DBLoadingTask * task = new DBLoadingTask(maps,db,future, lg);
   sad::Renderer::ref()->lockRendering();
   sad::Renderer::ref()->pipeline()->prepend(task);
   sad::Renderer::ref()->unlockRendering();
   if (future->result() == false) {
		// 3.1. If loading failed, report error	 
	    delete maps;
		delete future;
		delete db;
		sad::String filename = this->parsedArgs()->single("ifaceconfig").value();
		SL_WARNING(str(fmt::Print("Map file \"{0}\": loading font and templates failed") << filename));
		return;
   }
   // At this point we need only database, db to
   // check compliance with other objects
   delete maps;
   delete future;
   // 4. Check, whether all scene needed data in DB
   bool allobjectsvalid = true;
   // Container of errors
   sad::Vector<sad::String> errors;
   AbstractScreenObject * it = this->result()->templateBegin();	
   while (it)
   {
		allobjectsvalid = allobjectsvalid && it->isValid(db, &errors);
		it = this->result()->templateNext();
   }
   if (!allobjectsvalid)
   {
	   // 4.1. If failed, report error
	   sad::String errorsasstring = "Not all objects are valid:\n";
       for(unsigned int i = 0; i < errors.count(); i++)
	   {
			errorsasstring << errors[i];
	   }
	   SL_WARNING(errorsasstring);
	   delete db;
	   return;
   }
   // 5. Reload scene data for db
   it = this->result()->templateBegin();	
   while (it)
   {
	    it->tryReload(db);
		it = this->result()->templateNext();
   }
   // 6. Remove old DB
   this->setDatabase(db);
   /*
	  7. Reload fonts in UI 
	  8. Reload sprites in UI
    */
   this->panel()->synchronizeDatabase();
   /**	 
	 9. Toggle selected object, if in selected state. This for resetting object data
   */
   if (this->currentBehaviour()->state() == "selected")
   {
   	   this->panel()->updateObjectStats(this->behaviourSharedData()->selectedObject());
   }
}


void core::Editor::save()
{
	QString filename = QFileDialog::getSaveFileName(
		NULL, 
		"Please specify save file name",
		"",
		QString("XML (*.xml)")
		);
	if (filename.length())
	{
		ObjectXMLWriter w(filename, "screentemplate");
		if(w.write(this->result()) == false)
		{
			SL_WARNING(QString("Cannot write into file \"") + filename + "\"");
		}
	}
}

void core::Editor::load()
{
	QString filename = QFileDialog::getOpenFileName(
		NULL, 
		"Please specify save file name",
		"",
		QString("XML (*.xml)")
		);
	if (filename.length())
	{
		ScreenObjectXMLReader  r(filename);
		ScreenTemplate * e = new ScreenTemplate();
		if (r.read(e) == false)
		{
			delete e;
			SL_WARNING(QString("Cannot load  file \"") + filename + "\"");
			return;
		}
		// We must save our uids, to force it to work with container 
		// Also, we check when all objects are valid and can be loaded from db
		AbstractScreenObject * it = e->templateBegin();
		bool allobjectsarevalid = true;
		sad::Vector<sad::Pair<sad::String, AbstractScreenObject *> > m_pairs;
		sad::log::Log* lg = sad::log::Log::ref();
		while(it)
		{
			allobjectsarevalid = allobjectsarevalid && it->isValid(this->database());
			// We cannot perform ::setUid here, because it will broke some iterator
			sad::String uid = it->prop<sad::String>("uid", lg);
			m_pairs << sad::Pair<sad::String, AbstractScreenObject *>(uid, it);
			// This must be done, because we data added via
			// HashBasedSerializableContainer::add, which don't care
			// to some reference-counting, but real container cares about it
			// in destructor, so we need to increment counter
			// to make object survive death of his container or scene
			it->addRef();
			it = e->templateNext();
		}
        for(unsigned int i = 0; i < m_pairs.count(); i++)
		{
			e->setUid(m_pairs[i].p2(), m_pairs[i].p1());
		}
		if (!allobjectsarevalid)
		{
			delete e;
			SL_WARNING(QString("Not all objects can be mapped to database, aborting"));
			return;
		}
		// Reload an objects
		it = e->templateBegin();
		while(it)
		{
			it->initializeGraphicAfterLoad(this->database());
			it = e->templateNext();
		}
		
		// Clear history
		this->history()->clear();
		// Clear result
		delete m_result;
		m_result = e;
		// Perform cleanup data
		this->currentBehaviour()->enterState("idle");
		sad::Renderer::ref()->lockRendering();
		if (this->scene()->objectCount() != 0)
			this->scene()->clear();
		// Add post-render task, which adds a sorted results when scene is empty and dies
		sad::Renderer::ref()->pipeline()->append(new SceneAddingTask(e, this->scene()));
		sad::Renderer::ref()->unlockRendering();
	}
}

sad::cli::Parser * core::Editor::parsedArgs() const
{
	return m_cmdoptions;
}

void core::Editor::waitForSaddyThread()
{
	while(this->shouldMainThreadWaitForSaddy());
}

sad::Sprite2DConfig & core::Editor::icons()
{
	return m_icons;
}

void core::Editor::eraseBehaviour()
{
	if (m_current_behaviour.length())
	{
		m_behaviours[m_current_behaviour]->deactivate();
	}
	m_current_behaviour.clear();
}

void core::Editor::setBehaviour(const sad::String & name)
{
	if (m_current_behaviour.length())
	{
		m_behaviours[m_current_behaviour]->deactivate();
	}
	if (behaviours().contains(name))
	{
		m_current_behaviour = name;
		m_behaviours[m_current_behaviour]->activate();
	}
	else 
	{
		SL_DEBUG(QString("Can\'t find editor behaviour, named %1").arg(name.data()));
	}
}

core::EditorBehaviour nullBehaviour(NULL,"");

core::EditorBehaviour * core::Editor::currentBehaviour()
{
	if (m_current_behaviour.length())
	{
		return m_behaviours[m_current_behaviour];
	}
	return &nullBehaviour;
}

void core::Editor::onClosureArrived(sad::ClosureBasic * closure)
{
	closure->run();
	delete closure;
}


void core::Editor::emitClosure(sad::ClosureBasic * closure)
{
	emit closureArrived(closure);
}
