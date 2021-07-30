/*! \file sadqtmainloop.h
    
    Describes a main loop, that can be ran inside a qt event loop
 */
#pragma once
#include <mainloop.h>
#include <sadvector.h>
#include <sadpair.h>
#include <input/events.h>


namespace sad
{
    
namespace qt
{
    
class MainLoop: public sad::MainLoop
{
public:
    /*! Constructs new main loop
     */
    MainLoop();
    /*! Frees a main loop, removing attached dispatcher
     */
    virtual ~MainLoop() override;
    /*! Runs a main loop
        \param[in] once if once, loop will be run only once
     */
    virtual void run(bool once = false) override;
    /*! Returns nullptr, since accessing a dispatcher is disabled for Qt
        \return nullptr
     */
    virtual sad::os::SystemEventDispatcher *  dispatcher() override;
    /*! Submits new event into main loop
        \param[in] t event type
        \param[in] ev event
        \param[in] now whether event should be handled now
     */
    virtual void submitEvent(sad::input::EventType t, sad::input::AbstractEvent* ev, bool now = false);
protected:
    /*! Processes events for renderer
     */
    virtual void processEvents();
    /*! Does nothing, since Qt handles this stuff
     */
    virtual void initKeyboardInput() override;
    /*! Does nothing, since Qt handles this stuff
     */
    virtual void registerRenderer() override;
    /*! Does nothing, since Qt handles this stuff
     */
    virtual void unregisterRenderer() override;
    /*! An events, that should be processed in event loop
     */
    sad::Vector< sad::Pair<sad::input::EventType, sad::input::AbstractEvent*> > m_events;
};

}

}

