#define  DO_NOT_CLEANUP_X11_NAMESPACE
#include <x11recode.h>
#include <time.h>
#include <sched.h>

static clock_t dblclick=0;
static clock_t clk=0;
static float freq=25000;
static int lastkey=0;

#define DOUBLE_CLICK_FREQ 0.25

static int predicate(Display *, XEvent * e, char *)
{
	return true;
}

void sad::Renderer::mainLoop()
{
  m_running = true;											// Loop program
  m_window.active=true;
  XEvent event;
  m_fps = 75;
  m_setimmediately = true;
  m_reset = false;
  m_frames = 0;
  bool altstate=false;
  ::Window  winDummy = 0;
  while(m_running)
  {
        if (m_setimmediately || m_reset)
       {
	  m_timer.start();
	  m_reset = false;
       }
  	while (XCheckIfEvent(m_window.dpy, &event, predicate, NULL) )
        {
             switch (event.type)
            {
            	case Expose:              {
	                                            m_window.active=true; 
                                                    break;
                                                  }
               case ConfigureNotify:  {
               	                                     if ((event.xconfigure.width != m_window.width) ||  (event.xconfigure.height != m_window.height))
                                                     {  
													    reshape(event.xconfigure.width,event.xconfigure.height);      
                                                     }
                                                     break;
               	                                  }
               case ButtonRelease:    {  //Button release
                                                      if (event.xbutton.button==1 || event.xbutton.button==3 || event.xbutton.button==2)
                                                      {
                    	                                if (this->controls()->areUpNotTracked() == false)
						        {
							 float mx=0,my=0,mz=0;
							 int key=(event.xbutton.button==1)?MOUSE_BUTTON_LEFT:(event.xbutton.button==3)?MOUSE_BUTTON_RIGHT:MOUSE_BUTTON_MIDDLE;
							 unsigned int borderDummy = 0;
						         XGetGeometry(m_window.dpy, m_window.win, &winDummy, &m_window.x, &m_window.y,&m_window.width, &m_window.height, &borderDummy, &m_window.depth);
							 mapToOGL(event.xbutton.x,event.xbutton.y,mx,my,mz);
							 this->controls()->postMouseUp(sad::Event(mx,my,mz,key));
							}
                                                      }
                                                      break;
                                                  }
               case ButtonPress:      {  //Handle button press and double click
               	                                    float mx=0,my=0,mz=0;
						    unsigned  int borderDummy = 0;
						   XGetGeometry(m_window.dpy, m_window.win, &winDummy, &m_window.x, &m_window.y,&m_window.width, &m_window.height, &borderDummy, &m_window.depth);
                                                    mapToOGL(event.xbutton.x,event.xbutton.y,mx,my,mz);
                                                    if (event.xbutton.button==1 || event.xbutton.button==3 || event.xbutton.button==2)
                                                    {
                             	                        //Handle button press and click
                                                        int key=(event.xbutton.button==1)?MOUSE_BUTTON_LEFT:(event.xbutton.button==3)?MOUSE_BUTTON_RIGHT:MOUSE_BUTTON_MIDDLE;
                                                        if  (this->controls()->areDownNotTracked() == false || this->controls()->areClickNotTracked() == false)
                                                        {
							this->controls()->postMouseDown(sad::Event(mx,my,mz,key));
							this->controls()->postMouseClick(sad::Event(mx,my,mz,key));
                                                        }
                                                        //Handle double click
                                                        clk=clock();
                    					freq=(float)clk-(float)dblclick; freq/=CLOCKS_PER_SEC;
                    				        dblclick=clk;      
                    					if (freq<DOUBLE_CLICK_FREQ && lastkey==key)
                                                       {
                                                         this->controls()->postMouseDblClick(sad::Event(mx,my,mz,key));
                                                       }
                                                       lastkey=key;
                                                    }
						    else
						    {  //Handle wheel events
							float fw=(event.xbutton.button==4)?1.0:((event.xbutton.button==5)?-1.0:0.0);
                                                        int key=0;
							sad::Event ev(mx,my,mz,key);
							ev.delta=fw;
							this->controls()->postMouseWheel(ev);
						    }
                                                    break;
               	                                 }
		case KeyPress:          {
                                                   int key = sad::recode(&event.xkey); 
						   if (key==KEY_LALT || key==KEY_RALT) altstate=true;
						   sad::Event sev(key);  sev.alt=altstate; sev.ctrl=(event.xkey.state & ControlMask) !=0;
						   sev.capslock=(event.xkey.state & LockMask) !=0; sev.shift=(event.xkey.state & ShiftMask) !=0;
						  this->controls()->postKeyDown(sev);
                    				   break;
                                                 }
		case KeyRelease:      {
						   int key = sad::recode(&event.xkey); 
						   if (key==KEY_LALT || key==KEY_RALT) altstate=false;
						   sad::Event sev(key); sev.alt=altstate; sev.ctrl=(event.xkey.state & ControlMask) !=0;
						   sev.capslock=(event.xkey.state & LockMask) !=0; sev.shift=(event.xkey.state & ShiftMask) !=0;
						  this->controls()->postKeyUp(sev);
                    				   break;
			                         }
		case MotionNotify:    { //MouseMove Event
                    				   if (this->controls()->areMovingNotTracked() == false) 
                                                   { 
                                                      float mx=0,my=0,mz=0;
						      int key=0;
						      if (event.xmotion.state & Button1MotionMask) key=MOUSE_BUTTON_LEFT;
						      if (event.xmotion.state & Button2MotionMask) key=MOUSE_BUTTON_MIDDLE;
						      if (event.xmotion.state & Button3MotionMask) key=MOUSE_BUTTON_RIGHT;
						      unsigned  int borderDummy = 0;
						      XGetGeometry(m_window.dpy, m_window.win, &winDummy, &m_window.x, &m_window.y,&m_window.width, &m_window.height, &borderDummy, &m_window.depth); 
						      mapToOGL(event.xmotion.x, event.xmotion.y,mx,my,mz); 
						     this->controls()->postMouseMove(sad::Event(mx,my,mz,key));
                    				   } 
                                                   break;
                                                 }
		case ClientMessage: {    
                       				   if (*XGetAtomName(m_window.dpy, event.xclient.message_type)          //Represents a closing window
                        				== *"WM_PROTOCOLS")
                    				  {
                        			    m_running=false;
                    				  }
                    				  break;
						}
		case MapNotify:       { m_window.active=true; break; }  //Maximize and restore
		case UnmapNotify:   { m_window.active=false; break; } //Minimize   
        };
       }
       //Update a window, if active
       if (m_window.active)
       {
		update();
       }
       else
       {
		sched_yield();
       }
       
	//Change scene, if need so
	if (m_chscene) 
	{ setCurrentScene(m_chscene); m_chscene=NULL;}
	m_timer.stop();
	double elapsed = m_timer.elapsed();
	// Reset counter to avoid FPS jumps
	if (m_window.active == false)  m_timer.start();
	if (m_setimmediately || elapsed  > 2000.0)
	{
		setFPS( 1000.0 * m_frames / m_timer.elapsed() );
		m_frames = 0;
		m_reset = true;
		m_setimmediately = false;
	}
  }
 this->controls()->postQuit();
  m_window.active=false;
  this->releaseWindow();
}

