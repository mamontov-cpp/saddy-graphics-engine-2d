#include "dialogue/phrase.h"


sad::dialogue::Phrase::Phrase()
: m_duration(0)
{

}

sad::dialogue::Phrase::~Phrase()
{

}

void sad::dialogue::Phrase::setActorName(const sad::String& name)
{
    this->m_actor_name = name;
}

void sad::dialogue::Phrase::setActorPortrait(const sad::String& portrait)
{
    this->m_actor_portrait = portrait;
}

void sad::dialogue::Phrase::setPhrase(const sad::String& phrase)
{
    this->m_phrase = phrase;
}

void sad::dialogue::Phrase::setDuration(double duration)
{
    this->m_duration  = duration;
}

void sad::dialogue::Phrase::setViewHint(const sad::String& view_hint)
{
    this->m_view_hint = view_hint;
}
 
const sad::String& sad::dialogue::Phrase::actorName() const
{
    return this->m_actor_name;
}

const sad::String& sad::dialogue::Phrase::actorPortrait() const
{
    return this->m_actor_portrait;
}

const sad::String& sad::dialogue::Phrase::phrase() const
{
    return this->m_phrase;
}

double sad::dialogue::Phrase::duration() const
{
    return this->m_duration;
}

const sad::String& sad::dialogue::Phrase::viewHint() const
{
    return this->m_view_hint;
}
