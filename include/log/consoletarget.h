/*! \file consoletarget.h
    

    Describes a target, which writes a log messages to console
 */ 
#pragma once
#include "logtarget.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "logmessage.h"
#include "color.h"
#include "priority.h"

#include "../sadhash.h"
#include "../sadpair.h"

namespace sad
{

namespace os
{
class ConsoleImpl;
}

namespace log
{

/*! A special kind of target, which prints output to console.
    This kind of target, supports colored output and allocating console on Windows.
 */
class ConsoleTarget: public sad::log::Target
{
public:
    /*! Creates console target  with specified format.
        Format defined as follows
        {0} - current time
        {1} - message priority
        {2} - formatSubsystem() result, by default, subsystem + ": ", 
              nothing if subsystem is not specified. For example: "commit():", ""
        {3} - formatFileLine() result, file and line 
              through ', ', nothing if not specified
        {4} - message text
        \param[in] format format string 
        \param[in] min_priority Minimal priority for outputting. 
                               Messages with priority  value, 
                               lesser than minimal priority are discarded
        \param[in] colored  whether output should be colored
        \param[in] allocate_console forces allocating new console, 
                                    if current platform is Windows. The console
                                    will be allocated immediately after creation
     */
    ConsoleTarget(const sad::String & format = "{0}: [{1}] {3}{2}{4}", 
                  int min_priority = 0,  
                  bool colored =  true, 
                  bool allocate_console = false);
    /*! Formats message and outputs it to console
        \param[in] message received message
     */
    virtual void receive(const sad::log::Message & message) override;
    /*! Sets color for specific priority and colored output
       \param[in] p priority
       \param[in] c color
     */
    void setColorForPriorityAndColoredOutput(sad::log::Priority p, sad::log::Color c);
    /*! Restores default color mode for console, saving problems
      */
    ~ConsoleTarget() override;
protected:
    /*! An inner implementation of console, used to support low-level console 
        operations, like setting color and printing already formatted output
     */
    sad::os::ConsoleImpl * m_console; //!< A handle-like console
    /*! Maximum priority of output message. 
        Messages of bigger priority will not be printed
     */
    int         m_min_priority;  
    sad::String m_format; //!< Format for converting message to string
    /*! Color schema, as map from priority to pair of colors, where first is
        background color and second is foreground color
     */
    sad::Hash<sad::log::Priority, 
              sad::Pair<sad::log::Color, sad::log::Color>
             > m_coloring; 
    /*! Formats a subsystem message part. By default adds ": " to and of name of
        current subsystem, if it's specified
        \return format string
     */
    virtual std::string formatSubsystem(const sad::log::Message & message);
    /*! Formats file and line message part. 
        If file and line are specified,
        encloses them in brackets and places between them
        a sad::log::Message::fileline() result. Adds a trailing space after result.
        If file and line are not specified, returns empty string 
        \return format string
      */
    virtual std::string formatFileLine(const sad::log::Message & message);
    /*! Initializes default coloring schema
     */
    void createColoredOutput();
    /*! Initializes schema. The console output, initialized with
        this method won't be colored at all.
     */
    void createNormalOutput();
};

}

}
