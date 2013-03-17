#include "vbolayer.h"
#include "log.h"



bool VBO::error=false;
bool VBO::initted=false;

VBO::VBO()
{
	m_id=0;
	if (!initted) { init(); initted=true; }
}
#ifdef WIN32
PFNGLGENBUFFERSARBPROC VBO::pglGenBuffersARB=0 ;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC VBO::pglBindBufferARB=0 ;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC VBO::pglBufferDataARB=0 ;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC VBO::pglBufferSubDataARB=0 ;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC VBO::pglDeleteBuffersARB=0 ;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC VBO::pglGetBufferParameterivARB=0 ; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC VBO::pglMapBufferARB=0 ;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC VBO::pglUnmapBufferARB=0 ;                   // unmap VBO procedure
#else
extern "C"
{
//Dunno why, but on some compilers I can't got it to work
GLAPI void APIENTRY glBindBufferARB (GLenum, GLuint);
GLAPI void APIENTRY glDeleteBuffersARB (GLsizei, const GLuint *);
GLAPI void APIENTRY glGenBuffersARB (GLsizei, GLuint *);
GLAPI GLboolean APIENTRY glIsBufferARB (GLuint);
GLAPI void APIENTRY glBufferDataARB (GLenum, GLsizeiptrARB, const GLvoid *, GLenum);
GLAPI void APIENTRY glBufferSubDataARB (GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid *);
GLAPI void APIENTRY glGetBufferSubDataARB (GLenum, GLintptrARB, GLsizeiptrARB, GLvoid *);
GLAPI GLvoid* APIENTRY glMapBufferARB (GLenum, GLenum);
GLAPI GLboolean APIENTRY glUnmapBufferARB (GLenum);
GLAPI void APIENTRY glGetBufferParameterivARB (GLenum, GLenum, GLint *);
GLAPI void APIENTRY glGetBufferPointervARB (GLenum, GLenum, GLvoid**);
}
#endif
void VBO::init()
{
	error=!ext::presented("GL_ARB_vertex_buffer_object");

#ifdef WIN32
        glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
        glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
        glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
        glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
        glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
        glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
        glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
		error= error || (glGenBuffersARB==0);
		error= error || (glBindBufferARB==0);
		error= error || (glBufferDataARB==0);
		error= error || (glBufferSubDataARB==0);
		error= error || (glDeleteBuffersARB==0);
		error= error || (glGetBufferParameterivARB==0);
		error= error || (glMapBufferARB==0);
		error= error || (glUnmapBufferARB==0);
#endif
		if (error) { hst::log::inst()->owrite(hst::string("VBO: VBO is not supported.\n"));}
}

bool VBO::supported()
{
	if (!initted) { init(); initted=true; }
	return error;
}
VBO::~VBO()
{
	if (m_id) glDeleteBuffersARB(1,&m_id);
}

void VBO::bind()
{
	if (supported() && m_id)
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_id);
}
void * VBO::map(VBO::Access ac)
{
	if (supported() && m_id)
	{
		bind();
		return glMapBufferARB(GL_ARRAY_BUFFER_ARB,(GLenum)ac);
	}
	else 
	    return NULL;
}
void VBO::unmap()
{
	if (supported() && m_id)
	{
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	}
}
void VBO::store(void * data, size_t sz,VBO::Usage usage)
{
	if (supported())
	{
	  glGenBuffersARB(1,&m_id);
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_id);
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, sz, data, (GLenum)usage);
	}
}
void VBO::substore(void * data, size_t sz,size_t offset)
{
	if (supported())
	{
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,(GLintptr)offset,(GLsizeiptr)sz,data);
	}
}
