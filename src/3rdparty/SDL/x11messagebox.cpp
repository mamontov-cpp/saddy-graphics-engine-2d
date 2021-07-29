/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  ===========================================================================================
  
  This is modified version of SDL_x11messagebox.c, which could be used with Saddy.
  Basically, I've made some changes, to improve compatibility, removed some unneeded stuff
*/

// Use SDL backend only if we are on X11
#ifdef X11

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <algorithm>
#include <cstdio>

#include <sadmutex.h>


typedef unsigned int Uint32;
typedef bool SDL_bool;

#define SDL_TRUE 1
#define SDL_FALSE 0

struct SDL_MessageBoxColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;   
};

struct SDL_Rect
{
    int x;
    int y;
    int w;
    int h;
};

struct SDL_WindowData
{
    Window xwindow;
};

/**
 * \brief SDL_MessageBox flags. If supported will display warning icon, etc.
 */
typedef enum
{
    SDL_MESSAGEBOX_ERROR        = 0x00000010,   /**< error dialog */
    SDL_MESSAGEBOX_WARNING      = 0x00000020,   /**< warning dialog */
    SDL_MESSAGEBOX_INFORMATION  = 0x00000040    /**< informational dialog */
} SDL_MessageBoxFlags;

/**
 * \brief Flags for SDL_MessageBoxButtonData.
 */
typedef enum
{
    SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT = 0x00000001,  /**< Marks the default button when return is hit */
    SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT = 0x00000002   /**< Marks the default button when escape is hit */
} SDL_MessageBoxButtonFlags;

/**
 *  \brief Individual button data.
 */
typedef struct
{
    Uint32 flags;       /**< ::SDL_MessageBoxButtonFlags */
    int buttonid;       /**< User defined button id (value returned via SDL_ShowMessageBox) */
    const char * text;  /**< The UTF-8 button text */
} SDL_MessageBoxButtonData;

typedef enum
{
    SDL_MESSAGEBOX_COLOR_BACKGROUND = 0,
    SDL_MESSAGEBOX_COLOR_TEXT = 1,
    SDL_MESSAGEBOX_COLOR_BUTTON_BORDER = 2,
    SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND = 3,
    SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED = 4,
    SDL_MESSAGEBOX_COLOR_MAX = 5
} SDL_MessageBoxColorType;

/**
 * \brief A set of colors to use for message box dialogs
 */
typedef struct
{
    SDL_MessageBoxColor colors[SDL_MESSAGEBOX_COLOR_MAX];
} SDL_MessageBoxColorScheme;

/**
 *  \brief MessageBox structure containing title, text, window, etc.
 */
typedef struct
{
    Uint32 flags;                       /**< ::SDL_MessageBoxFlags */
    const char *title;                  /**< UTF-8 title */
    const char *message;                /**< UTF-8 message text */

    int numbuttons;
    const SDL_MessageBoxButtonData *buttons;

    const SDL_MessageBoxColorScheme *colorScheme;   /**< ::SDL_MessageBoxColorScheme, can be nullptr to use system settings */
} SDL_MessageBoxData;

#define MAX_BUTTONS             8       /* Maximum number of buttons supported */
#define MAX_TEXT_LINES          32      /* Maximum number of text lines supported */
#define MIN_BUTTON_WIDTH        64      /* Minimum button width */
#define MIN_DIALOG_WIDTH        200     /* Minimum dialog width */
#define MIN_DIALOG_HEIGHT       100     /* Minimum dialog height */

static const char g_MessageBoxFontLatin1[] = "-*-*-medium-r-normal--0-120-*-*-p-0-iso8859-1";
static const char g_MessageBoxFont[] = "-*-*-*-*-*-*-*-120-*-*-*-*-*-*";

static const SDL_MessageBoxColor g_default_colors[ SDL_MESSAGEBOX_COLOR_MAX ] = {
    { 56,  54,  53  }, /* SDL_MESSAGEBOX_COLOR_BACKGROUND, */
    { 209, 207, 205 }, /* SDL_MESSAGEBOX_COLOR_TEXT, */
    { 140, 135, 129 }, /* SDL_MESSAGEBOX_COLOR_BUTTON_BORDER, */
    { 105, 102, 99  }, /* SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND, */
    { 205, 202, 53  }, /* SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED, */
};

