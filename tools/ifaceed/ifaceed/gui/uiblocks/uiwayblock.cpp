#include <new>
#include <cassert>
#include "uiwayblock.h"
#include <QListWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>

gui::uiblocks::UIWayBlock::UIWayBlock() : lstWays(nullptr),
txtWayName(nullptr),
dsbWayTotalTime(nullptr),
cbWayClosed(nullptr),
btnWayAdd(nullptr),
btnWayRemove(nullptr),
lstWayPoints(nullptr),
dsbWayPointX(nullptr),
dsbWayPointY(nullptr),
btnWayPointAdd(nullptr),
btnWayPointRemove(nullptr),
btnWayPointMoveBack(nullptr),
btnWayPointMoveFront(nullptr)
{

}

void gui::uiblocks::UIWayBlock::init(QWidget* w)
{
    assert(w);
    this->lstWays = w->findChild<QListWidget*>("lstWays");
    assert(this->lstWays);
    this->txtWayName = w->findChild<QLineEdit*>("txtWayName");
    assert(this->txtWayName);
    this->dsbWayTotalTime = w->findChild<QDoubleSpinBox*>("dsbWayTotalTime");
    assert(this->dsbWayTotalTime);
    this->cbWayClosed = w->findChild<QCheckBox*>("cbWayClosed");
    assert(this->cbWayClosed);
    this->btnWayAdd = w->findChild<QPushButton*>("btnWayAdd");
    assert(this->btnWayAdd);
    this->btnWayRemove = w->findChild<QPushButton*>("btnWayRemove");
    assert(this->btnWayRemove);
    this->lstWayPoints = w->findChild<QListWidget*>("lstWayPoints");
    assert(this->lstWayPoints);
    this->dsbWayPointX = w->findChild<QDoubleSpinBox*>("dsbWayPointX");
    assert(this->dsbWayPointX);
    this->dsbWayPointY = w->findChild<QDoubleSpinBox*>("dsbWayPointY");
    assert(this->dsbWayPointY);
    this->btnWayPointAdd = w->findChild<QPushButton*>("btnWayPointAdd");
    assert(this->btnWayPointAdd);
    this->btnWayPointRemove = w->findChild<QPushButton*>("btnWayPointRemove");
    assert(this->btnWayPointRemove);
    this->btnWayPointMoveBack = w->findChild<QPushButton*>("btnWayPointMoveBack");
    assert(this->btnWayPointMoveBack);
    this->btnWayPointMoveFront = w->findChild<QPushButton*>("btnWayPointMoveFront");
    assert(this->btnWayPointMoveFront);
}

gui::uiblocks::UIWayBlock::~UIWayBlock()
{

}
