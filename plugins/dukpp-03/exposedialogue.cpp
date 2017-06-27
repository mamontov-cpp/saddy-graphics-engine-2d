#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"

#include <geometry2d.h>
#include <fuzzyequal.h>
#include <renderer.h>

#include <dialogue/dialogue.h>
#include <dialogue/phrase.h>

#include <cassert>

#define PERFORM_AND_ASSERT(X)   {bool b = ctx->eval(X); assert(b); }

static void exposeDialoguePhrase(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::dialogue::Phrase>("SadDialoguePhrase");

    c->addMethod("setActorName", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::setActorName));
    c->addMethod("actorName", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::actorName));

    c->addMethod("setActorPortrait", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::setActorPortrait));
    c->addMethod("actorPortrait", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::actorPortrait));

    c->addMethod("setPhrase", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::setPhrase));
    c->addMethod("phrase", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::phrase));

    c->addMethod("setDuration", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::setDuration));
    c->addMethod("duration", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::duration));

    c->addMethod("setViewHint", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::setViewHint));
    c->addMethod("viewHint", sad::dukpp03::bind_method::from(&sad::dialogue::Phrase::viewHint));

    c->setPrototypeFunction("SadDialoguePhrase");

    ctx->addClassBinding("sad::dialogue::Phrase", c);

    PERFORM_AND_ASSERT(
        "sad.dialogue.Phrase = SadDialoguePhrase;"
    );
}


static void exposeDialogueDialogue(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::dialogue::Dialogue>("SadDialogueDialogue");

    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::dialogue::Dialogue::MajorId), sad::dukpp03::setter::from(&sad::dialogue::Dialogue::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::dialogue::Dialogue::MinorId), sad::dukpp03::setter::from(&sad::dialogue::Dialogue::MinorId));

    void (sad::dialogue::Dialogue::*set_phrases)(const sad::Vector<sad::dialogue::Phrase>& phrases) = &sad::dialogue::Dialogue::setPhrases;
    c->addMethod("setPhrases", sad::dukpp03::bind_method::from(set_phrases));
    c->addMethod("phrases", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::copyPhrases));
    c->addMethod("copyPhrases", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::copyPhrases));

    c->addMethod("addPhrase", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::addPhrase));
    c->addMethod("removePhrase", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::removePhrase));
    c->addMethod("getPhrase", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::getPhrase));
    c->addMethod("setPhrase", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::setPhrase));
    c->addMethod("phraseCount", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::phraseCount));
    c->addMethod("serializableName", sad::dukpp03::bind_method::from(&sad::dialogue::Dialogue::serializableName));


    c->setPrototypeFunction("SadDialogueDialogue");

    ctx->addClassBinding("sad::dialogue::Dialogue", c);

    PERFORM_AND_ASSERT(
        "sad.dialogue.Dialogue = SadDialogueDialogue;"
    );
}


void sad::dukpp03::exposeDialogue(sad::dukpp03::Context* ctx)
{
    PERFORM_AND_ASSERT(
        "sad.dialogue = {};"
    );

    exposeDialoguePhrase(ctx);
    exposeDialogueDialogue(ctx);
}