#define SDL_MAKE_RGB( _r, _g, _b )  ( ( ( Uint32 )( _r ) << 16 ) | \
                                      ( ( Uint32 )( _g ) << 8 ) |  \
                                      ( ( Uint32 )( _b ) ) )

typedef struct SDL_MessageBoxButtonDataX11 {
    int x, y;                           /* Text position */
    int length;                         /* Text length */
    int text_width;                     /* Text width */

    SDL_Rect rect;                      /* Rectangle for entire button */

    const SDL_MessageBoxButtonData *buttondata;   /* Button data from caller */
} SDL_MessageBoxButtonDataX11;

typedef struct TextLineData {
    int width;                          /* Width of this text line */
    int length;                         /* String length of this text line */
    const char *text;                   /* Text for this line */
} TextLineData;

typedef struct SDL_MessageBoxDataX11
{
    Display *display;
    int screen;
    Window window;
    long event_mask;
    Atom wm_protocols;
    Atom wm_delete_message;

    int dialog_width;                   /* Dialog box width. */
    int dialog_height;                  /* Dialog box height. */

    XFontSet font_set;                  /* for UTF-8 systems */
    XFontStruct *font_struct;           /* Latin1 (ASCII) fallback. */
    int xtext, ytext;                   /* Text position to start drawing at. */
    int numlines;                       /* Count of Text lines. */
    int text_height;                    /* Height for text lines. */
    TextLineData linedata[ MAX_TEXT_LINES ];

    int *pbuttonid;                     /* Pointer to user return buttonid value. */

    int button_press_index;             /* Index into buttondata/buttonpos for button which is pressed (or -1). */
    int mouse_over_index;               /* Index into buttondata/buttonpos for button mouse is over (or -1). */

    int numbuttons;                     /* Count of buttons. */
    const SDL_MessageBoxButtonData *buttondata;
    SDL_MessageBoxButtonDataX11 buttonpos[ MAX_BUTTONS ];

    Uint32 color[ SDL_MESSAGEBOX_COLOR_MAX ];

    const SDL_MessageBoxData *messageboxdata;
} SDL_MessageBoxDataX11;



/* Return width and height for a string. */
static void
GetTextWidthHeight( SDL_MessageBoxDataX11 *data, const char *str, int nbytes, int *pwidth, int *pheight )
{    
    XCharStruct text_structure;
    int font_direction, font_ascent, font_descent;
    XTextExtents( data->font_struct, str, nbytes,
                  &font_direction, &font_ascent, &font_descent,
                  &text_structure );
    *pwidth = text_structure.width;
    *pheight = text_structure.ascent + text_structure.descent;   
}

/* Return index of button if position x,y is contained therein. */
static int
GetHitButtonIndex( SDL_MessageBoxDataX11 *data, int x, int y )
{
    int i;
    int numbuttons = data->numbuttons;
    SDL_MessageBoxButtonDataX11 *buttonpos = data->buttonpos;

    for ( i = 0; i < numbuttons; i++ ) {
        SDL_Rect *rect = &buttonpos[ i ].rect;

        if ( ( x >= rect->x ) &&
             ( x <= ( rect->x + rect->w ) ) &&
             ( y >= rect->y ) &&
             ( y <= ( rect->y + rect->h ) ) ) {
            return i;
        }
    }

    return -1;
}

