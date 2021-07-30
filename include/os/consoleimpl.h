/*! \file consoleimpl.h
    

    Describes a cross-platform implementation for working with console interface
 */
#pragma once
#include "../log/color.h"

#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include <windows.h>
#endif

namespace sad
{

namespace os
{

/*! A console class, which implements cross-platform support for low-level console IO
 */
class ConsoleImpl
{
public:
    /*! Connects to console and initializes support for console
     */
    ConsoleImpl();
    /*! Disconnects from console and restores default color scheme
     */
    virtual ~ConsoleImpl();
    /*! Used to allocate console on Windows
     */
    virtual void createConsole() ;
    /*! Sets a color mode for console
     */
    virtual void setColorMode(sad::log::Color foreground, sad::log::Color background) ; 
    /*! Used to restore default color mode in console
     */
    virtual void clearColorMode();
    /*! Outputs a text, using color, set previously
     */
    virtual void print(const char * text) ;

#ifdef WIN32
private:
    WORD m_oldattributes;
    HANDLE m_console;
    /*! Initializes console information
     */
    void initConsole();
#endif

};

}

}
