#include <QTimer>
#include "editor.h"
#include <orthocamera.h>
#include <log.h>

Editor::Editor() 
{
	m_cmdargs = NULL;
	m_rendermutex = new os::mutex();
	m_initmutex = new os::mutex();
	m_saddywaitmutex = new os::mutex();
	m_renderthread = new Editor::SaddyThread(this);
	m_waitforqt = false;
	m_waitforsaddy = false;
	m_qtapp = NULL;
}

void Editor::init(int argc,char ** argv)
{
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
	delete m_saddywaitmutex;
	delete m_qtapp;
	delete m_initmutex;
	delete m_renderthread;
	delete m_rendermutex;
	delete m_cmdargs;
}

void Editor::SaddyThread::run() 
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

void Editor::SaddyThread::waitForQtThread() 
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
	this->m_scene->setCamera(new OrthoCamera(true));
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