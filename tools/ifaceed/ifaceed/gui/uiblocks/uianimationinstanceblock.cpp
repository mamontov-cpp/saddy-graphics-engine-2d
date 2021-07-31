#include <new>
#include <cassert>
#include "uianimationinstanceblock.h"
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QPushButton>

gui::uiblocks::UIAnimationInstanceBlock::UIAnimationInstanceBlock() : lstAnimationInstances(nullptr),
txtAnimationInstanceName(nullptr),
cmbAnimationInstanceAnimationFromTree(nullptr),
rbAnimationInstanceFromTree(nullptr),
cmbAnimationInstanceAnimationFromDatabase(nullptr),
rbAnimationInstanceFromDatabase(nullptr),
cmbAnimationInstanceObject(nullptr),
dsbAnimationInstanceStartTime(nullptr),
cmbWayAnimationInstanceWay(nullptr),
btnAnimationsInstanceStart(nullptr),
btnAnimationsInstanceCancel(nullptr),
cmbAddedAnimationInstanceType(nullptr),
btnAnimationsInstanceAdd(nullptr),
btnAnimationsInstanceRemove(nullptr)
{

}

void gui::uiblocks::UIAnimationInstanceBlock::init(QWidget* w)
{
    assert(w);
    this->lstAnimationInstances = w->findChild<QListWidget*>("lstAnimationInstances");
    assert(this->lstAnimationInstances);
    this->txtAnimationInstanceName = w->findChild<QLineEdit*>("txtAnimationInstanceName");
    assert(this->txtAnimationInstanceName);
    this->cmbAnimationInstanceAnimationFromTree = w->findChild<QComboBox*>("cmbAnimationInstanceAnimationFromTree");
    assert(this->cmbAnimationInstanceAnimationFromTree);
    this->rbAnimationInstanceFromTree = w->findChild<QRadioButton*>("rbAnimationInstanceFromTree");
    assert(this->rbAnimationInstanceFromTree);
    this->cmbAnimationInstanceAnimationFromDatabase = w->findChild<QComboBox*>("cmbAnimationInstanceAnimationFromDatabase");
    assert(this->cmbAnimationInstanceAnimationFromDatabase);
    this->rbAnimationInstanceFromDatabase = w->findChild<QRadioButton*>("rbAnimationInstanceFromDatabase");
    assert(this->rbAnimationInstanceFromDatabase);
    this->cmbAnimationInstanceObject = w->findChild<QComboBox*>("cmbAnimationInstanceObject");
    assert(this->cmbAnimationInstanceObject);
    this->dsbAnimationInstanceStartTime = w->findChild<QDoubleSpinBox*>("dsbAnimationInstanceStartTime");
    assert(this->dsbAnimationInstanceStartTime);
    this->cmbWayAnimationInstanceWay = w->findChild<QComboBox*>("cmbWayAnimationInstanceWay");
    assert(this->cmbWayAnimationInstanceWay);
    this->btnAnimationsInstanceStart = w->findChild<QPushButton*>("btnAnimationsInstanceStart");
    assert(this->btnAnimationsInstanceStart);
    this->btnAnimationsInstanceCancel = w->findChild<QPushButton*>("btnAnimationsInstanceCancel");
    assert(this->btnAnimationsInstanceCancel);
    this->cmbAddedAnimationInstanceType = w->findChild<QComboBox*>("cmbAddedAnimationInstanceType");
    assert(this->cmbAddedAnimationInstanceType);
    this->btnAnimationsInstanceAdd = w->findChild<QPushButton*>("btnAnimationsInstanceAdd");
    assert(this->btnAnimationsInstanceAdd);
    this->btnAnimationsInstanceRemove = w->findChild<QPushButton*>("btnAnimationsInstanceRemove");
    assert(this->btnAnimationsInstanceRemove);
}

gui::uiblocks::UIAnimationInstanceBlock::~UIAnimationInstanceBlock()
{

}
