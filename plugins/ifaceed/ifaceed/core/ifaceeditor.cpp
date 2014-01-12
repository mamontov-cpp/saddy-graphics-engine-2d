#include <QFontComboBox>
#include <QFontDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include "ifaceeditor.h"
#include <freetype/font.h>
#include <label.h>
#include <fontmanager.h>
#include <pipeline/pipelinetask.h>
#include <pipeline/pipeline.h>
#include "core/fonttemplatesdatabase.h"
#include "../editorcore/editorbehaviour.h"
#include "../objects/screentemplate.h"
#include "../objects/screenlabel.h"
#include "../objects/screensprite.h"
#include "states/idlestate.h"
#include "states/labeladdingstate.h"
#include "states/spriteaddingstate.h"
#include "states/selectedstate.h"
#include "objectborders.h"
#include "../history/propertychangecommand.h"
#include "../history/deletecommand.h"
#include <QTimer>
#include "objectxmlreader.h"
#include "sceneaddingtask.h"



IFaceEditor::IFaceEditor()
{
	m_handling_event = false;
	sad::log::FileTarget * fh = new sad::log::FileTarget("{0}: [{1}] {3}{2}{4}", sad::log::MESSAGE);
	fh->open("user.txt");
	this->log()->addTarget(fh);
	m_target = new sad::log::FileTarget();
	m_target->open("full.txt");
	this->log()->addTarget(m_target);

	m_db = NULL;
	m_counter = 0;
	m_result = new ScreenTemplate();
	m_selection_border = NULL;

	EditorBehaviour * behaviour = new EditorBehaviour(this,"idle");
	behaviour->addState("idle", new IdleState());
	behaviour->addState("label_adding", new LabelAddingState());
	behaviour->addState("selected", new SelectedState());
	behaviour->addState("sprite_adding_simple", new SimpleSpriteAddingState());
	behaviour->addState("sprite_adding_diagonal", new DiagonalSpriteAddingState());
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
	sad::Renderer::ref()->setWindowTitle("Saddy Interface Editor");
	sad::Renderer::ref()->makeFixedSize();
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

void IFaceEditor::quit()
{
	sad::Renderer::ref()->quit();
}

sad::cli::Parser * IFaceEditor::createOptionParser()
{
	sad::cli::Parser * r = new sad::cli::Parser();
	r->addSingleValuedOption("ifaceconfig");
	r->addFlag("debug");
	return r;
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




void IFaceEditor::onFullAppStart()
{
	if (this->parsedArgs()->single("ifaceconfig").value().length() == 0)
	{
		SL_WARNING("Config file is not specified. You must choose it now");
		QString config = QFileDialog::getOpenFileName(this->panel(),"Choose a config file",QString(),
													  ("Configs (*.xml)"));
		if (config.length() == 0) 
		{
			SL_FATAL("Config file is not specified. Quitting...");
			QTimer::singleShot(0, this->panel(), SLOT(close()));
			return;
		} 
		else 
		{
			this->parsedArgs()->setSingleValuedOption("ifaceconfig", config.toStdString().c_str());
		}
	}
	bool success = true;
	// Load first stage - a maps of handling all of data
	FontTemplatesMaps maps;
	if (maps.load(this->parsedArgs()->single("ifaceconfig").value().data(), sad::log::Log::ref()))
	{
		FontTemplateDatabase * db = new FontTemplateDatabase(&m_counter);
		
		
		DBLoadingTaskFuture * future = new DBLoadingTaskFuture();
		DBLoadingTask * task = new DBLoadingTask(&maps,db,future,this->log());
		// Locking rendering due to adding of new task
		this->lockRendering();
		sad::Renderer::ref()->pipeline()->append(task);
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
		SL_FATAL("Can\'t load config file");
		QTimer::singleShot(0, this->panel(), SLOT(close()));
	}
	if (success) {
		this->panel()->setEditor(this);
		this->panel()->synchronizeDatabase();
		this->Editor::onFullAppStart();
		
		
		class IFaceMouseMoveHandler 
		: public sad::input::AbstractHandlerForType<sad::input::MouseMoveEvent>
		{
		 public:
			IFaceMouseMoveHandler(IFaceEditor * ed) : m_editor(ed)
			{
			}
			virtual void invoke(const sad::input::MouseMoveEvent & ev)
			{
				if (m_editor) 
				{
					MainPanel * p = m_editor->panel();
					p->setMouseMovePosView(ev.pos2D().x(),ev.pos2D().y());
				}
				m_editor->currentBehaviour()->onMouseMove(ev);
			}
		 protected:
		   IFaceEditor * m_editor;
		} * handler = new IFaceMouseMoveHandler(this);
		class IFaceKeyDownHandler
		: public sad::input::AbstractHandlerForType<sad::input::KeyPressEvent>
		{
		 public:
			IFaceKeyDownHandler(IFaceEditor * ed) : m_editor(ed)
			{
			}
			void commitInEditor()
			{
				CLOSURE
				CLOSURE_DATA(IFaceEditor * e;)
				CLOSURE_CODE(e->history()->commit(e); )
				INITCLOSURE( CLSET(e, m_editor); );
				SUBMITCLOSURE( m_editor->emitClosure );
			}
			void rollbackInEditor()
			{
				IFaceEditor * ed = static_cast<IFaceEditor*>(m_editor);
				CLOSURE
				CLOSURE_DATA(IFaceEditor * e;)
				CLOSURE_CODE(e->history()->rollback(e); )
				INITCLOSURE( CLSET(e, m_editor); );
				SUBMITCLOSURE( m_editor->emitClosure );
			}
			virtual void invoke(const sad::input::KeyPressEvent & ev)
			{
				bool handled = false;
				if (m_editor) 
				{
					if (m_editor->behaviourSharedData()->activeObject() == NULL
						&& m_editor->behaviourSharedData()->selectedObject() == NULL)
					{
						if (ev.Key == sad::Esc)
						{
							handled = true;
							m_editor->quit();
						}
					}
					if (ev.Key == sad::Z && ev.CtrlHeld)
					{
						handled = true;
						this->rollbackInEditor();
					}
					if (ev.Key == sad::R && ev.CtrlHeld)
					{
						handled = true;
						this->commitInEditor();
					}
				}
				if (!handled)
					m_editor->currentBehaviour()->onKeyDown(ev);
			}
		protected:
		   IFaceEditor * m_editor;
		} * kbdhandler = new IFaceKeyDownHandler(this);


		sad::Renderer::ref()->controls()->add(*sad::input::ET_MouseMove, handler);
		sad::Renderer::ref()->controls()->add(*sad::input::ET_KeyPress, kbdhandler);
		m_selection_border = new SelectedObjectBorder(this->shdata());
		sad::Renderer::ref()->pipeline()->append( new ActiveObjectBorder(this->shdata()) );
		sad::Renderer::ref()->pipeline()->append( m_selection_border );

		this->setBehaviour("main");
		this->highlightState("Idle");

	}
}

FontTemplateDatabase * IFaceEditor::database()
{
	return m_db;
}

void IFaceEditor::highlightState(const sad::String & hint)
{
	this->panel()->highlightState(hint);
}

InterlockedScene * IFaceEditor::myScene()
{
	return static_cast<InterlockedScene*>(this->scene());
}
void IFaceEditor::tryEraseObject()
{
	sad::String state = this->currentBehaviour()->state(); 
	if (state == "label_adding" 
		|| state == "sprite_adding_simple" 
		|| state == "sprite_adding_diagonal")
	{
		AbstractScreenObject * o =	this->behaviourSharedData()->activeObject();
		this->behaviourSharedData()->setActiveObject(NULL);
		InterlockedScene * scene = this->myScene();
		scene->remove(o);
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

void IFaceEditor::submitEvent(UNUSED const sad::String & eventType,UNUSED const sad::Variant & v)
{
	CLOSURE
	CLOSURE_DATA( IFaceEditor * me; )
	CLOSURE_CODE( 
		SL_SCOPE("IFaceEditor::submitEvent()::closure");
		if (me->m_handling_event)
			return;
		me->m_handling_event = true;
		me->panel()->updateList(); 
		if (me->behaviourSharedData()->selectedObject() != NULL )
		{
			if (me->behaviourSharedData()->activeObject() == NULL)
			{
				SL_SCOPE("IFaceEditor::submitEvent()::closure::callUpdateObjectStats()");
				me->panel()->updateObjectStats(me->behaviourSharedData()->selectedObject());
			}
			// Remove order, if selected removed
			if (me->shdata()->selectedObject()->prop<bool>("activity", me->log()) == false
			   )
			{
				SL_SCOPE("IFaceEditor::submitEvent()::closure::fixingSelected()");
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


EditorBehaviourSharedData * IFaceEditor::createBehaviourData()
{
	IFaceSharedData * e = new IFaceSharedData();
	e->setEditor(this);
	return e;
}

IFaceSharedData * IFaceEditor::shdata()
{
	return static_cast<IFaceSharedData *>(this->behaviourSharedData());
}

void IFaceEditor::appendRotationCommand()
{
	float new_angle = 0.0f;
	float old_angle = 0.0f;
	AbstractScreenObject * o = NULL;
	this->shdata()->getAndDismissRotationCommand(o, new_angle, old_angle);
	this->history()->add(new PropertyChangeCommand<float>(o, "angle", new_angle, old_angle, this->log()));
}


void IFaceEditor::reload()
{
   SL_SCOPE("IFaceEditor::reload()");
   // 1. Load maps
   FontTemplatesMaps * maps =  new FontTemplatesMaps(); 
   sad::String filename = this->parsedArgs()->single("ifaceconfig").value();
   if (maps->load(filename.data(),this->log()) 
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
   DBLoadingTask * task = new DBLoadingTask(maps,db,future,this->log());
   this->lockRendering();
   sad::Renderer::ref()->pipeline()->prepend(task);
   this->unlockRendering();
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


void IFaceEditor::save()
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

void IFaceEditor::load()
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
		while(it)
		{
			allobjectsarevalid = allobjectsarevalid && it->isValid(this->database());
			// We cannot perform ::setUid here, because it will broke some iterator
			sad::String uid = it->prop<sad::String>("uid", this->log());
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
		this->lockRendering();
		if (this->scene()->objectCount() != 0)
			this->scene()->clear();
		// Add post-render task, which adds a sorted results when scene is empty and dies
		sad::Renderer::ref()->pipeline()->append(new SceneAddingTask(e, this->myScene()));
		this->unlockRendering();
	}
}
