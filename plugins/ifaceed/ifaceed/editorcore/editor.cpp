#include <QTimer>
#include "editor.h"
#include <orthographiccamera.h>
#include "../objects/abstractscreenobject.h"
#include "editorbehaviour.h"
#include "editorbehaviourshareddata.h"
#include "../core/xmlconfigloader.h"
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <texturemanager.h>
#include <unused.h>
#include <input/controls.h>

Editor::Editor():m_icons("editor_icons")
{
	m_target = new QtTarget(this);
	sad::Renderer::ref()->log()->addTarget(m_target);
	m_cmdargs = NULL;
	m_rendermutex = new sad::Mutex();
	m_initmutex = new sad::Mutex();
	m_saddywaitmutex = new sad::Mutex();
	m_renderthread = new SaddyThread(this);
	m_waitforqt = false;
	m_waitforsaddy = false;
	m_qtapp = NULL;
	m_history = new EditorHistory();
	
}

sad::cli::Parser * Editor::parsedArgs() const
{
	return m_cmdoptions;
}

EditorBehaviourSharedData * Editor::createBehaviourData()
{
	return new EditorBehaviourSharedData();
}

void Editor::init(int argc,char ** argv)
{
	// Create dependent behaviour data
	m_behavioursharedata = this->createBehaviourData();
	// Firstly we create an arguments and application
	// to strip all of Qt's options, which wouldn't break a parser, after this work
	m_cmdargs = new sad::cli::Args(argc, argv);
	this->m_qtapp = new QApplication(this->m_cmdargs->count(),
									(this->m_cmdargs->arguments()));

	m_cmdoptions = this->createOptionParser();
	m_cmdoptions->parse(argc, (const char **)argv);
	
	this->assertSaddyInit(true);
	this->m_waitforsaddy = true;
	
	
	// This thread also runs ALL of event loops
	m_waitforsaddy = true;
	m_renderthread->start();
	this->waitForSaddyThread();
	if (this->saddyInitSuccessfull()) {
		this->initQtActions();
		m_waitforsaddy = true;
		this->awakeSaddyThread();
		this->waitForSaddyThread();
		this->runQtEventLoop();
	}
	m_renderthread->wait();

}
void Editor::waitForSaddyThread()
{
	while(this->shouldMainThreadWaitForSaddy());
}

void Editor::quit() 
{

}

Editor::~Editor() 
{
	for (sad::Hash<sad::String, EditorBehaviour*>::iterator it =m_behaviours.begin();it!=m_behaviours.end();it++)
	{
		delete it.value();
	}
	delete m_cmdoptions;
	delete m_saddywaitmutex;
	delete m_qtapp;
	delete m_initmutex;
	delete m_renderthread;
	delete m_rendermutex;
	delete m_cmdargs;
	delete m_history;
	delete m_behavioursharedata;
}

void SaddyThread::run() 
{
	this->m_editor->initSaddyActions();
	this->m_editor->m_waitforqt = true;
	this->m_editor->awakeMainThread();
	if (this->m_editor->saddyInitSuccessfull()) {
		this->waitForQtThread();
		this->m_editor->awakeMainThread();
		this->m_editor->runSaddyEventLoop();
	}
}

void SaddyThread::waitForQtThread() 
{
	while(this->m_editor->shouldSaddyThreadWaitForQt()) {
		this->msleep(100);
	}
}

void Editor::initSaddyActions() 
{
	this->initSaddyRendererOptions();
}

void Editor::initQtActions() 
{
	this->m_mainwindow = this->createQtWindow();
	this->bindQtSlots();
	this->awakeSaddyThread();
}

QMainWindow * Editor::createQtWindow()
{
	return NULL;
}

void Editor::bindQtSlots()
{
}

void Editor::runQtEventLoop()
{
	if (this->m_mainwindow) 
	{
		if (this->m_qtapp) 
		{
			QObject::connect(this->m_qtapp,SIGNAL(lastWindowClosed()),this,SLOT(qtQuitSlot()));
		}
		this->m_mainwindow->show();
	}

	if (this->m_qtapp) 
	{
		QObject::connect(this, SIGNAL(closureArrived(sad::ClosureBasic*)), this, SLOT(onClosureArrived(sad::ClosureBasic*)) );
		m_target->enable();
		QTimer::singleShot(0,this,SLOT(onFullAppStart()));
		this->m_qtapp->exec();
		m_target->disable();
	}
}

void Editor::runSaddyEventLoop() 
{
	m_quit_reason = EditorQuitReasonNotSet;
	sad::Renderer::ref()->controls()->add(*sad::input::ET_Quit, this, &Editor::onSaddyWindowDestroySlot);
	sad::Renderer::ref()->run();
	// Quit reason can be set by main thread, when window is closed
	if (m_quit_reason == EditorQuitReasonNotSet)
		this->saddyQuitSlot();
}

