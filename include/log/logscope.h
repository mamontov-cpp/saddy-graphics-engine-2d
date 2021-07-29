/*! \file	logscope.h
    

    A logging scope, which automatically pushes message on log, and pops it when destroyed
 */
#pragma once
#include "../sadstring.h"
#include "../3rdparty/format/format.h"

#include <memory>

namespace sad
{

namespace log
{

class Log;
/*! Scopes define a current scope for log
    It pushes subsystem name to it when are created, and  
    pops it, when destroyed.
 */
class Scope
{
public:
    /*! Pushes a new subsystem name into a log
        \param[in] c subsystem name
        \param[in] file source file name, where scope was created
        \param[in] line source code line, where scope was created
        \param[in] log a log, where changes will be registered (nullptr for global)
        \param[in] saddyinternal whether this is internal scope of library
     */
    Scope(
        const char * c, 
        const char * file = nullptr, 
        int line = 0, 
        sad::log::Log * log = nullptr,
        bool saddyinternal = false
    );
    /*! Pushes a new subsystem name into a log
        \param[in] c subsystem name
        \param[in] file source file name, where scope was created
        \param[in] line source code line, where scope was created
        \param[in] log a log, where changes will be registered (nullptr for global)
        \param[in] saddyinternal whether scope should be marked as internal
     */
    Scope(
        const sad::String & c, 
        const char * file = nullptr, 
        int line = 0, 
        sad::log::Log * log = nullptr,
        bool saddyinternal = false
    );
    /*! Pushes a new subsystem name into a log
        \param[in] c subsystem name
        \param[in] file source file name, where scope was created
        \param[in] line source code line, where scope was created
        \param[in] log a log, where changes will be registered (nullptr for global)
        \param[in] saddyinternal whether scope should be marked as internal
     */
    Scope(
        const std::string & c, 
        const char * file = nullptr, 
        int line  = 0, 
        sad::log::Log * log = nullptr,
        bool saddyinternal = false
    );
    /*! Pushes a new subsystem name into a log
        \param[in] c subsystem name
        \param[in] file source file name, where scope was created
        \param[in] line source code line, where scope was created
        \param[in] log a log, where changes will be registered (nullptr for global)
        \param[in] saddyinternal whether scope should be marked as internal
     */
    Scope(
        const fmt::internal::ArgInserter<char> & c, 
        const char * file = nullptr, 
        int line  = 0, 
        sad::log::Log * log = nullptr,
        bool saddyinternal = false
    );
    /*! Pops  current subsystem, specified on creation of scope from subsystem names
        stack of linked logger
     */
    ~Scope();
private:
    /*! A linked log, where all state changes will be registered
     */
    sad::log::Log * m_log;
    /*! Toggles, whether scope will be logged as internal messages of saddy
     */
    bool m_saddyinternal;
};

}

}

/*! Creates new scope with subsystem name X
 */
#define SL_SCOPE(X)  sad::log::Scope  _____1_____(X, __FILE__, __LINE__)
/*! Creates new scope in local log R with subsystem name X
 */
#define SL_LOCAL_SCOPE(X,R)  sad::log::Scope  _____1_____(X, __FILE__, __LINE__, (R).log())
/*! Creates a local internal scope in local log R with subsystem name X
 */
#define SL_INTERNAL_SCOPE(X, R) sad::log::Scope  _____1_____(X, __FILE__, __LINE__, (R).log(), true)

/*! Creates conditional internal scope, if pointer to renderer R is not null
 */
#define SL_COND_INTERNAL_SCOPE(X, R)             \
    std::unique_ptr<sad::log::Scope>  ____1____;   \
    if (R)                                       \
    {                                            \
        ____1____.reset(new sad::log::Scope(X, __FILE__, __LINE__, (R)->log(), true));  \
    }
