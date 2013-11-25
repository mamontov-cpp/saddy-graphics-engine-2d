#include "texture.h"
#include "texturemanager.h"
#include "log/log.h"

#include "3rdparty/glext/glext.h"
#include <renderer.h>
#include <opengl.h>

#include <errno.h>

#ifdef LINUX
#include <renderer.h>
#include <GL/gl.h>
#include <GL/glx.h>

#ifdef  GLX_ARB_get_proc_adress
	#define glxGetProcAdress glXGetProcAddressARB
#else
	#define glxGetProcAdress glXGetProcAddress
#endif

#endif

#ifdef WIN32
	#include <windows.h>
#endif


static PFNGLGENERATEMIPMAPEXTPROC glGenerateMipMapsSaddy  = NULL;


static PFNGLGENERATEMIPMAPEXTPROC getglGenerateMipMaps()
{
	if (glGenerateMipMapsSaddy == NULL)
	{
#ifdef WIN32
		glGenerateMipMapsSaddy = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress((LPCSTR)"glGenerateMipmap");
#else
		glGenerateMipMapsSaddy = (PFNGLGENERATEMIPMAPEXTPROC)(glxGetProcAdress)((const GLubyte*)"glGenerateMipmap");
#endif
	}
	return glGenerateMipMapsSaddy;
}


 

sad::Texture::Texture()
{
	// Set default mode of texture
	m_mode   = BORDER_CLAMP;	
	// Set default filter of texture
	m_filter = 10;		
	// Make uploading to gpu necessary
	m_ongpu  = false;      
}

sad::Texture::~Texture()
{
	this->unload();
}

void sad::Texture::bind()
{
	if (!m_ongpu)
		upload();
	glBindTexture(GL_TEXTURE_2D, m_id);
}


void sad::Texture::upload()
{
	m_ongpu = true;
	
	// Get texture type and components
	GLuint type,components;
	switch (m_bpp)
	{
		case 24: 
			type=GL_RGB;components=3; break;
		case 32: 
			type=GL_RGBA;components=4; break;
	};
    
	glGenTextures(1, (GLuint *)&m_id);	
	glBindTexture(GL_TEXTURE_2D, m_id);
	GLint result = glGetError();
	if (result)
	{
		// We should check here for other context stuff
	}
	GLuint what;
	switch (m_mode)
	{
		case BORDER_CLAMP:	what=GL_CLAMP;break;
		case BORDER_REPEAT: what=GL_REPEAT;break;
	}

	makePOT();

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, what);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, what);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	


	// Build Mip Maps	
	GLint res;
	sad::Pair<int,int> version = sad::Renderer::ref()->opengl()->version();
	if (version.p1() < 3) // In OpenGL 3.0  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);  is deprecated
	{
		if (version.p1() == 1 && version.p2() < 4)
		{
			res=gluBuild2DMipmaps(GL_TEXTURE_2D, components, m_width, m_height, type, GL_UNSIGNED_BYTE, m_data.data());
		}
		else
		{
			  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
			  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, type, GL_UNSIGNED_BYTE, m_data.data());
			  res = glGetError();
		}
	} 
	else
	{
		float max  = (float)((m_width > m_height) ? m_width : m_height);
		int levels = (int)(::log(max)/::log(2.0f));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  m_width, m_height, 0, type, GL_UNSIGNED_BYTE, m_data.data());			  
		res = glGetError();
		PFNGLGENERATEMIPMAPEXTPROC proc = getglGenerateMipMaps();
		if (proc != NULL)
		{
			proc(GL_TEXTURE_2D);
		}
		else
		{
			// TODO: Add here critical error message.
		}
		res = glGetError();
	}
	if (res)
	{
		SL_FATAL(gluErrorString(res));
	}

	
}

void sad::Texture::setAlpha(sad::uchar a)
{
	m_bpp=32;
	for (unsigned int i=3;i<m_data.count();i+=4)
	{
		m_data[i]=255-a;
	}
}
void sad::Texture::setAlpha(sad::uchar a, const sad::Color & clr, sad::uchar prec)
{
	m_bpp=32;
	for (unsigned int i=3;i<m_data.count();i+=4)
	{
	  bool rck=abs(m_data[i-3]-clr.r())<=prec;
	  bool gck=abs(m_data[i-2]-clr.g())<=prec;
	  bool bck=abs(m_data[i-1]-clr.b())<=prec;
	  if (rck && gck && bck)
		m_data[i]=255-a;
	}
}

void sad::Texture::setAlpha(sad::uchar a, const sad::Color & clr,const sad::Rect2D & rect)
{
	sad::Rect2D tmp=rect;
	for (int i=0;i<4;i++)
	{
		if (tmp[i].x()<0) tmp[i].setX(0);
		if (tmp[i].y()<0) tmp[i].setY(0);
		if (tmp[i].x()>width()-1) tmp[i].setX(this->width()-1);
		if (tmp[i].y()>height()-1) tmp[i].setY(this->height()-1);
	}
	if (tmp[2].x()<tmp[0].x())
	{
		double f=tmp[2].x(); tmp[2].setX(tmp[0].x()); tmp[0].setX(f);
	}
	
	if (tmp[2].y()<tmp[0].y())
	{
		double f=tmp[2].y(); tmp[2].setY(tmp[0].y()); tmp[0].setY(f);
	}
	int minx=(int)tmp[0].x();
	int maxx=(int)tmp[2].x();
	int miny=(int)tmp[0].y();
	int maxy=(int)tmp[2].y();
	for (int row=minx;row<=maxx;row++)
	{
		for (int col=miny;col<=maxy;col++)
		{
			sad::uchar * pix=this->pixel(row,col);
			bool rck=abs(clr.r()-pix[0])<10;
			bool gck=abs(clr.g()-pix[1])<10;
			bool bck=abs(clr.b()-pix[2])<10;
			if (rck && gck && bck)
			{
				setPixelAlpha(row,col,0);
			}
		}
	}

}
void sad::Texture::setMode(Texture::Mode mode)
{
	m_mode=mode;
}

bool sad::Texture::load(const sad::String & filename, sad::Renderer * r)
{
	sad::String ff(filename.getExtension());
	char * f=const_cast<char *>(ff.data());
	while(*f) { *f=toupper(*f); ++f; }

	sad::imageformats::Loader * l = r->textures()->loader(ff);
	if (l)
	{
		FILE * fl = fopen(filename.data(), "rb");
		if (fl)
		{
			bool result = l->load(fl, this);
			fclose(fl);
			return result;
		}
	}
	return false;
}
bool sad::Texture::load(const sad::WString & filename, sad::Renderer * r)
{
	char * tmp=new char[2*filename.length()+2];
	wcstombs(tmp,filename.data(),2*filename.length()+2);
	sad::String tt(tmp);
	delete tmp;
	return load(tt, r);
}

void sad::Texture::unload()
{
	if (m_ongpu)
		glDeleteTextures(1,&m_id);
	m_ongpu = false;
}

