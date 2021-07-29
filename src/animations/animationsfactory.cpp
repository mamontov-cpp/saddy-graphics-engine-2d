#include "animations/animationsfactory.h"
#include "animations/animationsblinking.h"
#include "animations/animationscamerarotation.h"
#include "animations/animationscamerashaking.h"
#include "animations/animationscolor.h"
#include "animations/animationsfontlist.h"
#include "animations/animationsfontsize.h"
#include "animations/animationsoptionlist.h"
#include "animations/animationsresize.h"
#include "animations/animationsrotate.h"
#include "animations/animationsparallel.h"
#include "animations/animationssimplemovement.h"
#include "animations/animationssequential.h"
#include "animations/animationstyping.h"
#include "animations/animationswaymoving.h"
#include "animations/animationstexturecoordinatescontinuous.h"
#include "animations/animationstexturecoordinateslist.h"

// ======================== PUBLIC METHODS of sad::animations::Factory::AbstractDelegate ========================

sad::animations::Factory::AbstractDelegate::~AbstractDelegate()
{
    
}

// ======================== PUBLIC METHODS of sad::animations::Factory ========================

sad::animations::Factory::Factory()
{
    push<sad::animations::Blinking>("sad::animations::Blinking");
    push<sad::animations::CameraRotation>("sad::animations::CameraRotation");
    push<sad::animations::CameraShaking>("sad::animations::CameraShaking");
    push<sad::animations::Color>("sad::animations::Color");
    push<sad::animations::FontList>("sad::animations::FontList");
    push<sad::animations::FontSize>("sad::animations::FontSize");
    push<sad::animations::OptionList>("sad::animations::OptionList");
    push<sad::animations::Parallel>("sad::animations::Parallel");
    push<sad::animations::Resize>("sad::animations::Resize");
    push<sad::animations::Rotate>("sad::animations::Rotate");
    push<sad::animations::SimpleMovement>("sad::animations::SimpleMovement");
    push<sad::animations::Sequential>("sad::animations::Sequential");
    push<sad::animations::TextureCoordinatesContinuous>("sad::animations::TextureCoordinatesContinuous");
    push<sad::animations::TextureCoordinatesList>("sad::animations::TextureCoordinatesList");
    push<sad::animations::Typing>("sad::animations::Typing");
    push<sad::animations::WayMoving>("sad::animations::WayMoving");        
}

sad::animations::Factory::Factory(const sad::animations::Factory& f)
{
    copy(f);
}

sad::animations::Factory& sad::animations::Factory::operator=(const sad::animations::Factory& f)
{
    clear();
    copy(f);
    return *this;
}

sad::animations::Factory::~Factory()
{
    
}

sad::animations::Animation* sad::animations::Factory::create(const sad::String& s)
{
    if (m_delegates.contains(s))
    {
        return m_delegates[s]->create();
    }
    return nullptr;
}

void sad::animations::Factory::clear()
{
    for(sad::PtrHash<sad::String, sad::animations::Factory::AbstractDelegate>::const_iterator it = m_delegates.const_begin();
        it != m_delegates.const_end();
        ++it)
    {
        delete it.value();
    }
    m_delegates.clear();
}

sad::Vector<sad::String> sad::animations::Factory::registeredClasses() const
{
    sad::Vector<sad::String> result;
    for(sad::PtrHash<sad::String, sad::animations::Factory::AbstractDelegate>::const_iterator it = m_delegates.const_begin();
        it != m_delegates.const_end();
        ++it)
    {
        result << it.key();
    }
    return result;
}

// ======================== PROTECTED METHODS of sad::animations::Factory  ========================

void sad::animations::Factory::copy(const sad::animations::Factory& f)
{
    for(sad::PtrHash<sad::String, sad::animations::Factory::AbstractDelegate>::const_iterator it = f.m_delegates.const_begin();
        it != f.m_delegates.const_end();
        ++it)
    {
        m_delegates.insert(it.key(), it.value()->clone());
    }
}

