#include "extchecker.h"
#include <string.h>

#ifdef   _WIN32
    #include <windows.h>
    #include <gl/gl.h>														
    #include <gl/glu.h>
#else
	#include <GL/gl.h>														
    #include <GL/glu.h>
#endif


static const char * ext_list=NULL;

const char * ext::list()
{
  if (!ext_list)
  {
		ext_list=(const char *)glGetString(GL_EXTENSIONS);
  }
  return ext_list;
}
bool ext::presented(const char * name)
{
	if (ext::list())
	{
		return strstr(ext_list,name)!=NULL;
	}
	return false;
}
