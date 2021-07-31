#include <new>
#include <cassert>
#include "uidialogueblock.h"
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>

gui::uiblocks::UIDialogueBlock::UIDialogueBlock() : lstDialogues(nullptr),
txtDialogueName(nullptr),
btnDialogueAdd(nullptr),
btnDialogueRemove(nullptr)
{

}

void gui::uiblocks::UIDialogueBlock::init(QWidget* w)
{
    assert(w);
    this->lstDialogues = w->findChild<QListWidget*>("lstDialogues");
    assert(this->lstDialogues);
    this->txtDialogueName = w->findChild<QLineEdit*>("txtDialogueName");
    assert(this->txtDialogueName);
    this->btnDialogueAdd = w->findChild<QPushButton*>("btnDialogueAdd");
    assert(this->btnDialogueAdd);
    this->btnDialogueRemove = w->findChild<QPushButton*>("btnDialogueRemove");
    assert(this->btnDialogueRemove);
}

gui::uiblocks::UIDialogueBlock::~UIDialogueBlock()
{

}
