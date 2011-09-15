#include "renderer.h"
#include "texturemanager.h"
#include "log.h"

#define LOG_WRITE(MESG)    hst::log::inst()->owrite(hst::string(MESG))

//Single buffered
static int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE, 8, 
    GLX_GREEN_SIZE, 8, 
    GLX_BLUE_SIZE, 8, 
    GLX_DEPTH_SIZE, 16,
    None};

//Double buffered
static int attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, 
    GLX_RED_SIZE, 8, 
    GLX_GREEN_SIZE, 8, 
    GLX_BLUE_SIZE, 8, 
    GLX_DEPTH_SIZE, 16,
    None };

void sad::Renderer::sendFSAtom(int flag)
{
	XEvent xev;
        Atom wm_state = XInternAtom(m_window.dpy, "_NET_WM_STATE", False);
        Atom fullscreen = XInternAtom(m_window.dpy, "_NET_WM_STATE_FULLSCREEN", False);

         for (int i=0;i<sizeof(XEvent);i++)
           ((unsigned char *)(&xev))[i]=0;

        xev.type = ClientMessage;
        xev.xclient.window = m_window.win;
        xev.xclient.message_type = wm_state;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = flag;
        xev.xclient.data.l[1] = fullscreen;
        xev.xclient.data.l[2] = 0;

       XSendEvent(m_window.dpy, DefaultRootWindow(m_window.dpy), False, SubstructureNotifyMask, &xev);
      
       if (flag==1)
       {
       XWindowAttributes xwa;
       XGetWindowAttributes(m_window.dpy, DefaultRootWindow(m_window.dpy), &xwa);
       XMoveResizeWindow(m_window.dpy, m_window.win, 0, 0, xwa.width, xwa.height);
      }
}
hst::point<hst::D3,float> sad::Renderer::mousePos()
{
    int x=0, y=0;
	int wx=0, wy=0;
    unsigned int mask=0;
	X11Window rootw,childw;
	float px=0.0f,py=0.0f,pz=0.0f;
	XQueryPointer(m_window.dpy, m_window.win, &rootw ,&childw, &x, &y, &wx, &wy, &mask);
	mapToOGL((float)wx,(float)wy,px,py,pz);
	return hst::point<hst::D3,float>(px,py,pz);
}

void sad::Renderer::releaseWindow()
{
    if (m_window.ctx)
    {
        if (!glXMakeCurrent(m_window.dpy, None, NULL))
        {
            printf("Renderer: Could not release drawing context.\n");
        }
        glXDestroyContext(m_window.dpy, m_window.ctx);
        m_window.ctx = NULL;
    }
    
    XCloseDisplay(m_window.dpy);
}

bool sad::Renderer::XContextInit()
{
	static bool init=false;
        if (!init)
        {
          init=true;
	  /* get a connection */
  	 m_window.dpy = XOpenDisplay(0);
  	 m_window.screen = DefaultScreen(m_window.dpy);
   
  	m_window.vi = glXChooseVisual(m_window.dpy, m_window.screen, attrListDbl);
  	if (m_window.vi == NULL)
  	{
        	m_window.vi = glXChooseVisual(m_window.dpy, m_window.screen, attrListSgl);
        	LOG_WRITE("Renderer: can't init doublebuffering, defaulting to singlebuffering\n");
  	}
  	if (m_window.vi==NULL)
  	{
  		LOG_WRITE("Renderer: can't init XVisualInfo, quitting\n");
        	return false;
  	}
  	m_window.ctx = glXCreateContext(m_window.dpy, m_window.vi, 0, GL_TRUE);
         init=true;
        }
	return true;
}
bool sad::Renderer::createWindow()
{
  Colormap cmap;
  Atom wmDelete;
  X11Window winDummy;
  unsigned int borderDummy=0;
  
  if (!XContextInit()) return false;  
  
 cmap = XCreateColormap(m_window.dpy, RootWindow(m_window.dpy, m_window.vi->screen),m_window.vi->visual, AllocNone);
 m_window.attr.colormap = cmap;
 m_window.attr.border_pixel = 0;
 m_window.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | PointerMotionMask | ButtonReleaseMask | KeyReleaseMask;
 
 m_window.win = XCreateWindow(m_window.dpy, RootWindow(m_window.dpy, m_window.vi->screen), 0, 0, m_glsettings.width(), m_glsettings.height(), 0, m_window.vi->depth, InputOutput, m_window.vi->visual, CWBorderPixel | CWColormap | CWEventMask, &m_window.attr);
 wmDelete = XInternAtom(m_window.dpy, "WM_DELETE_WINDOW", True);
 XSetWMProtocols(m_window.dpy, m_window.win, &wmDelete, 1);
 XSetStandardProperties(m_window.dpy, m_window.win, m_windowtitle.data(), m_windowtitle.data(), None, NULL, 0, NULL);
 XMapRaised(m_window.dpy, m_window.win);

 reshape(m_glsettings.width(),m_glsettings.height());
 

 glXMakeCurrent(m_window.dpy, m_window.win, m_window.ctx);
 XGetGeometry(m_window.dpy, m_window.win, &winDummy, &m_window.x, &m_window.y,&m_window.width, &m_window.height, &borderDummy, &m_window.depth);
 
 if (!initGLRendering())
 {
     this->releaseWindow();
     LOG_WRITE("Renderer: can't init GL rendering\n");
     return false;
 }
 
 if  (m_window.fullscreen)
 {
     sendFSAtom(1);
 }

  return true;	
}

void sad::Renderer::update()
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
  
 if (getCurrentScene())
 	getCurrentScene()->render();
 glXSwapBuffers(m_window.dpy, m_window.win);
}

sad::Renderer::Renderer()
{
  m_windowtitle="SadExample";
  m_created=false;
  m_window.fullscreen=false;
  m_window.active=true;
  m_window.ctx=NULL;
  m_currentscene=NULL;
}


void sad::Renderer::setWindowTitle(const hst::string & s)
{
	m_windowtitle=s;
	if (m_window.ctx)
         XStoreName(m_window.dpy,m_window.win,s.data());
}

void sad::Renderer::quit()
{
  m_running=false;
  m_created=false;
}

void sad::Renderer::toggleFullscreen()								// Toggle Fullscreen/Windowed
{
  if (m_running)
  {
   this->m_window.fullscreen=!this->m_window.fullscreen;
   if (this->m_window.fullscreen)
   {
     sendFSAtom(0);			
   }
   else
   {
      sendFSAtom(1);
   }
  }
  else
  {
	  this->m_window.fullscreen=!this->m_window.fullscreen;
  }
}

void sad::Renderer::toggleFixedOn()
{
	XSizeHints * size_hints;
	size_hints=XAllocSizeHints();
	size_hints->flags=PMinSize | PMaxSize;
	size_hints->min_width=m_window.width;
	size_hints->max_width=m_window.width;
	size_hints->min_height=m_window.height;
	size_hints->max_height=m_window.height;
	XSetWMNormalHints(m_window.dpy,m_window.win,size_hints);
}

void sad::Renderer::toggleFixedOff()
{
	XSizeHints * size_hints;
	size_hints=XAllocSizeHints();
	size_hints->flags=PMinSize | PMaxSize;
	size_hints->min_width=1;
	size_hints->max_width=40000;
	size_hints->min_height=1;
	size_hints->max_height=30000;
	XSetWMNormalHints(m_window.dpy,m_window.win,size_hints);
}

