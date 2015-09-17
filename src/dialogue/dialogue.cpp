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

static sad::db::schema::Schema* SadDialogueDialogueSchema = NULL;

static sad::Mutex SadDialogueDialogueSchemaInit;
sad::db::schema::Schema* sad::dialogue::Dialogue::basicSchema()
{
    if (SadDialogueDialogueSchema == NULL)
    {
        SadDialogueDialogueSchemaInit.lock();
        if (SadDialogueDialogueSchema == NULL)
        {
            SadDialogueDialogueSchema = new sad::db::schema::Schema();
            SadDialogueDialogueSchema->addParent(sad::db::Object::basicSchema());

            sad::Vector<sad::dialogue::Phrase*>& (sad::dialogue::Dialogue::*f)() = &sad::dialogue::Dialogue::phrases;
            SadDialogueDialogueSchema->add(
                "phrases", 
                new sad::db::MethodPair<sad::dialogue::Dialogue, sad::Vector<sad::dialogue::Phrase*> >(
                    f,
                    &sad::dialogue::Dialogue::setPhrases
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
