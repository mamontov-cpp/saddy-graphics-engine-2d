#include "glcontext.h"
#include "os/glcontextimpl.h"


sad::GLContext::GLContext()
: m_dptr(new sad::os::GLContextImpl())
{

}

sad::GLContext::~GLContext()
{
	delete m_dptr;
}

bool sad::GLContext::valid() const
{
	return m_dptr->valid();
}

void sad::GLContext::swapBuffers()
{
	m_dptr->swapBuffers();
}

sad::Point3D sad::GLContext::mapToViewport(const sad::Point2D & p, bool ztest)
{
	return m_dptr->mapToViewport(p, ztest);
}

sad::os::GLContextHandle * sad::GLContext::handle() const
{
	return m_dptr->handle();
}

sad::Window * sad::GLContext::window() const
{
	return m_dptr->window();
}

bool sad::GLContext::isOpenGL3compatible() const
{
	return m_dptr->isOpenGL3compatible();
}


bool sad::GLContext::createFor(sad::Window * win)
{
	return m_dptr->createFor(win);
}

void sad::GLContext::destroy()
{
	m_dptr->destroy();
}

