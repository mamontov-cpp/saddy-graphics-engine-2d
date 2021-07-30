/*! \file filetarget.h
    

    Describes a target, which writes all formatted messages into a file.
 */
#pragma once
#include "logtarget.h"
#include "logmessage.h"
#include <cstdio>

namespace sad
{

namespace log
{

/*! A log, that writes formatted messages to a file. 
    You must call sad::log::FileTarget::open() before doing any output, because 
    OS could fail to open file. Otherwise, or when OS fails to open FILE no output
    is performed
 */
class FileTarget: public sad::log::Target
{
public:
    /*! Creates a new file with specified format.
        Format defined as follows
        {0} - current time
        {1} - message priority
        {2} - formatSubsystem() result, by default, subsystem + ": ", nothing if subsystem is not specified. For example: "commit():", ""
        {3} - formatFileLine() result, file and line through ', ', nothing if not specified
        {4} - message text
        \param[in] format format string 
        \param[in] min_priority Minimal priority for outputting. 
                               Messages with priority  value, 
                               lesser than minimum priority are discarded
     */
    FileTarget(const sad::String & format = "{0}: [{1}] {3}{2}{4}", int min_priority = 0);
    /*! Tries to open file for writing. Previous file, if opened with object
        of this target is closed and new file is opened, with destroying it's content
        \param[in] filename name of file
        \return true if opened successfully
     */
    bool open(const sad::String & filename);
    /*! Formats message and writes it to file
        \param[in] message received message
     */
    virtual void receive(const sad::log::Message & message) override;
    /*! Set minimal priority, which can be output
        \param[in] priority level message
     */ 
    inline void setMinPriority(int priority) { m_min_priority = priority; }
    /*! Closes a file if opened it before
     */
    virtual ~FileTarget() override;
protected:
    FILE * m_file;         //!< Inner file handle
    /*! Minimum priority of output message. 
        Messages of bigger priority will not be printed
     */
    int    m_min_priority; 
    sad::String m_format;  //!< Format for outputting the message
    /*! Formats a subsystem part, by default adds ": "
        \param[in] message a logged message
        \return format string
     */
    virtual std::string formatSubsystem(const sad::log::Message & message);
    /*! Formats file and line part, by default adds a space, or empty string
        \param[in] message a logged message
        \return format string
     */
    virtual std::string formatFileLine(const sad::log::Message & message);
    /*! Closes a to opened file, if can. Also resets it to null
     */
    virtual void close();
};

}

}
