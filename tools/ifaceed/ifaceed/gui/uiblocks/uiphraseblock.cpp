#include <new>
#include <cassert>
#include "uiphraseblock.h"
#include <QListWidget>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

gui::uiblocks::UIPhraseBlock::UIPhraseBlock() : lstPhrases(nullptr),
dsbPhraseDuration(nullptr),
txtPhraseViewHint(nullptr),
txtPhraseActorName(nullptr),
txtPhraseActorPortrait(nullptr),
txtPhrasePhrase(nullptr),
btnPhraseAdd(nullptr),
btnPhraseRemove(nullptr),
btnPhraseMoveBack(nullptr),
btnPhraseMoveFront(nullptr)
{

}

void gui::uiblocks::UIPhraseBlock::init(QWidget* w)
{
    assert(w);
    this->lstPhrases = w->findChild<QListWidget*>("lstPhrases");
    assert(this->lstPhrases);
    this->dsbPhraseDuration = w->findChild<QDoubleSpinBox*>("dsbPhraseDuration");
    assert(this->dsbPhraseDuration);
    this->txtPhraseViewHint = w->findChild<QLineEdit*>("txtPhraseViewHint");
    assert(this->txtPhraseViewHint);
    this->txtPhraseActorName = w->findChild<QLineEdit*>("txtPhraseActorName");
    assert(this->txtPhraseActorName);
    this->txtPhraseActorPortrait = w->findChild<QLineEdit*>("txtPhraseActorPortrait");
    assert(this->txtPhraseActorPortrait);
    this->txtPhrasePhrase = w->findChild<QPlainTextEdit*>("txtPhrasePhrase");
    assert(this->txtPhrasePhrase);
    this->btnPhraseAdd = w->findChild<QPushButton*>("btnPhraseAdd");
    assert(this->btnPhraseAdd);
    this->btnPhraseRemove = w->findChild<QPushButton*>("btnPhraseRemove");
    assert(this->btnPhraseRemove);
    this->btnPhraseMoveBack = w->findChild<QPushButton*>("btnPhraseMoveBack");
    assert(this->btnPhraseMoveBack);
    this->btnPhraseMoveFront = w->findChild<QPushButton*>("btnPhraseMoveFront");
    assert(this->btnPhraseMoveFront);
}

gui::uiblocks::UIPhraseBlock::~UIPhraseBlock()
{

}