/* Initialize SDL_MessageBoxData structure and Display, etc. */
static int
X11_MessageBoxInit( SDL_MessageBoxDataX11 *data, const SDL_MessageBoxData * messageboxdata, int * pbuttonid )
{
    int i;
    int numbuttons = messageboxdata->numbuttons;
    const SDL_MessageBoxButtonData *buttondata = messageboxdata->buttons;
    const SDL_MessageBoxColor *colorhints;

    if ( numbuttons > MAX_BUTTONS ) {
        //printf("Too many buttons\n");
        return -1;
    }

    data->dialog_width = MIN_DIALOG_WIDTH;
    data->dialog_height = MIN_DIALOG_HEIGHT;
    data->messageboxdata = messageboxdata;
    data->buttondata = buttondata;
    data->numbuttons = numbuttons;
    data->pbuttonid = pbuttonid;

    data->display = XOpenDisplay( nullptr );
    if ( !data->display ) {
        //printf("No display\n");
        return -1;
    }

    data->font_struct = XLoadQueryFont( data->display, g_MessageBoxFontLatin1 );
    if ( data->font_struct == nullptr ) {
        data->font_struct = XLoadQueryFont( data->display, g_MessageBoxFont);
        if (data->font_struct == nullptr) {
            //printf("No such font\n");
            return -1;
        }
    }

    if ( messageboxdata->colorScheme ) {
        colorhints = messageboxdata->colorScheme->colors;
    } else {
        colorhints = g_default_colors;
    }

    /* Convert our SDL_MessageBoxColor r,g,b values to packed RGB format. */
    for ( i = 0; i < SDL_MESSAGEBOX_COLOR_MAX; i++ ) {
        data->color[ i ] = SDL_MAKE_RGB( colorhints[ i ].r, colorhints[ i ].g, colorhints[ i ].b );
    }

    return 0;
}

/* Calculate and initialize text and button locations. */
static int
X11_MessageBoxInitPositions( SDL_MessageBoxDataX11 *data )
{
    int i;
    int ybuttons;
    int text_width_max = 0;
    int button_text_height = 0;
    int button_width = MIN_BUTTON_WIDTH;
    const SDL_MessageBoxData *messageboxdata = data->messageboxdata;

    /* Go over text and break linefeeds into separate lines. */
    if ( messageboxdata->message && messageboxdata->message[ 0 ] ) {
        const char *text = messageboxdata->message;
        TextLineData *plinedata = data->linedata;

        for ( i = 0; i < MAX_TEXT_LINES; i++, plinedata++ ) {
            int height;
            char *lf = strchr( ( char * )text, '\n' );

            data->numlines++;

            /* Only grab length up to lf if it exists and isn't the last line. */
            plinedata->length = ( lf && ( i < MAX_TEXT_LINES - 1 ) ) ? ( lf - text ) : strlen( text );
            plinedata->text = text;

            GetTextWidthHeight( data, text, plinedata->length, &plinedata->width, &height );

            /* Text and widths are the largest we've ever seen. */
            data->text_height = std::max( data->text_height, height );
            text_width_max = std::max( text_width_max, plinedata->width );

            if (lf && (lf > text) && (lf[-1] == '\r')) {
                plinedata->length--;
            }

            text += plinedata->length + 1;

            /* Break if there are no more linefeeds. */
            if ( !lf )
                break;
        }

        /* Bump up the text height slightly. */
        data->text_height += 2;
    }

    /* Loop through all buttons and calculate the button widths and height. */
    for ( i = 0; i < data->numbuttons; i++ ) {
        int height;

        data->buttonpos[ i ].buttondata = &data->buttondata[ i ];
        data->buttonpos[ i ].length = strlen( data->buttondata[ i ].text );

        GetTextWidthHeight( data, data->buttondata[ i ].text, strlen( data->buttondata[ i ].text ),
                            &data->buttonpos[ i ].text_width, &height );

        button_width = std::max( button_width, data->buttonpos[ i ].text_width );
        button_text_height = std::max( button_text_height, height );
    }

    if ( data->numlines ) {
        /* x,y for this line of text. */
        data->xtext = data->text_height;
        data->ytext = data->text_height + data->text_height;

        /* Bump button y down to bottom of text. */
        ybuttons = 3 * data->ytext / 2 + ( data->numlines - 1 ) * data->text_height;

        /* Bump the dialog box width and height up if needed. */
        data->dialog_width = std::max( data->dialog_width, 2 * data->xtext + text_width_max );
        data->dialog_height = std::max( data->dialog_height, ybuttons );
    } else {
        /* Button y starts at height of button text. */
        ybuttons = button_text_height;
    }

    if ( data->numbuttons ) {
        int x, y;
        int width_of_buttons;
        int button_spacing = button_text_height;
        int button_height = 2 * button_text_height;

        /* Bump button width up a bit. */
        button_width += button_text_height;

        /* Get width of all buttons lined up. */
        width_of_buttons = data->numbuttons * button_width + ( data->numbuttons - 1 ) * button_spacing;

        /* Bump up dialog width and height if buttons are wider than text. */
        data->dialog_width = std::max( data->dialog_width, width_of_buttons + 2 * button_spacing );
        data->dialog_height = std::max( data->dialog_height, ybuttons + 2 * button_height );

        /* Location for first button. */
        x = ( data->dialog_width - width_of_buttons ) / 2;
        y = ybuttons + ( data->dialog_height - ybuttons - button_height ) / 2;

        for ( i = 0; i < data->numbuttons; i++ ) {
            /* Button coordinates. */
            data->buttonpos[ i ].rect.x = x;
            data->buttonpos[ i ].rect.y = y;
            data->buttonpos[ i ].rect.w = button_width;
            data->buttonpos[ i ].rect.h = button_height;

            /* Button text coordinates. */
            data->buttonpos[ i ].x = x + ( button_width - data->buttonpos[ i ].text_width ) / 2;
            data->buttonpos[ i ].y = y + ( button_height - button_text_height - 1 ) / 2 + button_text_height;

            /* Scoot over for next button. */
            x += button_width + button_spacing;
        }
    }

    return 0;
}

