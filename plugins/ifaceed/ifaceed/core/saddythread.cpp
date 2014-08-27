#include "core/saddythread.h"
#include "core/editor.h"

#include <renderer.h>

core::SaddyThread::SaddyThread(core::Editor * editor) : m_editor(editor)
{
	
}

void core::SaddyThread::waitForQtThread() 
{
	while(this->m_editor->shouldSaddyThreadWaitForQt()) {
		this->msleep(100);
	}
}

void core::SaddyThread::run() 
{
	sad::Renderer::ref()->init(sad::Settings(WINDOW_WIDTH, WINDOW_HEIGHT, false));
	sad::Renderer::ref()->setWindowTitle("Saddy Interface Editor");
	sad::Renderer::ref()->makeFixedSize();

	this->m_editor->awakeMainThread();

	this->m_editor->runSaddyEventLoop();
}
