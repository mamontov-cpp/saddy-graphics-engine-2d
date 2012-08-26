#include "editor.h"

Editor::Editor() 
{
	m_cmdargs = NULL;
	m_rendermutex = new os::mutex();
	m_initmutex = new os::mutex();
	m_renderthread = new Editor::SaddyThread(this);
	m_waitforqt = false;
}

void Editor::init(int argc,const char ** argv)
{
	m_cmdargs = new CommandArguments(argc, argv);
	// This thread also runs ALL of event loops
	
	m_renderthread->start();
}

void Editor::quit() 
{

}

Editor::~Editor() 
{
	delete m_initmutex;
	delete m_renderthread;
	delete m_rendermutex;
	delete m_cmdargs;
}

void Editor::SaddyThread::run() 
{
	this->m_editor->initSaddyActions();
	QTimer::singleShot(0, this->m_editor, SLOT(initQtActions()));
	this->waitForQtThread();
	QTimer::singleShot(0, this->m_editor, SLOT(runQtEventLoop()));
	this->waitForQtThread();
	this->m_editor->runSaddyEventLoop();
}

void Editor::SaddyThread::waitForQtThread() 
{
	while(this->m_editor->shouldSaddyThreadWaitForQt()) {
		this->msleep(100);
	}
}

void Editor::initSaddyActions() 
{
}

void Editor::initQtActions() 
{
	this->awakeSaddyThread();
}

void Editor::runQtEventLoop()
{
	this->awakeSaddyThread();
}

void Editor::runSaddyEventLoop() 
{
}