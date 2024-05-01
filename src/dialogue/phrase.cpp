#include "dialogue/phrase.h"
#include "opticksupport.h"


sad::dialogue::Phrase::Phrase()
: m_duration(0)
{
    PROFILER_EVENT;

}

sad::dialogue::Phrase::~Phrase()
{
    PROFILER_EVENT;

}

void sad::dialogue::Phrase::setActorName(const sad::String& name)
{
    PROFILER_EVENT;
    this->m_actor_name = name;
}

void sad::dialogue::Phrase::setActorPortrait(const sad::String& portrait)
{
    PROFILER_EVENT;
    this->m_actor_portrait = portrait;
}

void sad::dialogue::Phrase::setPhrase(const sad::String& phrase)
{
    PROFILER_EVENT;
    this->m_phrase = phrase;
}

void sad::dialogue::Phrase::setDuration(double duration)
{
    PROFILER_EVENT;
    this->m_duration  = duration;
}

void sad::dialogue::Phrase::setViewHint(const sad::String& view_hint)
{
    PROFILER_EVENT;
    this->m_view_hint = view_hint;
}
 
const sad::String& sad::dialogue::Phrase::actorName() const
{
    PROFILER_EVENT;
    return this->m_actor_name;
}

const sad::String& sad::dialogue::Phrase::actorPortrait() const
{
    PROFILER_EVENT;
    return this->m_actor_portrait;
}

const sad::String& sad::dialogue::Phrase::phrase() const
{
    PROFILER_EVENT;
    return this->m_phrase;
}

double sad::dialogue::Phrase::duration() const
{
    PROFILER_EVENT;
    return this->m_duration;
}

const sad::String& sad::dialogue::Phrase::viewHint() const
{
    PROFILER_EVENT;
    return this->m_view_hint;
}
