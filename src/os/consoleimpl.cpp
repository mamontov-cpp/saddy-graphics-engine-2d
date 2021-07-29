#include <os/consoleimpl.h>
#include <cstdio>

sad::os::ConsoleImpl::ConsoleImpl()
{
#ifdef WIN32
    initConsole();
#endif	
}

sad::os::ConsoleImpl::~ConsoleImpl()
{
    this->clearColorMode();
}

void sad::os::ConsoleImpl::createConsole()
{
#ifdef  WIN32
    if (AllocConsole())
    {
        freopen("CONIN$", "r",stdin); //-V530
        freopen("CONOUT$", "w",stdout); //-V530
        freopen("CONOUT$", "w",stderr); //-V530
        initConsole();
    }
#endif	
}

#ifdef WIN32

#ifdef _MSC_VER
    #pragma warning(disable: 4838)
#endif

static WORD fg[] = {
-1, 
FOREGROUND_RED | FOREGROUND_INTENSITY,
FOREGROUND_GREEN | FOREGROUND_INTENSITY,
FOREGROUND_BLUE | FOREGROUND_INTENSITY,
FOREGROUND_RED ,
FOREGROUND_GREEN ,
FOREGROUND_BLUE,
FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
FOREGROUND_INTENSITY,
0,
FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
FOREGROUND_RED | FOREGROUND_GREEN,
FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
FOREGROUND_RED | FOREGROUND_BLUE,
FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
FOREGROUND_GREEN | FOREGROUND_BLUE,
};


static WORD bg[] = {
-1, 
BACKGROUND_RED | BACKGROUND_INTENSITY,
BACKGROUND_GREEN | BACKGROUND_INTENSITY,
BACKGROUND_BLUE | BACKGROUND_INTENSITY,
BACKGROUND_RED ,
BACKGROUND_GREEN ,
BACKGROUND_BLUE,
BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
BACKGROUND_INTENSITY,
0,
BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
BACKGROUND_RED | BACKGROUND_GREEN,
BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
BACKGROUND_RED | BACKGROUND_BLUE,
BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
BACKGROUND_GREEN | BACKGROUND_BLUE,
};

#else
static const char * fg[] = { "01", "01;31", "01;32", "01;34", "31", "32", "34", "01;37", "37", "02;37", "30", "01;33", "33", "01;35", "35", "01;36", "36"};
static const char * bg[] = { ""    , "41"      , "42"      , "44"      , "41" , "42" , "44" , "47"      , "47" , "47"      ,  "40", "43"      , "43" , "45" ,      "45",  "46"      , "46;"};		
#endif


void sad::os::ConsoleImpl::setColorMode(sad::log::Color foreground, sad::log::Color background)
{
#ifdef WIN32
    if (m_console == INVALID_HANDLE_VALUE)
        return;
    this->clearColorMode();
    WORD fattrs = fg[(int)foreground];
    WORD battrs = bg[(int)background];
    if (fattrs == (WORD)-1 )
        fattrs = (m_oldattributes & FOREGROUND_RED)
               | (m_oldattributes & FOREGROUND_GREEN)
               | (m_oldattributes & FOREGROUND_BLUE)
               | (m_oldattributes & FOREGROUND_INTENSITY);
    if (battrs == (WORD)-1 )
        battrs = (m_oldattributes & BACKGROUND_RED)
               | (m_oldattributes & BACKGROUND_GREEN)
               | (m_oldattributes & BACKGROUND_BLUE)
               | (m_oldattributes & BACKGROUND_INTENSITY);
    SetConsoleTextAttribute(m_console, fattrs | battrs);
#else
    this->clearColorMode();
    printf("\033[%s%s%s%s", 
    fg[(int)foreground], 
    (background != sad::log::Color::NONE) ? ";" : "",
    bg[(int)background], "m");
#endif
}

void sad::os::ConsoleImpl::clearColorMode()
{
#ifdef WIN32
    if (m_console != INVALID_HANDLE_VALUE)
    {
        SetConsoleTextAttribute(m_console, m_oldattributes);
    }
#else
    puts("\033[00m");
#endif	
}


void sad::os::ConsoleImpl::print(const char * text)
{
#ifdef WIN32
    if (m_console != INVALID_HANDLE_VALUE)
    {
        DWORD p = 0;
        WriteFile(m_console, text, strlen(text), &p, nullptr);
        char c = '\n';
        WriteFile(m_console, &c, 1, &p, nullptr);		
    }
#else
    printf("%s", text);
#endif
}


#ifdef WIN32
void sad::os::ConsoleImpl::initConsole()
{
    m_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (m_console != INVALID_HANDLE_VALUE)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        GetConsoleScreenBufferInfo(m_console,&csbiInfo);
        m_oldattributes = csbiInfo.wAttributes;
    }
}
#endif
