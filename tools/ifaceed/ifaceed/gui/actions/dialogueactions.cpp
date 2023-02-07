#include <QPlainTextEdit>
#include <QDoubleSpinBox>

#include "dialogueactions.h"

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

#include "../../history/dialogues/dialoguesnew.h"
#include "../../history/dialogues/dialoguesremove.h"
#include "../../history/dialogues/dialogueschangename.h"
#include "../../history/dialogues/dialoguesphrasenew.h"
#include "../../history/dialogues/dialoguesphraseremove.h"
#include "../../history/dialogues/dialoguesphraseswap.h"
#include "../../history/dialogues/dialoguesphrasechangeduration.h"
#include "../../history/dialogues/dialoguesphrasechangephrase.h"
#include "../../history/dialogues/dialoguesphrasechangelineeditbasedproperty.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uidialogueblock.h"
#include "../uiblocks/uiphraseblock.h"


Q_DECLARE_METATYPE(sad::dialogue::Dialogue*) //-V566

// ========================== PUBLIC METHODS ==========================

gui::actions::DialogueActions::DialogueActions(QObject* parent) : QObject(parent)
{
    
}

gui::actions::DialogueActions::~DialogueActions()
{
    
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::viewDialogue(sad::dialogue::Dialogue* d)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

    invoke_blocked(blk->txtDialogueName, &QLineEdit::setText, STD2QSTRING(d->objectName().c_str()));
    pblk->lstPhrases->clear();
    for(size_t i = 0; i < d->phrases().size(); i++)
    {
        pblk->lstPhrases->addItem(this->nameForPhrase(*(d->phrases()[i])));
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::viewPhrase(sad::dialogue::Phrase* p)
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

    m_editor->emitClosure( blocked_bind(
            pblk->txtPhraseActorName,
            &QLineEdit::setText,
            STD2QSTRING(p->actorName().c_str())
    ));
    m_editor->emitClosure( blocked_bind(
        pblk->txtPhraseActorPortrait,
        &QLineEdit::setText,
        STD2QSTRING(p->actorPortrait().c_str())
    ));
    m_editor->emitClosure( blocked_bind(
        pblk->txtPhrasePhrase,
        &QPlainTextEdit::setPlainText,
        STD2QSTRING(p->phrase().c_str())
    ));
    m_editor->emitClosure( blocked_bind(
        pblk->dsbPhraseDuration,
        &QDoubleSpinBox::setValue,
        p->duration()
    ));
    m_editor->emitClosure( blocked_bind(
        pblk->txtPhraseViewHint,
        &QLineEdit::setText,
        STD2QSTRING(p->viewHint().c_str())
    ));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::changePhraseText(
    sad::dialogue::Dialogue* d, 
    int pos, 
    const sad::String& new_value, 
    bool from_editor
)
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

    sad::String old_value = d->phrases()[pos]->phrase();
    if (old_value != new_value)
    {
        d->phrases()[pos]->setPhrase(new_value);
        if (m_editor->shared()->selectedDialogue() == d)
        {
            pblk->lstPhrases->item(pos)->setText(this->nameForPhrase(*(d->phrases()[pos])));
        }
        history::dialogues::PhraseChangePhrase* c = new history::dialogues::PhraseChangePhrase(d, pos, old_value, new_value);
        if (from_editor)
        {
            m_editor->history()->add(c);
        } 
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::changePhraseDuration(
    sad::dialogue::Dialogue* d, 
    int pos, 
    double new_value, 
    bool from_editor
)
{
    double old_value = d->phrases()[pos]->duration();
    if (sad::is_fuzzy_equal(old_value, new_value) == false)
    {
        history::dialogues::PhraseChangeDuration* c = new history::dialogues::PhraseChangeDuration(
            d, 
            pos, 
            old_value, 
            new_value
        );
        c->commitWithoutUpdatingUI(m_editor);
        if (from_editor)
        {
            m_editor->history()->add(c);
        } 
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::changePhraseActorName(
    sad::dialogue::Dialogue* d, 
    int pos, 
    const sad::String& new_value, 
    bool from_editor
)
{
    sad::String old_value = d->phrases()[pos]->actorName();
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 
    if (old_value != new_value)
    {
        history::Command* c = new history::dialogues::PhraseChangeLineEditBasedProperty(
            pblk->txtPhraseActorName,
            &sad::dialogue::Phrase::setActorName,
            true,
            d, 
            pos, 
            old_value, 
            new_value
        );
        if (from_editor)
        {
            c->commitWithoutUpdatingUI(m_editor);
        }
        else
        {
            c->commit(m_editor);
        }
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::changePhraseActorPortrait(
    sad::dialogue::Dialogue* d, 
    int pos, 
    const sad::String& new_value, 
    bool from_editor
)
{
    sad::String old_value = d->phrases()[pos]->actorPortrait();
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 
    if (old_value != new_value)
    {
        history::Command* c = new history::dialogues::PhraseChangeLineEditBasedProperty(
            pblk->txtPhraseActorPortrait,
            &sad::dialogue::Phrase::setActorPortrait,
            false,
            d, 
            pos, 
            old_value, 
            new_value
        );
        if (from_editor)
        {
            c->commitWithoutUpdatingUI(m_editor);
        }
        else
        {
            c->commit(m_editor);
        }
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::changePhraseViewHint(
    sad::dialogue::Dialogue* d, 
    int pos, 
    const sad::String& new_value, 
    bool from_editor
)
{
    sad::String old_value = d->phrases()[pos]->viewHint();
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 
    if (old_value != new_value)
    {
        history::Command* c = new history::dialogues::PhraseChangeLineEditBasedProperty(
            pblk->txtPhraseViewHint,
            &sad::dialogue::Phrase::setViewHint,
            false,
            d, 
            pos, 
            old_value, 
            new_value
        );
        if (from_editor)
        {
            c->commitWithoutUpdatingUI(m_editor);
        }
        else
        {
            c->commit(m_editor);
        }
        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

void gui::actions::DialogueActions::removeDialogueFromDatabase(
        sad::dialogue::Dialogue* d,
        bool from_editor,
        int row
)
{
    if (row == -1)
    {
        row = this->findDialogueInList(d);
    }
    history::dialogues::Remove* c = new history::dialogues::Remove(d, row);
    c->commit(m_editor);
    if (from_editor)
    {
        m_editor->history()->add(c);
    }
    else
    {
        m_editor->currentBatchCommand()->add(c);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::addDialogueToDialogueList(sad::dialogue::Dialogue* dialogue)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 
    
    blk->lstDialogues->addItem(this->viewableObjectName(dialogue));
    QVariant v;
    v.setValue(dialogue);
    blk->lstDialogues->item(blk->lstDialogues->count()-1)->setData(Qt::UserRole, v);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::removeLastDialogueFromDialogueList()
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 

    if (blk->lstDialogues->count() > 0)
    {
        QVariant v = blk->lstDialogues->item(blk->lstDialogues->count() - 1)->data(Qt::UserRole);
        sad::dialogue::Dialogue* w  = v.value<sad::dialogue::Dialogue*>();
        if (w == m_editor->shared()->selectedDialogue())
        {
            m_editor->shared()->setSelectedDialogue(nullptr);
        }
        delete blk->lstDialogues->takeItem(blk->lstDialogues->count() - 1);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::insertDialogueToDialogueList(sad::dialogue::Dialogue* s, int position)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 


    QListWidgetItem* i = new QListWidgetItem(this->viewableObjectName(s));
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    blk->lstDialogues->insertItem(position, i);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::removeDialogueFromDialogueList(int position)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 

    QVariant v = blk->lstDialogues->item(position)->data(Qt::UserRole);
    sad::dialogue::Dialogue* w  = v.value<sad::dialogue::Dialogue*>();
    if (w == m_editor->shared()->selectedDialogue())
    {
        m_editor->shared()->setSelectedDialogue(nullptr);
    }
    delete blk->lstDialogues->takeItem(position);
}

void gui::actions::DialogueActions::removeDialogueFromDialogueList(sad::dialogue::Dialogue* s)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 

    int pos = this->findDialogueInList(s);
    if (s == m_editor->shared()->selectedDialogue())
    {
        m_editor->shared()->setSelectedDialogue(nullptr);
    }
    if (pos >= 0)
    {
        delete blk->lstDialogues->takeItem(pos);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
int gui::actions::DialogueActions::findDialogueInList(sad::dialogue::Dialogue* s)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 

    return findInList(blk->lstDialogues, s);
}

void gui::actions::DialogueActions::updateDialogueName(sad::dialogue::Dialogue* s)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 

    int row = this->findDialogueInList(s);
    if (row != -1)
    {
        blk->lstDialogues->item(row)->setText(this->viewableObjectName(s));
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::removePhraseFromPhraseList(int row)
{
    gui::uiblocks::UIPhraseBlock* blk = m_editor->uiBlocks()->uiPhraseBlock(); 

    delete blk->lstPhrases->takeItem(row);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
QString gui::actions::DialogueActions::nameForPhrase(const sad::dialogue::Phrase& p) const
{
    sad::String s = p.phrase();
    if (s.length() > 3)
    {
        s = s.subString(0, 3);
        s += "...";
    }
    return QString("%1(%2)")
           .arg(STD2QSTRING(p.actorName().c_str()))
           .arg(STD2QSTRING(s.c_str()));
}

// ========================== PUBLIC SLOTS ==========================

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::addDialogue()
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 

    sad::dialogue::Dialogue* w = new sad::dialogue::Dialogue();
    w->setObjectName(Q2STDSTRING(blk->txtDialogueName->text()));
    sad::Renderer::ref()->database("")->table("dialogues")->add(w);
    history::dialogues::New* c = new history::dialogues::New(w);
    c->commit(m_editor);
    m_editor->history()->add(c);
    m_editor->shared()->setSelectedDialogue(w);
    blk->lstDialogues->setCurrentRow(blk->lstDialogues->count() - 1);
}

void gui::actions::DialogueActions::removeDialogue()
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 
    int row = blk->lstDialogues->currentRow();
    if (row > -1)
    {
        QVariant variant = blk->lstDialogues->item(row)->data(Qt::UserRole);
        sad::dialogue::Dialogue* w = variant.value<sad::dialogue::Dialogue*>();
        removeDialogueFromDatabase(w, true, row);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::addPhrase()
{
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (d)
    {
        gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

        sad::dialogue::Phrase p;
        p.setActorName(Q2STDSTRING(pblk->txtPhraseActorName->text()));
        p.setActorPortrait(Q2STDSTRING(pblk->txtPhraseActorPortrait->text()));
        p.setPhrase(Q2STDSTRING(pblk->txtPhrasePhrase->toPlainText()));
        p.setDuration(pblk->dsbPhraseDuration->value());
        p.setViewHint(Q2STDSTRING(pblk->txtPhraseViewHint->text()));
        history::dialogues::PhraseNew* c = new history::dialogues::PhraseNew(d, p);
        c->commit(m_editor);
        m_editor->history()->add(c);
        
        QListWidget* phrase_list = pblk->lstPhrases;
        bool b = phrase_list->blockSignals(true);
        phrase_list->setCurrentRow(phrase_list->count() - 1);
        phrase_list->blockSignals(b);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::removePhrase()
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

    int row = pblk->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row >= 0 && row < pblk->lstPhrases->count() && d)
    {
        history::dialogues::PhraseRemove* c = new history::dialogues::PhraseRemove(d, row);
        c->commit(m_editor);
        m_editor->history()->add(c);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::movePhraseBack()
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

    int row = pblk->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row > 0 && row < pblk->lstPhrases->count() && d)
    {
        history::dialogues::PhraseSwap* c = new history::dialogues::PhraseSwap(d, row, row - 1);
        c->commit(m_editor);
        m_editor->history()->add(c);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::DialogueActions::movePhraseFront()
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

    int row = pblk->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row >= 0 && row < pblk->lstPhrases->count() - 1 && d)
    {
        history::dialogues::PhraseSwap* c = new history::dialogues::PhraseSwap(d, row, row + 1);
        c->commit(m_editor);
        m_editor->history()->add(c);
    }
}

void gui::actions::DialogueActions::nameEdited(const QString& name)
{
    sad::String new_value = Q2STDSTRING(name);
    sad::dialogue::Dialogue* w = m_editor->shared()->selectedDialogue();
    if (w)
    {
        sad::String old_value =  w->objectName();
        if (new_value != old_value)
        {
            w->setObjectName(new_value);
            this->updateDialogueName(w);
            m_editor->history()->add(new history::dialogues::ChangeName(w, old_value, new_value));
        }
    }
}

void gui::actions::DialogueActions::dialogueChanged(int i)
{
    gui::uiblocks::UIDialogueBlock* blk = m_editor->uiBlocks()->uiDialogueBlock(); 
    
    if (i >= 0)
    {
        QListWidgetItem* item = blk->lstDialogues->item(i);
        if (item)
        {
            QVariant v = item->data(Qt::UserRole);
            sad::dialogue::Dialogue* w = v.value<sad::dialogue::Dialogue*>();
            m_editor->shared()->setSelectedDialogue(w);
            this->viewDialogue(w);
        }
        else
        {
            m_editor->shared()->setSelectedDialogue(nullptr);
        }
    }
    else
    {
        m_editor->shared()->setSelectedDialogue(nullptr);
    }
}


void gui::actions::DialogueActions::phraseChanged(int i)
{
    sad::dialogue::Dialogue* w =  m_editor->shared()->selectedDialogue();
    if (w)
    {
        if (i >= 0 && i < static_cast<int>(w->phrases().size()))
        {
            sad::dialogue::Phrase* p = w->phrases()[i];
            this->viewPhrase(p);
        }
    }
}

void gui::actions::DialogueActions::phraseTextChanged()
{
    gui::uiblocks::UIPhraseBlock* phrase_block = m_editor->uiBlocks()->uiPhraseBlock(); 
    
    const int row = phrase_block->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row >= 0 && row < phrase_block->lstPhrases->count() && d)
    {
        const sad::String new_value = Q2STDSTRING(phrase_block->txtPhrasePhrase->toPlainText());
        changePhraseText(d, row, new_value, true);
    }
}

void gui::actions::DialogueActions::durationChanged(double new_value)
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 
    
    int row = pblk->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row >= 0 && row < pblk->lstPhrases->count() && d)
    {
        changePhraseDuration(d, row, new_value, true);
    }
}

void gui::actions::DialogueActions::actorNameChanged(const QString& new_value)
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 
    
    int row = pblk->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row >= 0 && row < pblk->lstPhrases->count() && d)
    {
        sad::String nv = Q2STDSTRING(new_value);
        changePhraseActorName(d, row, nv, true);
    }
}

void gui::actions::DialogueActions::actorPortraitChanged(const QString& new_value)
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 

    int row = pblk->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row >= 0 && row < pblk->lstPhrases->count() && d)
    {
        sad::String nv = Q2STDSTRING(new_value);
        changePhraseActorPortrait(d, row, nv, true);
    }
}

void gui::actions::DialogueActions::viewHintChanged(const QString& new_value)
{
    gui::uiblocks::UIPhraseBlock* pblk = m_editor->uiBlocks()->uiPhraseBlock(); 
    
    int row = pblk->lstPhrases->currentRow();
    sad::dialogue::Dialogue* d = m_editor->shared()->selectedDialogue();
    if (row >= 0 && row < pblk->lstPhrases->count() && d)
    {		
        sad::String nv = Q2STDSTRING(new_value);
        changePhraseViewHint(d, row, nv, true);
    }
}
