#include "dukpp-03/jscontrols.h"
#include "db/dbtypename.h"
#include <sstream>

sad::dukpp03::JSControls::JSControls() : m_enabled(true)
{
    
}


sad::dukpp03::JSControls::~JSControls()
{
    for(size_t i = 0; i < SAD_INPUT_EVENT_TYPE_COUNT; i++)
    {
        sad::Vector<sad::Pair<sad::String, sad::dukpp03::JSHandler*> >& vector = m_handlers[i];
        for(size_t j = 0; j < vector.size(); j++)
        {
            delete vector[j].p2();    
        }
        vector.clear();
    }
}

void sad::dukpp03::JSControls::addToRenderer(sad::Renderer* r)
{
    r->controls()->add(*sad::input::EventType::ET_Quit, new sad::dukpp03::JSControls::EventHandler<sad::input::QuitEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_Activate, new sad::dukpp03::JSControls::EventHandler<sad::input::ActivateEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_Deactivate, new sad::dukpp03::JSControls::EventHandler<sad::input::DeactivateEvent>(this));
    
    r->controls()->add(*sad::input::EventType::ET_MouseEnter, new sad::dukpp03::JSControls::EventHandler<sad::input::MouseEnterEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_MouseLeave, new sad::dukpp03::JSControls::EventHandler<sad::input::MouseLeaveEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_KeyPress, new sad::dukpp03::JSControls::EventHandler<sad::input::KeyPressEvent>(this));
    
    r->controls()->add(*sad::input::EventType::ET_KeyRelease, new sad::dukpp03::JSControls::EventHandler<sad::input::KeyReleaseEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_MouseMove, new sad::dukpp03::JSControls::EventHandler<sad::input::MouseMoveEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_MousePress, new sad::dukpp03::JSControls::EventHandler<sad::input::MousePressEvent>(this));
    
    r->controls()->add(*sad::input::EventType::ET_MouseRelease, new sad::dukpp03::JSControls::EventHandler<sad::input::MouseReleaseEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_MouseDoubleClick, new sad::dukpp03::JSControls::EventHandler<sad::input::MouseDoubleClickEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_MouseWheel, new sad::dukpp03::JSControls::EventHandler<sad::input::MouseWheelEvent>(this));
    r->controls()->add(*sad::input::EventType::ET_Resize, new sad::dukpp03::JSControls::EventHandler<sad::input::ResizeEvent>(this));
}

void sad::dukpp03::JSControls::enable()
{
    m_enabled = true;
}

void sad::dukpp03::JSControls::disable()
{
    m_enabled = false;    
}

bool sad::dukpp03::JSControls::enabled() const
{
    return m_enabled;
}

sad::String sad::dukpp03::JSControls::onEvent(int event_type, sad::dukpp03::Context* ctx, const sad::dukpp03::CompiledFunction& f)
{
    if (event_type >= 0 && event_type < SAD_INPUT_EVENT_TYPE_COUNT)
    {
        sad::dukpp03::JSHandler* h = new sad::dukpp03::JSHandler(ctx, f);
        std::ostringstream ostream; 
        ostream << static_cast<void *>(h);
        sad::String id = ostream.str();
        m_handlers[event_type] << sad::Pair<sad::String, sad::dukpp03::JSHandler*>(id, h);
        return id;
    }
    return "";
}


void sad::dukpp03::JSControls::unbind(const sad::String& event_handler_id)
{
    for(size_t i = 0; i < SAD_INPUT_EVENT_TYPE_COUNT; i++)
    {
        sad::Vector<sad::Pair<sad::String, sad::dukpp03::JSHandler*> >& vector = m_handlers[i];
        for(size_t j = 0; j < vector.size(); j++)
        {
            if (vector[j].p1() == event_handler_id)
            {
                vector.removeAt(static_cast<unsigned long>(j));
                --j;
            }
        }
    }
}

DECLARE_COMMON_TYPE(sad::dukpp03::JSControls)
