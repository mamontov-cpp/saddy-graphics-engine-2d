/*! \file jscontrols.h
    
    A controls, related to JS specified here
 */
#pragma once
#include "../input/controls.h"
#include "../refcountable.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../sadhash.h"
#include "../sadstring.h"
#include "jshandler.h"


namespace sad
{

namespace dukpp03
{

/*! A controls, which are exposed to JS, specified here
 */
class JSControls: public sad::RefCountable
{
public:
    /*! Makes new JS Controls
     */
    JSControls();
    /*! Destroys them
     */
    virtual ~JSControls();
    /*! Adds controls to renderer
        \param[in] r renderer
     */
    void addToRenderer(sad::Renderer* r);
    /*! Enables controls in all related renderers
     */
    void enable();
    /*! Disables controls in all related renderers
     */
    void disable();
    /*! Return whether controls are enabled in all renderer
        \return whether they are enabled
     */
    bool enabled() const;
    /*! Bind controls on event
        \param[in] event_type an event type data
        \param[in] ctx context
        \param[in] f a function
        \return handler identifier
     */
    sad::String onEvent(int event_type, sad::dukpp03::Context* ctx, const sad::dukpp03::CompiledFunction& f);
    /*! Unbinds controls from event
        \param[in] event_handler_id a handler id       
     */
    void unbind(const sad::String& event_handler_id);

    /*! Invokes handlers with specified event
        \param[in] e events
     */
    template<typename _EventType>
    void invoke(const _EventType & e)
    {
        if (!m_enabled)
        {
            return;
        }
        const int type = static_cast<int>(sad::input::EnumValueForEventType<_EventType>::Type);
        const sad::Vector<sad::Pair<sad::String, sad::dukpp03::JSHandler*> >& handler_list = m_handlers[type];
        for(size_t i = 0; i < handler_list.size(); i++)
        {
            handler_list[i].p2()->call(e);
        }
    }
protected: 
/*! A handler for type, that will be added to renderer
 */
template<typename _EventType>
class EventHandler: public sad::input::AbstractHandlerForType<_EventType>
{
public:
    /*! A reference to controls is stored until object is freed
     */
    EventHandler(sad::dukpp03::JSControls* controls) : m_controls(controls)
    {
        m_controls->addRef();
    }
    /*! On destruction, handler destroys controls
     */
    virtual ~EventHandler()
    {
        m_controls->delRef();
    }
protected:
    /*! Invokes a callback. Used by sad::Input
        \param[in] e event type
     */
    virtual void invoke(const _EventType & e) override
    {
        m_controls->invoke(e);
    }
    /*! A controls, that will be invoked
     */
    sad::dukpp03::JSControls* m_controls;
};

/*! Whether JS controls are enabled
 */
bool m_enabled;
/*! A handlers per event
 */
sad::Vector<sad::Pair<sad::String, sad::dukpp03::JSHandler*> > m_handlers[SAD_INPUT_EVENT_TYPE_COUNT];
};

}

}