/* Free SDL_MessageBoxData data. */
static void
X11_MessageBoxShutdown( SDL_MessageBoxDataX11 *data )
{
    if ( data->font_set != nullptr ) {
        XFreeFontSet( data->display, data->font_set );
        data->font_set = nullptr;
    }

    if ( data->font_struct != nullptr ) {
        XFreeFont( data->display, data->font_struct );
        data->font_struct = nullptr;
    }

    if ( data->display ) {
        if ( data->window != None ) {
            XWithdrawWindow( data->display, data->window, data->screen );
            XDestroyWindow( data->display, data->window );
            data->window = None;
        }

        XCloseDisplay( data->display );
        data->display = nullptr;
    }
}

/* Create and set up our X11 dialog box window. */
static int
X11_MessageBoxCreateWindow( SDL_MessageBoxDataX11 *data )
{
    int x, y;
    XSizeHints *sizehints;
    XSetWindowAttributes wnd_attr;
    Display *display = data->display;
    SDL_WindowData *windowdata = nullptr;
    const SDL_MessageBoxData *messageboxdata = data->messageboxdata;

    data->screen = DefaultScreen( display );

    data->event_mask = ExposureMask |
                       ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask |
                       StructureNotifyMask | FocusChangeMask | PointerMotionMask;
    wnd_attr.event_mask = data->event_mask;

    data->window = XCreateWindow(
                       display, RootWindow(display, data->screen),
                       0, 0,
                       data->dialog_width, data->dialog_height,
                       0, CopyFromParent, InputOutput, CopyFromParent,
                       CWEventMask, &wnd_attr );
    if ( data->window == None ) {
        return -1;
    }

    if ( windowdata ) {
        /* http://tronche.com/gui/x/icccm/sec-4.html#WM_TRANSIENT_FOR */
        XSetTransientForHint( display, data->window, windowdata->xwindow );
    }

    XStoreName( display, data->window, messageboxdata->title );

    /* Allow the window to be deleted by the window manager */
    data->wm_protocols = XInternAtom( display, "WM_PROTOCOLS", False );
    data->wm_delete_message = XInternAtom( display, "WM_DELETE_WINDOW", False );
    XSetWMProtocols( display, data->window, &data->wm_delete_message, 1 );

    if ( windowdata ) {
        XWindowAttributes attrib;
        Window dummy;

        XGetWindowAttributes(display, windowdata->xwindow, &attrib);
        x = attrib.x + ( attrib.width - data->dialog_width ) / 2;
        y = attrib.y + ( attrib.height - data->dialog_height ) / 3 ;
        XTranslateCoordinates(display, windowdata->xwindow, RootWindow(display, data->screen), x, y, &x, &y, &dummy);
    } else {
        x = ( DisplayWidth( display, data->screen ) - data->dialog_width ) / 2;
        y = ( DisplayHeight( display, data->screen ) - data->dialog_height ) / 3 ;
    }
    XMoveWindow( display, data->window, x, y );

    sizehints = XAllocSizeHints();
    if ( sizehints ) {
        sizehints->flags = USPosition | USSize | PMaxSize | PMinSize;
        sizehints->x = x;
        sizehints->y = y;
        sizehints->width = data->dialog_width;
        sizehints->height = data->dialog_height;

        sizehints->min_width = sizehints->max_width = data->dialog_width;
        sizehints->min_height = sizehints->max_height = data->dialog_height;

        XSetWMNormalHints( display, data->window, sizehints );

        XFree( sizehints );
    }

    XMapRaised( display, data->window );
    return 0;
}

