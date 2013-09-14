#include <QTimer>
#include "editor.h"
#include <orthocamera.h>
#include "../objects/abstractscreenobject.h"
#include "editorbehaviour.h"
#include "editorbehaviourshareddata.h"
#include "../core/xmlconfigloader.h"
// Avoid X11 namespace pollution
#ifdef Status
    #undef Status
#endif
#ifdef Bool
    #undef Bool
#endif
#ifdef None
    #undef None
#endif
#ifdef CursorShape
    #undef CursorShape
#endif
#ifdef GrayScale
    #undef GrayScale
#endif
#ifdef Color
    #undef Color
#endif
#ifdef KeyPress
    #undef KeyPress
#endif
#ifdef KeyRelease
    #undef KeyRelease
#endif
#ifdef FocusIn
    #undef FocusIn
#endif
#ifdef FocusOut
    #undef FocusOut
#endif
#ifdef FontChange
    #undef FontChange
#endif
#ifdef Unsorted
    #undef Unsorted
#endif
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <png/picopngloader.h>
#include <texturemanager.h>

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

Editor::Editor():m_icons("editor_icons")
{
	m_target = new QtTarget(this);
	sad::Renderer::ref()->log()->addTarget(m_target);
	m_cmdargs = NULL;
	m_rendermutex = new os::mutex();
	m_initmutex = new os::mutex();
	m_saddywaitmutex = new os::mutex();
	m_renderthread = new SaddyThread(this);
	m_waitforqt = false;
	m_waitforsaddy = false;
	m_qtapp = NULL;
	m_history = new EditorHistory(this->log());
	
}

sad::cmd::Parser * Editor::parsedArgs() const
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
	m_cmdargs = new sad::cmd::Args(argc, argv);
	this->m_qtapp = new QApplication(this->m_cmdargs->mutableCount(),
									(this->m_cmdargs->fullArgv()));

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
	sad::Input::ref()->setQuitHandler( 
										new HandlerFor<Editor>::Method<
											void (Editor::*)(const sad::Event&),
											sad::Event
										>(this,&Editor::onSaddyWindowDestroySlot));
	sad::Renderer::ref()->run();
	// Quit reason can be set by main thread, when window is closed
	if (m_quit_reason == EditorQuitReasonNotSet)
		this->saddyQuitSlot();
}

void Editor::onSaddyWindowDestroySlot(UNUSED const sad::Event & ev)
{
	this->onSaddyWindowDestroy();
}

void Editor::initDefaultSaddyOptions()
{
	sad::Settings sett(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	sad::Renderer::ref()->init(sett);
	this->m_scene = new InterlockedScene(this);
	this->m_scene->setCamera(new OrthoCamera(false));
	sad::Renderer::ref()->setCurrentScene(this->m_scene);
	sad::Renderer::ref()->toggleFixedOn();
	sad::Renderer::ref()->textures()->setLoader("PNG", new PicoPNGTextureLoader());
	// Try to load default icons
	QString a = QApplication::applicationDirPath();
	a = QDir(a).filePath(ICONS_XML);
	XMLConfigLoader * loader = new XMLConfigLoader(a);
	m_icons.setLoader(loader);
	bool loaded =  (m_icons.reload() == SCR_OK);
	SL_DEBUG(QString("Loading icons from %1").arg(a));
	if (!loaded) 
	{
		SL_FATAL(QString("Can\'t load %1").arg(a));
	}
	this->assertSaddyInit(loaded);
}

Sprite2DConfig & Editor::icons()
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
	sad::Input::ref()->setKeyDownHandler(new EditorEventHandler(this, &EditorBehaviour::onKeyDown));
	sad::Input::ref()->setKeyUpHandler(new EditorEventHandler(this, &EditorBehaviour::onKeyUp));
	sad::Input::ref()->setMouseWheelHandler(new EditorEventHandler(this, &EditorBehaviour::onWheel));
	sad::Input::ref()->setMouseDownHandler(new EditorEventHandler(this, &EditorBehaviour::onMouseDown));
	sad::Input::ref()->setMouseUpHandler(new EditorEventHandler(this, &EditorBehaviour::onMouseUp));
	sad::Input::ref()->setMouseMoveHandler(new EditorEventHandler(this, &EditorBehaviour::onMouseMove));
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
		SL_DEBUG(QString("Can\'t find editor behaviour, named %1").arg(name.data()));
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


void Editor::highlightState(UNUSED const hst::string & hint)
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




