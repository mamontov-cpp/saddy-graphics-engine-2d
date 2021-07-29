#include "dialogue/dialogue.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

#include "sadmutex.h"

sad::dialogue::Dialogue::Dialogue()
{

}

sad::dialogue::Dialogue::Dialogue(const sad::dialogue::Dialogue& d)
{
    copy(d);
}


sad::dialogue::Dialogue& sad::dialogue::Dialogue::operator=(const sad::dialogue::Dialogue& d)
{
    copy(d);
    return *this;
}

sad::dialogue::Dialogue::~Dialogue()
{

}

void sad::dialogue::Dialogue::setPhrases(const sad::Vector<sad::dialogue::Phrase>& phrases)
{
    clear();
    for(size_t i = 0; i < phrases.size(); ++i)
    {
        m_phrases.push_back(new sad::dialogue::Phrase(phrases[i]));
    }
}


sad::Vector<sad::dialogue::Phrase> sad::dialogue::Dialogue::copyPhrases() const
{
    sad::Vector<sad::dialogue::Phrase> result;
    for(size_t i = 0; i < m_phrases.size(); ++i)
    {
        result.push_back(*(m_phrases[i]));
    }
    return result;
}

void sad::dialogue::Dialogue::addPhrase(const sad::dialogue::Phrase& phrase)
{
    m_phrases.push_back(new sad::dialogue::Phrase(phrase));
}

void sad::dialogue::Dialogue::removePhrase(size_t i)
{
    if (i < m_phrases.size())
    {
        delete m_phrases[i];
        m_phrases.removeAt(i);
    }
}

sad::dialogue::Phrase sad::dialogue::Dialogue::getPhrase(size_t i) const
{
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
    if (i < m_phrases.size())
    {
        *(m_phrases[i]) = phrase;
    }
}

size_t sad::dialogue::Dialogue::phraseCount() const
{
    return m_phrases.size();
}

void sad::dialogue::Dialogue::setPhrases(const sad::Vector<sad::dialogue::Phrase*>& phrases)
{
    clear();
    for(size_t i = 0; i < phrases.size(); ++i)
    {
        m_phrases.push_back(phrases[i]);
    }
}

const sad::Vector<sad::dialogue::Phrase*>& sad::dialogue::Dialogue::phrases() const
{
    return m_phrases;
}

sad::Vector<sad::dialogue::Phrase*>& sad::dialogue::Dialogue::phrases()
{
    return m_phrases;
}

static sad::String SadDialogDialogueName = "sad::dialogue::Dialogue";

const sad::String& sad::dialogue::Dialogue::serializableName() const
{
    return SadDialogDialogueName;
}

static sad::db::schema::Schema* SadDialogueDialogueSchema = nullptr;

static sad::Mutex SadDialogueDialogueSchemaInit;
sad::db::schema::Schema* sad::dialogue::Dialogue::basicSchema()
{
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
    return sad::dialogue::Dialogue::basicSchema();
}

void sad::dialogue::Dialogue::clear()
{
    // Clear old
    for(size_t i = 0; i < m_phrases.size(); ++i)
    {
        delete m_phrases[i];
    }
    m_phrases.clear();
}

void sad::dialogue::Dialogue::copy(const sad::dialogue::Dialogue& d)
{
    clear();
    for(size_t i = 0; i < d.m_phrases.size(); ++i)
    {
        m_phrases.push_back(new sad::dialogue::Phrase(*(d.m_phrases[i])));
    }
}
