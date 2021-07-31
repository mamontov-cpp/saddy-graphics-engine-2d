/*! \file saddythread.h
    

    Defines a thread object, where renderer of Saddy is being run
 */
#pragma once
#include <QThread>

#include <maybe.h>
#include <sadstring.h>

namespace core
{
class Editor;

/*! Defines a thread object, where renderer of Saddy is being run
 */
class SaddyThread: public QThread 
{
Q_OBJECT
friend class Editor;
public:
    /*! Constructs a children rendering thread. Editor is needed to
        send a signal to run Qt event loop
        \param[in] editor editor to run
     */
    SaddyThread(core::Editor * editor);
    /*! Runs a thread, setting up a renderer,
        awaking main thread and running renderer's
        event loop
     */
    virtual void run() override;
private:
    /*! Editor to work with
     */
    core::Editor * m_editor;
    /*! Tries to parse width of height from result of CLI parser
        \param[in] maybe_value an input from parser
        \param[in] default_value a value, returned from input in case that parse value
        \param[in] min_value a minimal value for data
        \param[in] max_value a maximal value
     */
    static unsigned int tryParseWidthOrHeight(
        const sad::Maybe<sad::String> & maybe_value,
        unsigned int default_value,
        unsigned int min_value,
        unsigned int max_value
    );
};

}
