/*! \file dialoguesbindings.h
    

    A bindings for dialogues are listed here
 */
#pragma once
#include "../dukqtcontext.h"
#include <sadstring.h>

#include <QVector>

#include <dialogue/dialogue.h>

#include "dialoguesphraseref.h"


namespace scripting
{

class Scripting;

namespace dialogues
{

/*! Lists all major ids for all dialogues in database
    \return a major ids for dialogues
 */
QVector<unsigned long long> list();

/*! Adds dialogue. Prefixed by underscore, since it will be mapped to _add function and add
    is reserved for call, which will take object, preprocess it's fields and call _add using fields of this object.
    \param[in] scripting a scripting part
    \param[in] name a name for way
    \param[in] phrases a list of phrases
 */ 
unsigned long long _add(
    scripting::Scripting* scripting, 
    sad::String name,
    QVector<sad::dialogue::Phrase> phrases
);

/*! Removes a dialogue
    \param[in] scripting a scripting part
    \param[in] d a dialogue to be removed
 */
void remove(scripting::Scripting* scripting, sad::dialogue::Dialogue* d);

/*! Add phrase to a dialogue
    \param[in] scripting a scripting part
    \param[in] d a dialogue to be removed
    \param[in] phrase a phrase part
 */
void addPhrase(
    scripting::Scripting* scripting, 
    sad::dialogue::Dialogue* d,
    sad::dialogue::Phrase phrase
);


/*! Removes phrases from a dialogue
    \param[in] scripting a scripting part
    \param[in] d a dialogue to be removed
    \param[in] pos position of phrase
 */
bool removePhrase(
    scripting::Scripting* scripting, 
    sad::dialogue::Dialogue* d,
    unsigned int pos
);

/*! Returns length of dialogues as count of phrases
    \param[in] scripting a scripting part
    \param[in] d a dialogue
 */
unsigned int length(scripting::Scripting* scripting, sad::dialogue::Dialogue* d);

/*! Fetches phrase reference from dialogue
    \param[in] scripting a scripting part
    \param[in] d a dialogue to be removed
    \param[in] pos a position
 */
dukpp03::Maybe<scripting::dialogues::PhraseRef*> phrase(scripting::Scripting* scripting, sad::dialogue::Dialogue* d, unsigned int pos);

/*! Initializes bindings for dialogues
    \param[in] s scripting object
    \param[in] e a global binding object
    \return created dialogues object
 */
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* e);

}

}