/* Draw our message box. */
static void
X11_MessageBoxDraw( SDL_MessageBoxDataX11 *data, GC ctx )
{
    int i;
    Window window = data->window;
    Display *display = data->display;

    XSetForeground( display, ctx, data->color[ SDL_MESSAGEBOX_COLOR_BACKGROUND ] );
    XFillRectangle( display, window, ctx, 0, 0, data->dialog_width, data->dialog_height );

    XSetForeground( display, ctx, data->color[ SDL_MESSAGEBOX_COLOR_TEXT ] );
    for ( i = 0; i < data->numlines; i++ ) {
        TextLineData *plinedata = &data->linedata[ i ];

    XDrawString( display, window, ctx,
                         data->xtext, data->ytext + i * data->text_height,
                         plinedata->text, plinedata->length );
    }

    for ( i = 0; i < data->numbuttons; i++ ) {
        SDL_MessageBoxButtonDataX11 *buttondatax11 = &data->buttonpos[ i ];
        const SDL_MessageBoxButtonData *buttondata = buttondatax11->buttondata;
        int border = ( buttondata->flags &  0x00000001 ) ? 2 : 0;
        int offset = ( ( data->mouse_over_index == i ) && ( data->button_press_index == data->mouse_over_index ) ) ? 1 : 0;

        XSetForeground( display, ctx, data->color[ SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND ] );
        XFillRectangle( display, window, ctx,
                        buttondatax11->rect.x - border, buttondatax11->rect.y - border,
                        buttondatax11->rect.w + 2 * border, buttondatax11->rect.h + 2 * border );

        XSetForeground( display, ctx, data->color[ SDL_MESSAGEBOX_COLOR_BUTTON_BORDER ] );
        XDrawRectangle( display, window, ctx,
                        buttondatax11->rect.x, buttondatax11->rect.y,
                        buttondatax11->rect.w, buttondatax11->rect.h );

        XSetForeground( display, ctx, ( data->mouse_over_index == i ) ?
                        data->color[ SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED ] :
                        data->color[ SDL_MESSAGEBOX_COLOR_TEXT ] );

        XDrawString( display, window, ctx,
                         buttondatax11->x + offset, buttondatax11->y + offset,
                         buttondata->text, buttondatax11->length );
    }
}

