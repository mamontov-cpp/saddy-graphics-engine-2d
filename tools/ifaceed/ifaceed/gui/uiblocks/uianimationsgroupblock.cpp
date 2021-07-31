#include <new>
#include <cassert>
#include "uianimationsgroupblock.h"
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>

gui::uiblocks::UIAnimationsGroupBlock::UIAnimationsGroupBlock() : lstAnimationsGroupInGroup(nullptr),
lstAnimationsGroupAllAnimations(nullptr),
btnAnimationsGroupRemoveFromList(nullptr),
btnAnimationsGroupAddToList(nullptr),
lstAnimationsGroup(nullptr),
txtAnimationsGroupName(nullptr),
cbAnimationsGroupLooped(nullptr),
btnAnimationsGroupAdd(nullptr),
btnAnimationsGroupRemove(nullptr),
btnAnimationsGroupStart(nullptr),
btnAnimationsGroupCancel(nullptr),
rbAnimationsGroupParallel(nullptr),
rbAnimationsGroupSequential(nullptr)
{

}

void gui::uiblocks::UIAnimationsGroupBlock::init(QWidget* w)
{
    assert(w);
    this->lstAnimationsGroupInGroup = w->findChild<QListWidget*>("lstAnimationsGroupInGroup");
    assert(this->lstAnimationsGroupInGroup);
    this->lstAnimationsGroupAllAnimations = w->findChild<QListWidget*>("lstAnimationsGroupAllAnimations");
    assert(this->lstAnimationsGroupAllAnimations);
    this->btnAnimationsGroupRemoveFromList = w->findChild<QPushButton*>("btnAnimationsGroupRemoveFromList");
    assert(this->btnAnimationsGroupRemoveFromList);
    this->btnAnimationsGroupAddToList = w->findChild<QPushButton*>("btnAnimationsGroupAddToList");
    assert(this->btnAnimationsGroupAddToList);
    this->lstAnimationsGroup = w->findChild<QListWidget*>("lstAnimationsGroup");
    assert(this->lstAnimationsGroup);
    this->txtAnimationsGroupName = w->findChild<QLineEdit*>("txtAnimationsGroupName");
    assert(this->txtAnimationsGroupName);
    this->cbAnimationsGroupLooped = w->findChild<QCheckBox*>("cbAnimationsGroupLooped");
    assert(this->cbAnimationsGroupLooped);
    this->btnAnimationsGroupAdd = w->findChild<QPushButton*>("btnAnimationsGroupAdd");
    assert(this->btnAnimationsGroupAdd);
    this->btnAnimationsGroupRemove = w->findChild<QPushButton*>("btnAnimationsGroupRemove");
    assert(this->btnAnimationsGroupRemove);
    this->btnAnimationsGroupStart = w->findChild<QPushButton*>("btnAnimationsGroupStart");
    assert(this->btnAnimationsGroupStart);
    this->btnAnimationsGroupCancel = w->findChild<QPushButton*>("btnAnimationsGroupCancel");
    assert(this->btnAnimationsGroupCancel);
    this->rbAnimationsGroupParallel = w->findChild<QRadioButton*>("rbAnimationsGroupParallel");
    assert(this->rbAnimationsGroupParallel);
    this->rbAnimationsGroupSequential = w->findChild<QRadioButton*>("rbAnimationsGroupSequential");
    assert(this->rbAnimationsGroupSequential);
}

gui::uiblocks::UIAnimationsGroupBlock::~UIAnimationsGroupBlock()
{

}
