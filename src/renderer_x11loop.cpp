#include <time.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include "os/windowhandles.h"
#include "window.h"
#include "glcontext.h"
#include "x11recode.h"
#include "fpsinterpolation.h"
#include "os/systemwindowevent.h"
#include "os/keydecoder.h"
#include <X11/Xlocale.h>
#include <locale.h>

static clock_t dblclick=0;
static clock_t clk=0;
static float freq=25000;
static int lastkey=0;

#define DOUBLE_CLICK_FREQ 0.25

static int predicate(Display *, XEvent * e, char *)
{
	return true;
}

sad::os::KeyDecoder decoder;

void sad::Renderer::mainLoop()
{
	sad::os::SystemWindowEvent ev;
	// Set realtime priority
	pid_t myprocesspid = getpid();
	sched_param param;
	param.sched_priority = 77; // Don't set too much, since we still may need to switch to other windows	
	if (sched_setscheduler(myprocesspid, SCHED_FIFO, &param) != 0) 
	{
		SL_LOCAL_INTERNAL("Failed to set process priority", *this);
	}
  
	m_running = true;											
	m_window->setActive(true);
	fpsInterpolation()->reset();
	bool altstate=false;
	
	::Window  winDummy = 0;
	XEvent event;
	
	
	setlocale(LC_CTYPE, "");
	XSetLocaleModifiers("");

	while(m_running)
	{
		while (XCheckIfEvent(m_window->handles()->Dpy, &event, predicate, NULL) )
		{
			switch (event.type)
			{
				case Expose:                 {
												m_window->setActive(true);
												break;
                                             }
               case ConfigureNotify:         {
												reshape(event.xconfigure.width,event.xconfigure.height);      
												break;
               	                             }
               case ButtonRelease:    		 { 
												if (event.xbutton.button==1 || event.xbutton.button==3 || event.xbutton.button==2)
												{
													if (this->controls()->areUpNotTracked() == false)
													{
														int key=(event.xbutton.button==1)?MOUSE_BUTTON_LEFT:(event.xbutton.button==3)?MOUSE_BUTTON_RIGHT:MOUSE_BUTTON_MIDDLE;
														sad::Point2D p(event.xbutton.x,event.xbutton.y);
														p = this->window()->toClient(p);
														sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
														this->controls()->postMouseUp(sad::Event(op.x(),op.y(),op.z(),key));
													}
													break;
                                                }
											}
               case ButtonPress:      {  //Handle button press and double click
               	                          sad::Point2D p(event.xbutton.x,event.xbutton.y);
										  p = this->window()->toClient(p);
										 sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
                                                    if (event.xbutton.button==1 || event.xbutton.button==3 || event.xbutton.button==2)
                                                    {
                             	                        //Handle button press and click
                                                        int key=(event.xbutton.button==1)?MOUSE_BUTTON_LEFT:(event.xbutton.button==3)?MOUSE_BUTTON_RIGHT:MOUSE_BUTTON_MIDDLE;
                                                        if  (this->controls()->areDownNotTracked() == false || this->controls()->areClickNotTracked() == false)
                                                        {
							this->controls()->postMouseDown(sad::Event(op.x(), op.y(), op.z(),key));
							this->controls()->postMouseClick(sad::Event(op.x(), op.y(), op.z(),key));
                                                        }
                                                        //Handle double click
                                                        clk=clock();
                    					freq=(float)clk-(float)dblclick; freq/=CLOCKS_PER_SEC;
                    				        dblclick=clk;      
                    					if (freq<DOUBLE_CLICK_FREQ && lastkey==key)
                                                       {
                                                         this->controls()->postMouseDblClick(sad::Event(op.x(), op.y(), op.z(), key));
                                                       }
                                                       lastkey=key;
                                                    }
						    else
						    {  //Handle wheel events
							float fw=(event.xbutton.button==4)?1.0:((event.xbutton.button==5)?-1.0:0.0);
                                                        int key=0;
							sad::Event ev(op.x(), op.y(), op.z(),key);
							ev.delta=fw;
							this->controls()->postMouseWheel(ev);
						    }
                                                    break;
               	                                 }
		case KeyPress:          {
                                                   int key = sad::recode(&event.xkey); 
						   ev.Event = event;
						   sad::Maybe<sad::String> result = decoder.convert(&ev, this->window());
						   if (result.exists())
						   {
								SL_LOCAL_DEBUG(result.value(), *this); 
						   }
						   else
						   {
								SL_LOCAL_DEBUG("Cannot print key data!", *this);
						   }	
						   fflush(stdout);
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
               	                          sad::Point2D p(event.xbutton.x,event.xbutton.y);
										  p = this->window()->toClient(p);
										 sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
						      int key=0;
						      if (event.xmotion.state & Button1MotionMask) key=MOUSE_BUTTON_LEFT;
						      if (event.xmotion.state & Button2MotionMask) key=MOUSE_BUTTON_MIDDLE;
						      if (event.xmotion.state & Button3MotionMask) key=MOUSE_BUTTON_RIGHT;
						      this->controls()->postMouseMove(sad::Event(op.x(), op.y(), op.z(),key));
                    				   } 
                                                   break;
                                                 }
		case ClientMessage: {    
                       				   if (*XGetAtomName(m_window->handles()->Dpy, event.xclient.message_type)          //Represents a closing window
                        				== *"WM_PROTOCOLS")
                    				  {
                        			    m_running=false;
                    				  }
                    				  break;
						}
		case MapNotify:       { m_window->setActive(true); break; } 
		case UnmapNotify:   { m_window->setActive(false); break; } 
        };
       }
       //Update a window, if active
       if (m_window->active())
       {
		update();
       }
       else
       {
		sched_yield();
       }       
	}
	this->controls()->postQuit();
	m_window->setActive(false);
	m_running = false;
}