void Editor::onSaddyWindowDestroySlot()
{
	this->onSaddyWindowDestroy();
}

void Editor::initDefaultSaddyOptions()
{
	sad::Settings sett(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	sad::Renderer::ref()->init(sett);
	this->m_scene = new InterlockedScene(this);
	sad::Renderer::ref()->setScene(this->m_scene);
	sad::Renderer::ref()->makeFixedSize();
	// Try to load default icons
	QString a = QApplication::applicationDirPath();
	a = QDir(a).filePath(ICONS_XML);
	XMLConfigLoader * loader = new XMLConfigLoader(a);
	m_icons.setLoader(loader);
	bool loaded =  (m_icons.reload() == sad::SCR_OK);
	SL_DEBUG(QString("Loading icons from %1").arg(a));
	if (!loaded) 
	{
		SL_FATAL(QString("Can\'t load %1").arg(a));
	}
	this->assertSaddyInit(loaded);
}

sad::Sprite2DConfig & Editor::icons()
{
	return m_icons;
}

void Editor::initSaddyRendererOptions()
{
	this->initDefaultSaddyOptions();
}
void InterlockedScene::render()
{
	this->m_editor->lockRendering();
	this->sad::Scene::render();
	this->m_editor->unlockRendering();
}

void InterlockedScene::add(AbstractScreenObject * obj)
{
	obj->setScene(this);
	obj->addRef();
	this->Scene::add(obj);
}
void InterlockedScene::onNodeRemoval(sad::BasicNode * node)
{
	if (!node)
		return;
	if (node->metaData()->canBeCastedTo("AbstractScreenObject"))
	{
		static_cast<AbstractScreenObject*>(node)->delRef();
	}
	else
	{
		this->sad::Scene::onNodeRemoval(node);
	}
}
void InterlockedScene::remove(AbstractScreenObject * rem)
{
	rem->setScene(NULL);
	this->Scene::remove(rem);
}

InterlockedScene::~InterlockedScene()
{
}

void Editor::saddyQuitSlot()
{
	if (m_quit_reason == EditorQuitReasonNotSet) {
		m_quit_reason = QuitBySaddy;
		QTimer::singleShot(0,this,SLOT(onQuitActions()));
	}
}
void Editor::qtQuitSlot()
{
	if (m_quit_reason == EditorQuitReasonNotSet) {
		m_quit_reason = QuitByQtWindow;
		this->onQuitActions();
	}
}
void Editor::onQuitActions()
{
	this->onQtWindowDestroy();
	if (m_quit_reason == QuitBySaddy) {
		this->m_mainwindow->close();
	}
	if (m_quit_reason == QuitByQtWindow) {
		sad::Renderer::ref()->quit();
	}
	this->quitSaddyActions();
	this->quitQtActions();
}

void Editor::quitQtActions()
{

}
void Editor::quitSaddyActions()
{

}

void Editor::onQtWindowDestroy()
{

}
void Editor::onSaddyWindowDestroy()
{

}

void Editor::onFullAppStart()
{
	sad::input::Controls * c = sad::Renderer::ref()->controls();
	c->add(*sad::input::ET_KeyPress, this, &Editor::currentBehaviour, &EditorBehaviour::onKeyDown);
	c->add(*sad::input::ET_KeyRelease, this, &Editor::currentBehaviour, &EditorBehaviour::onKeyUp);
	c->add(*sad::input::ET_MouseWheel, this, &Editor::currentBehaviour, &EditorBehaviour::onWheel);
	c->add(*sad::input::ET_MousePress, this, &Editor::currentBehaviour, &EditorBehaviour::onMouseDown);
	c->add(*sad::input::ET_MouseRelease, this, &Editor::currentBehaviour, &EditorBehaviour::onMouseUp);
	c->add(*sad::input::ET_MouseMove, this, &Editor::currentBehaviour, &EditorBehaviour::onMouseMove);
}


void Editor::eraseBehaviour()
{
	if (m_current_behaviour.length())
	{
		m_behaviours[m_current_behaviour]->deactivate();
	}
	m_current_behaviour.clear();
}

void Editor::setBehaviour(const sad::String & name)
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

EditorBehaviour nullBehaviour(NULL,"");

EditorBehaviour * Editor::currentBehaviour()
{
	if (m_current_behaviour.length())
	{
		return m_behaviours[m_current_behaviour];
	}
	return &nullBehaviour;
}



void Editor::highlightState(UNUSED const sad::String & hint)
{

}

void Editor::onClosureArrived(sad::ClosureBasic * closure)
{
	closure->run();
	delete closure;
}


void Editor::emitClosure(sad::ClosureBasic * closure)
{
	emit closureArrived(closure);
}




