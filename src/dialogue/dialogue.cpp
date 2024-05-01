#include "dialogue/dialogue.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

#include "sadmutex.h"
#include "opticksupport.h"

sad::dialogue::Dialogue::Dialogue()
{
    PROFILER_EVENT;

}

sad::dialogue::Dialogue::Dialogue(const sad::dialogue::Dialogue& d)
{
    PROFILER_EVENT;
    copy(d);
}


sad::dialogue::Dialogue& sad::dialogue::Dialogue::operator=(const sad::dialogue::Dialogue& d)
{
    PROFILER_EVENT;
    copy(d);
    return *this;
}

sad::dialogue::Dialogue::~Dialogue()
{
    PROFILER_EVENT;

}

void sad::dialogue::Dialogue::setPhrases(const sad::Vector<sad::dialogue::Phrase>& phrases)
{
    PROFILER_EVENT;
    clear();
    for(size_t i = 0; i < phrases.size(); ++i)
    {
        m_phrases.push_back(new sad::dialogue::Phrase(phrases[i]));
    }
}


sad::Vector<sad::dialogue::Phrase> sad::dialogue::Dialogue::copyPhrases() const
{
    PROFILER_EVENT;
    sad::Vector<sad::dialogue::Phrase> result;
    for(size_t i = 0; i < m_phrases.size(); ++i)
    {
        result.push_back(*(m_phrases[i]));
    }
    return result;
}

void sad::dialogue::Dialogue::addPhrase(const sad::dialogue::Phrase& phrase)
{
    PROFILER_EVENT;
    m_phrases.push_back(new sad::dialogue::Phrase(phrase));
}

void sad::dialogue::Dialogue::removePhrase(size_t i)
{
    PROFILER_EVENT;
    if (i < m_phrases.size())
    {
        delete m_phrases[i];
        m_phrases.removeAt(i);
    }
}

sad::dialogue::Phrase sad::dialogue::Dialogue::getPhrase(size_t i) const
{
    PROFILER_EVENT;
    if (i < m_phrases.size())
    {
        return *(m_phrases[i]);
    }
    else
    {
        return sad::dialogue::Phrase();
    }
}

void sad::dialogue::Dialogue::setPhrase(size_t i, const sad::dialogue::Phrase& phrase)
{
    PROFILER_EVENT;
    if (i < m_phrases.size())
    {
        *(m_phrases[i]) = phrase;
    }
}

size_t sad::dialogue::Dialogue::phraseCount() const
{
    PROFILER_EVENT;
    return m_phrases.size();
}

void sad::dialogue::Dialogue::setPhrases(const sad::Vector<sad::dialogue::Phrase*>& phrases)
{
    PROFILER_EVENT;
    clear();
    for(size_t i = 0; i < phrases.size(); ++i)
    {
        m_phrases.push_back(phrases[i]);
    }
}

const sad::Vector<sad::dialogue::Phrase*>& sad::dialogue::Dialogue::phrases() const
{
    PROFILER_EVENT;
    return m_phrases;
}

sad::Vector<sad::dialogue::Phrase*>& sad::dialogue::Dialogue::phrases()
{
    PROFILER_EVENT;
    return m_phrases;
}

static sad::String SadDialogDialogueName = "sad::dialogue::Dialogue";

const sad::String& sad::dialogue::Dialogue::serializableName() const
{
    PROFILER_EVENT;
    return SadDialogDialogueName;
}

static sad::db::schema::Schema* SadDialogueDialogueSchema = nullptr;

static sad::Mutex SadDialogueDialogueSchemaInit;
sad::db::schema::Schema* sad::dialogue::Dialogue::basicSchema()
{
    PROFILER_EVENT;
    if (SadDialogueDialogueSchema == nullptr)
    {
        SadDialogueDialogueSchemaInit.lock();
        if (SadDialogueDialogueSchema == nullptr)
        {
            SadDialogueDialogueSchema = new sad::db::schema::Schema();
            SadDialogueDialogueSchema->addParent(sad::db::Object::basicSchema());

            sad::Vector<sad::dialogue::Phrase*>& (sad::dialogue::Dialogue::*f)() = &sad::dialogue::Dialogue::phrases;
            void (sad::dialogue::Dialogue::*set_phrases)(const  sad::Vector<sad::dialogue::Phrase*>&) = &sad::dialogue::Dialogue::setPhrases;
            SadDialogueDialogueSchema->add(
                "phrases", 
                new sad::db::MethodPair<sad::dialogue::Dialogue, sad::Vector<sad::dialogue::Phrase*> >(
                    f,
                    set_phrases
                )
            );

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(SadDialogueDialogueSchema);
        }
        SadDialogueDialogueSchemaInit.unlock();
    }
    return SadDialogueDialogueSchema;
}

sad::db::schema::Schema* sad::dialogue::Dialogue::schema() const
{
    PROFILER_EVENT;
    return sad::dialogue::Dialogue::basicSchema();
}

void sad::dialogue::Dialogue::clear()
{
    PROFILER_EVENT;
    // Clear old
    for(size_t i = 0; i < m_phrases.size(); ++i)
    {
        delete m_phrases[i];
    }
    m_phrases.clear();
}

void sad::dialogue::Dialogue::copy(const sad::dialogue::Dialogue& d)
{
    PROFILER_EVENT;
    clear();
    for(size_t i = 0; i < d.m_phrases.size(); ++i)
    {
        m_phrases.push_back(new sad::dialogue::Phrase(*(d.m_phrases[i])));
    }
}