/* Loop and handle message box event messages until something kills it. */
static int
X11_MessageBoxLoop( SDL_MessageBoxDataX11 *data )
{
    GC ctx;
    XGCValues ctx_vals;
    bool close_dialog = false;
    bool has_focus = true;
    KeySym last_key_pressed = XK_VoidSymbol;
    unsigned long gcflags = GCForeground | GCBackground;

    memset(&ctx_vals, 0, sizeof(XGCValues));
    ctx_vals.foreground = data->color[ SDL_MESSAGEBOX_COLOR_BACKGROUND ];
    ctx_vals.background = data->color[ SDL_MESSAGEBOX_COLOR_BACKGROUND ];

    gcflags |= GCFont;
    ctx_vals.font = data->font_struct->fid;

    ctx = XCreateGC( data->display, data->window, gcflags, &ctx_vals );
    if ( ctx == None ) {
        return -1;
    }

    data->button_press_index = -1;  /* Reset what button is currently depressed. */
    data->mouse_over_index = -1;    /* Reset what button the mouse is over. */

    while( !close_dialog ) {
        XEvent e;
        SDL_bool draw = SDL_TRUE;

        XWindowEvent( data->display, data->window, data->event_mask, &e );

        /* If X11_XFilterEvent returns True, then some input method has filtered the
           event, and the client should discard the event. */
        if ( ( e.type != Expose ) && XFilterEvent( &e, None ) )
            continue;

        switch( e.type ) {
        case Expose:
            if ( e.xexpose.count > 0 ) {
                draw = false;
            }
            break;

        case FocusIn:
            /* Got focus. */
            has_focus = true;
            break;

        case FocusOut:
            /* lost focus. Reset button and mouse info. */
            has_focus = false;
            data->button_press_index = -1;
            data->mouse_over_index = -1;
            break;

        case MotionNotify:
            if ( has_focus ) {
                /* Mouse moved... */
                int previndex = data->mouse_over_index;
                data->mouse_over_index = GetHitButtonIndex( data, e.xbutton.x, e.xbutton.y );
                if (data->mouse_over_index == previndex) {
                    draw = false;
                }
            }
            break;

        case ClientMessage:
            if ( e.xclient.message_type == data->wm_protocols &&
                 e.xclient.format == 32 &&
                 e.xclient.data.l[ 0 ] == data->wm_delete_message ) {
                close_dialog = true;
            }
            break;

        case KeyPress:
            /* Store key press - we make sure in key release that we got both. */
            last_key_pressed = XLookupKeysym( &e.xkey, 0 );
            break;

        case KeyRelease: {
            Uint32 mask = 0;
            KeySym key = XLookupKeysym( &e.xkey, 0 );

            /* If this is a key release for something we didn't get the key down for, then bail. */
            if ( key != last_key_pressed )
                break;

            if ( key == XK_Escape )
                mask = 0x00000002;
            else if ( ( key == XK_Return ) || ( key == XK_KP_Enter ) )
                mask = 0x00000001;

            if ( mask ) {
                int i;

                /* Look for first button with this mask set, and return it if found. */
                for ( i = 0; i < data->numbuttons; i++ ) {
                    SDL_MessageBoxButtonDataX11 *buttondatax11 = &data->buttonpos[ i ];

                    if ( buttondatax11->buttondata->flags & mask ) {
                        *data->pbuttonid = buttondatax11->buttondata->buttonid;
                        close_dialog = SDL_TRUE;
                        break;
                    }
                }
            }
            break;
        }

        case ButtonPress:
            data->button_press_index = -1;
            if ( e.xbutton.button == Button1 ) {
                /* Find index of button they clicked on. */
                data->button_press_index = GetHitButtonIndex( data, e.xbutton.x, e.xbutton.y );
            }
            break;

        case ButtonRelease:
            /* If button is released over the same button that was clicked down on, then return it. */
            if ( ( e.xbutton.button == Button1 ) && ( data->button_press_index >= 0 ) ) {
                int button = GetHitButtonIndex( data, e.xbutton.x, e.xbutton.y );

                if ( data->button_press_index == button ) {
                    SDL_MessageBoxButtonDataX11 *buttondatax11 = &data->buttonpos[ button ];

                    *data->pbuttonid = buttondatax11->buttondata->buttonid;
                    close_dialog = true;
                }
            }
            data->button_press_index = -1;
            break;
        }

        if ( draw ) {
            /* Draw our dialog box. */
            X11_MessageBoxDraw( data, ctx );
        }
    }

    XFreeGC( data->display, ctx );
    return 0;
}

static bool XInitThreadsCalled = false;

static sad::Mutex  XInitThreadsCalledLock;

