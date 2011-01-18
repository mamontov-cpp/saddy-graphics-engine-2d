#include <x11recode.h>
#include <time.h>

static clock_t dblclick=0;
static clock_t clk=0;
static float freq=25000;
static int lastkey=0;

#define DOUBLE_CLICK_FREQ 0.25

void sad::Renderer::mainLoop()
{
  m_running = true;											// Loop program
  m_window.active=true;
  Renderer::setTimer();
  XEvent event;
  int frames=0;

  while(m_running)
  {
  	while (XPending(m_window.dpy) > 0)
        {
            XNextEvent(m_window.dpy, &event);
             switch (event.type)
            {
            	case Expose:              {
	                                            if (event.xexpose.count == 0) { m_window.active=true; update(); } //Expose event
                                                    break;
                                                  }
               case ConfigureNotify:  {
               	                                     if ((event.xconfigure.width != m_window.width) ||  (event.xconfigure.height != m_window.height))
                                                    {  //Resize event
                                                        m_window.width = event.xconfigure.width;
                                                        m_window.height = event.xconfigure.height;
                                                        reshape(event.xconfigure.width,event.xconfigure.height);
                                                     }
                                                     break;
               	                                  }
               case ButtonRelease:    {  //Button release
                                                      if (event.xbutton.button==1 || event.xbutton.button==3 || event.xbutton.button==2)
                                                      {
                    	                                if (!sad::Input::inst()->areUpNotTracked())
						        {
							 float mx=0,my=0,mz=0;
							 int key=(event.xbutton.button==1)?MOUSE_BUTTON_LEFT:(event.xbutton.button==3)?MOUSE_BUTTON_RIGHT:MOUSE_BUTTON_MIDDLE;
							 mapToOGL(event.xbutton.x,event.xbutton.y,mx,my,mz);
							 sad::Input::inst()->postMouseUp(sad::Event(mx,my,mz,key));
							}
                                                      }
                                                      break;
                                                  }
               case ButtonPress:      {  //Handle button press and double click
               	                                    float mx=0,my=0,mz=0;
                                                    mapToOGL(event.xbutton.x,event.xbutton.y,mx,my,mz);
                                                    if (event.xbutton.button==1 || event.xbutton.button==3 || event.xbutton.button==2)
                                                    {
                             	                        //Handle button press and click
                                                        int key=(event.xbutton.button==1)?MOUSE_BUTTON_LEFT:(event.xbutton.button==3)?MOUSE_BUTTON_RIGHT:MOUSE_BUTTON_MIDDLE;
                                                        if  (!sad::Input::inst()->areDownNotTracked() || !sad::Input::inst()->areClickNotTracked())
                                                        {
							 sad::Input::inst()->postMouseDown(sad::Event(mx,my,mz,key));
							 sad::Input::inst()->postMouseClick(sad::Event(mx,my,mz,key));
                                                        }
                                                        //Handle double click
                                                        clk=clock();
                    					freq=(float)clk-(float)dblclick; freq/=CLOCKS_PER_SEC;
                    				        dblclick=clk;      
                    					if (freq<DOUBLE_CLICK_FREQ && lastkey==key)
                                                       {
                                                          sad::Input::inst()->postMouseDblClick(sad::Event(mx,my,mz,key));
                                                       }
                                                       lastkey=key;
                                                    }
						    else
						    {  //Handle wheel events
							float fw=(event.xbutton.button==4)?1.0:((event.xbutton.button==5)?-1.0:0.0);
                                                        int key=0;
							sad::Event ev(mx,my,mz,key);
							ev.delta=fw;
							sad::Input::inst()->postMouseWheel(ev);
						    }
                                                    break;
               	                                 }
		case KeyPress:          {
                                                   int key = sad::recode(&event.xkey); 
						   sad::Input::inst()->postKeyDown(key);
                    				   break;
                                                 }
		case KeyRelease:      {
						   int key = sad::recode(&event.xkey); 
						   sad::Input::inst()->postKeyUp(key);
                    				   break;
			                         }
		case MotionNotify:    { //MouseMove Event
                    				   if (!sad::Input::inst()->areMovingNotTracked()) 
                                                   { 
                                                      float mx=0,my=0,mz=0;
						      int key=0;
						      if (event.xmotion.state & Button1MotionMask) key=MOUSE_BUTTON_LEFT;
						      if (event.xmotion.state & Button2MotionMask) key=MOUSE_BUTTON_MIDDLE;
						      if (event.xmotion.state & Button3MotionMask) key=MOUSE_BUTTON_RIGHT;
						      mapToOGL(event.xmotion.x,event.xmotion.y,mx,my,mz); 
						      sad::Input::inst()->postMouseMove(sad::Event(mx,my,mz,key));
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
	// Process Application Loop
	frames++;
	if (Renderer::instance().elapsedInMSeconds() >= 1000)
	{
		  m_fps = frames;frames=0;Renderer::instance().setTimer();
	}
	//Update a window, if active
	if (m_window.active)
	     update();
	//Change scene, if need so
	if (m_chscene) 
	{ setCurrentScene(m_chscene); m_chscene=NULL;}
  }

  m_window.active=false;
  this->releaseWindow();
}

