/*! \file   vbolayer.h
	\author HiddenSeeker
	Contains a simple wrapper to work with Vertex Buffer Objects
*/
#include "extchecker.h"
#ifdef   _WIN32
    #include <windows.h>
    #include <gl/gl.h>														
    #include <gl/glu.h>
#else
	#include <GL/gl.h>														
    #include <GL/glu.h>
#endif
#define GL_GLEXT_PROTOTYPES
#include "primitives/glext.h"

/*! \class VBO
	Provides a simple access functions for Vertex Buffer Objects.
	
	HINT: You must create this objects only after sad::Renderer::init. That's because
	checks for extensions
*/
class VBO
{
 private:
	     GLuint m_id;     //!< ID of buffer
#ifdef WIN32
static PFNGLGENBUFFERSARBPROC pglGenBuffersARB ;                     // VBO Name Generation Procedure
static PFNGLBINDBUFFERARBPROC pglBindBufferARB ;                     // VBO Bind Procedure
static PFNGLBUFFERDATAARBPROC pglBufferDataARB ;                     // VBO Data Loading Procedure
static PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB ;               // VBO Sub Data Loading Procedure
static PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB ;               // VBO Deletion Procedure
static PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB ; // return various parameters of VBO
static PFNGLMAPBUFFERARBPROC pglMapBufferARB ;                       // map VBO procedure
static PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB ;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#endif
			static bool initted;
			static bool error;
			static void init();
 public:
	    //Detects, whether it'supported
		static bool supported();
		//Usage
		enum Usage
		{
		  StreamDraw=GL_STREAM_DRAW,
		  StreamRead=GL_STREAM_READ,
		  StreamCopy=GL_STREAM_COPY,
		  StaticDraw=GL_STATIC_DRAW,
		  StaticRead=GL_STATIC_READ,
		  StaticCopy=GL_STATIC_COPY,
		  DynamicDraw=GL_DYNAMIC_DRAW,
		  DynamicRead=GL_DYNAMIC_READ,
		  DynamicCopy=GL_DYNAMIC_COPY
		};
		//Access for mapping
		enum Access
		{
			RO=GL_READ_ONLY,
			WO=GL_WRITE_ONLY,
			RW=GL_READ_WRITE
		};
		inline GLuint id() const;

		/*! Creates an empty vbo
		*/
	    VBO();
		/*! Uses glBufferDataARB to store data
		     \param[in] data  to store
			 \param[in] sz    size
			 \param[in] usage usage
		*/
		void store(void * data, size_t sz,VBO::Usage usage);
		/*! Uses glBufferSubData to store. Must be called after ::store()
			 \param[in] data  to store
			 \param[in] sz    size
			 \param[in] off   offset
		*/
		void substore(void * data, size_t sz,size_t offset);
		//! Binds a buffer
		void bind();
		/*! Maps a buffer for access
			\param[in] ac desired access
		*/
		void * map(VBO::Access ac);
		/*! Unmaps a buffer
		*/
		void unmap();
		/*! Deletes a VBO, removing buffer
		*/
		~VBO();
};


//=============Source code==================
inline GLuint VBO::id() const {return m_id;}
