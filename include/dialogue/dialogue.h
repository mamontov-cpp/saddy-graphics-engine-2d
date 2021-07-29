/*! \file dialogue/dialogue.h
    
    
    Describes a dialogue as sequence of phrases
 */
#pragma once
#include "phrase.h"

#include "../sadptrvector.h"

#include "../db/dbobject.h"

namespace sad
{

namespace dialogue
{

/*! Describes a dialogue, which can be used to describe sequences of items
 */
class Dialogue: public sad::db::Object
{
public:
    /*! Constructs an empty dialogue
     */
    Dialogue();
    /*! Copies phrases from dialogue
        \param[in] d dialogue
     */
    Dialogue(const sad::dialogue::Dialogue& d);
    /*! Copies phrases from dialogue
        \param[in] d dialogue
        \return *this
     */
    sad::dialogue::Dialogue& operator=(const sad::dialogue::Dialogue& d);
    /*! Could be inherited
     */
    virtual ~Dialogue() override;
    /*! Clears dialogue, removing all phrases
     */
    void clear();
    /*! Sets list of phrases
        \param[in] phrases a list of phrases
     */
    void setPhrases(const sad::Vector<sad::dialogue::Phrase>& phrases);
    /*! Returns phrases list
        \return phrases list
     */
    sad::Vector<sad::dialogue::Phrase> copyPhrases() const;
    /*! Adds phrase to a dialogue
        \param[in] phrase a new phrase
     */
    void addPhrase(const sad::dialogue::Phrase& phrase);
    /*! Removes phrase from a dialog
        \param[in] i a phrase index
     */
    void removePhrase(size_t i);
    /*! Returns a phrase by index
        \param[in] i index of phrase
        \return a phrase
     */
    sad::dialogue::Phrase getPhrase(size_t i) const;
    /*! Sets a phrase by index
        \param[in] i index of phrase
        \param[in] phrase a phrase
     */
    void setPhrase(size_t i, const sad::dialogue::Phrase& phrase);
    /*! Returns amount of phrases in dialogue
        \return amount of phrases
     */
    size_t phraseCount() const;
    /*! Sets phrases
        \param[in] phrases a new phrases list
     */
    void setPhrases(const sad::Vector<sad::dialogue::Phrase*>& phrases);
    /*! Returns phrases
        \return phrases for dialogue
     */
    const sad::Vector<sad::dialogue::Phrase*>& phrases() const;
    /*! Returns phrases
        \return phrases for dialogue
     */
    sad::Vector<sad::dialogue::Phrase*>& phrases();
    /*! Returns string "sad::dialogue::Dialogue"
        \return name, under which object will be serialized
     */
    virtual const sad::String& serializableName() const override;
    /*! Returns schema for all kinds of dialogue
        \return schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const override;
protected:
    /*! Copies phrases from another dialogue
        \param[in] d dialogue
     */
    void copy(const sad::dialogue::Dialogue& d);
    /*! A list of phrases
     */
    sad::PtrVector<sad::dialogue::Phrase> m_phrases;
};

}

}
