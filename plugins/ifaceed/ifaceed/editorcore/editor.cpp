#include <QTimer>
#include "editor.h"
#include <orthocamera.h>
#include <log.h>
#include "../objects/abstractscreenobject.h"
#include "editorbehaviour.h"
#include "editorbehaviourshareddata.h"
#include "editorlog.h"
#include <QMessageBox>


Editor::Editor()
{
	m_log = new EditorLog(this);
	m_cmdargs = NULL;
	m_rendermutex = new os::mutex();
	m_initmutex = new os::mutex();
	m_saddywaitmutex = new os::mutex();
	m_renderthread = new SaddyThread(this);
	m_waitforqt = false;
	m_waitforsaddy = false;
	m_qtapp = NULL;
	m_history = new EditorHistory(this->log());
	m_behavioursharedata = new EditorBehaviourSharedData();
}

void Editor::init(int argc,char ** argv)
{
	m_cmdoptions = this->createOptionParser();
	m_cmdoptions->parse(argc, argv);

	this->assertSaddyInit(true);
	this->m_waitforsaddy = true;
	m_cmdargs = new CommandArguments(argc, argv);
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
	hst::log::save("log.txt");
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
	for (hst::hash<hst::string, EditorBehaviour*>::iterator it =m_behaviours.begin();it!=m_behaviours.end();it++)
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
	m_log->save();
	delete m_log;
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
	this->m_qtapp = new QApplication(this->m_cmdargs->mutableCount(),
									(this->m_cmdargs->fullArgv()));
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
		QObject::connect(this, SIGNAL(closureArrived(ClosureBasic*)), this, SLOT(onClosureArrived(ClosureBasic*)) );
		QTimer::singleShot(0,this,SLOT(onFullAppStart()));
		this->m_qtapp->exec();
	}
}

void Editor::runSaddyEventLoop() 
{
	m_quit_reason = EditorQuitReasonNotSet;
	sad::Input::inst()->setQuitHandler( 
										new HandlerFor<Editor>::Method<
											void (Editor::*)(const sad::Event&),
											sad::Event
										>(this,&Editor::onSaddyWindowDestroySlot));
	sad::Renderer::instance().run();
	// Quit reason can be set by main thread, when window is closed
	if (m_quit_reason == EditorQuitReasonNotSet)
		this->saddyQuitSlot();
}

void Editor::onSaddyWindowDestroySlot(const sad::Event & ev)
{
	this->onSaddyWindowDestroy();
}

void Editor::initDefaultSaddyOptions()
{
	sad::Renderer::instance().init(sad::Settings(800,600,false));
	this->m_scene = new InterlockedScene(this);
	this->m_scene->setCamera(new OrthoCamera(false));
	sad::Renderer::instance().setCurrentScene(this->m_scene);
	sad::Renderer::instance().toggleFixedOn();
	this->assertSaddyInit(true);
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
	this->markForAddition(obj);
}
void InterlockedScene::onNodeRemoval(sad::BasicNode * node)
{
	if (!node)
		return;
	if (sad::isKindOf<AbstractScreenObject>(node))
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
	this->markForDeletion(rem);
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
		sad::Renderer::instance().quit();
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
	sad::Input::inst()->setKeyDownHandler(new EditorEventHandler(this, &EditorBehaviour::onKeyDown));
	sad::Input::inst()->setKeyUpHandler(new EditorEventHandler(this, &EditorBehaviour::onKeyUp));
	sad::Input::inst()->setMouseWheelHandler(new EditorEventHandler(this, &EditorBehaviour::onWheel));
	sad::Input::inst()->setMouseDownHandler(new EditorEventHandler(this, &EditorBehaviour::onMouseDown));
	sad::Input::inst()->setMouseUpHandler(new EditorEventHandler(this, &EditorBehaviour::onMouseUp));
	sad::Input::inst()->setMouseMoveHandler(new EditorEventHandler(this, &EditorBehaviour::onMouseMove));
}


void Editor::eraseBehaviour()
{
	if (m_current_behaviour.length())
	{
		m_behaviours[m_current_behaviour]->deactivate();
	}
	m_current_behaviour.clear();
}

void Editor::setBehaviour(const hst::string & name)
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
		hst::log::inst()->owrite("Can\'t find editor behaviour, named ").owrite(name).owrite(" \n");
	}
}

EditorBehaviour * Editor::currentBehaviour()
{
	if (m_current_behaviour.length())
	{
		return m_behaviours[m_current_behaviour];
	}
	return NULL;
}

void Editor::postBehaviourCallback( void (EditorBehaviour::*cb)(const sad::Event & ev), const sad::Event & ev)
{
	EditorBehaviour  * b = currentBehaviour();
	if (b)
	{		
		(b->*cb)(ev);
	}
}


void Editor::highlightState(const hst::string & hint)
{

}

void Editor::onClosureArrived(ClosureBasic * closure)
{
	closure->run();
	delete closure;
}


void Editor::emitClosure(ClosureBasic * closure)
{
	emit closureArrived(closure);
}