void SafeXInitThreads()
{
    if (!XInitThreadsCalled)
    {
        XInitThreadsCalledLock.lock();
        if (!XInitThreadsCalled)
        {
            XInitThreadsCalled = true;
            XInitThreads();
        }
        XInitThreadsCalledLock.unlock();
    }
}

static int
X11_ShowMessageBoxImpl(const SDL_MessageBoxData *messageboxdata, int *buttonid)
{
    int ret;
    SDL_MessageBoxDataX11 data;
#if SDL_SET_LOCALE
    char *origlocale;
#endif

    memset(&data, 0, sizeof(SDL_MessageBoxDataX11)) ;


#if SDL_SET_LOCALE
    origlocale = setlocale(LC_ALL, nullptr);
    if (origlocale != nullptr) {
        origlocale = SDL_strdup(origlocale);
        if (origlocale == nullptr) {
            return -1;
        }
        setlocale(LC_ALL, "");
    }
#endif

    /* This code could get called from multiple threads maybe? */
    SafeXInitThreads();

    /* Initialize the return buttonid value to -1 (for error or dialogbox closed). */
    *buttonid = -1;

    /* Init and display the message box. */
    ret = X11_MessageBoxInit( &data, messageboxdata, buttonid );
    if ( ret != -1 ) {
        ret = X11_MessageBoxInitPositions( &data );
        if ( ret != -1 ) {
            ret = X11_MessageBoxCreateWindow( &data );
            if ( ret != -1 ) {
                ret = X11_MessageBoxLoop( &data );
            } else {
        //printf("Failed to create window\n");
        }
        } else {
        //printf("Failed to init positions\n");
    }
    } else {
      //printf("Failed to init\n");
    }

    X11_MessageBoxShutdown( &data );

#if SDL_SET_LOCALE
    if (origlocale) {
        setlocale(LC_ALL, origlocale);
        free(origlocale);
    }
#endif

    return ret;
}

/* Display an x11 message box. */
int
X11_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid)
{
    return X11_ShowMessageBoxImpl(messageboxdata, buttonid);
}



static int
SDL_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid)
{
    int dummybutton;
    int retval = -1;
    SDL_bool relative_mode;
    int show_cursor_prev;

    if (!buttonid) {
        buttonid = &dummybutton;
    }

    return X11_ShowMessageBox(messageboxdata, buttonid);
}

static int
SDL_ShowSimpleMessageBox(Uint32 flags, const char *title, const char *message)
{
    SDL_MessageBoxData data;
    SDL_MessageBoxButtonData button;

    memset(&data, 0, sizeof(SDL_MessageBoxData));
    data.flags = flags;
    data.title = title;
    data.message = message;
    data.numbuttons = 1;
    data.buttons = &button;
    

    memset(&button, 0 , sizeof(SDL_MessageBoxButtonData));
    button.flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    button.flags |= SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
    button.text = "OK";

    return SDL_ShowMessageBox(&data, nullptr);
}

bool SDL_MessageBoxError(
  const char * title,
  const char * message
)
{
    return SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        title,
        message
    ) == 0;
}

bool SDL_MessageBoxInformation(
  const char * title,
  const char * message
)
{
    return SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_INFORMATION,
        title,
        message
    ) == 0;
}

bool SDL_MessageBoxWarning(
  const char * title,
  const char * message
)
{
    return SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_WARNING,
        title,
        message
    ) == 0;
}

#endif


// Use Win32 backend
#ifdef WIN32

#include <algorithm>
#include <cstdio>
#include <windows.h>

bool SDL_MessageBoxError(
  const char * title,
  const char * message
)
{
    return MessageBoxA(
        nullptr,
        title,
        message,
        MB_ICONERROR|MB_OK
    ) != 0;
}

bool SDL_MessageBoxInformation(
  const char * title,
  const char * message
)
{
    return MessageBoxA(
        nullptr,
        title,
        message,
        MB_ICONINFORMATION|MB_OK
    ) != 0;
}

bool SDL_MessageBoxWarning(
  const char * title,
  const char * message
)
{
    return MessageBoxA(
        nullptr,
        title,
        message,
        MB_ICONWARNING|MB_OK
    ) != 0;
}

#endif