#include "editor.h"

Editor::Editor() 
{
	m_cmdargs = NULL;
	m_rendermutex = new os::mutex();
}

void Editor::init(int argc,const char ** argv)
{
	m_cmdargs = new CommandArguments(argc, argv);
}

void Editor::quit() 
{

}

Editor::~Editor() 
{
	delete m_rendermutex;
	delete m_cmdargs;
}

void Editor::SaddyThread::run() 
{
	this->m_editor->initSaddyActions();
	QTimer::singleShot(0, this->m_editor, SLOT(initQtActions()));
	QTimer::singleShot(0, this->m_editor, SLOT(runQtEventLoop()));
	this->m_editor->runQtEventLoop();
}

void Editor::initSaddyActions() 
{
}

void Editor::initQtActions() 
{
}

void Editor::runQtEventLoop()
{
}

void Editor::runSaddyEventLoop() 
{
}