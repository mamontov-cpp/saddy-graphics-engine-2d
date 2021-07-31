#include <new>
#include <cassert>
#include "uilayoutblock.h"
#include <QListWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QScrollArea>
#include "../gui/rectwidget/rectwidget.h"

gui::uiblocks::UILayoutBlock::UILayoutBlock() : lstLayoutGridList(nullptr),
txtLayoutGridName(nullptr),
tabLayoutTab(nullptr),
rwLayoutArea(nullptr),
dsbLayoutPaddingTop(nullptr),
cbLayoutPaddingTopPropagate(nullptr),
dsbLayoutPaddingBottom(nullptr),
cbLayoutPaddingBottomPropagate(nullptr),
dsbLayoutPaddingLeft(nullptr),
cbLayoutPaddingLeftPropagate(nullptr),
dsbLayoutPaddingRight(nullptr),
cbLayoutPaddingRightPropagate(nullptr),
cbLayoutFixedWidth(nullptr),
cbLayoutFixedHeight(nullptr),
btnLayoutMerge(nullptr),
btnLayoutSplit(nullptr),
btnLayoutAdd(nullptr),
btnLayoutRemove(nullptr),
spnLayoutGridRows(nullptr),
spnLayoutGridCols(nullptr),
tblLayoutCells(nullptr),
cbLayoutShow(nullptr),
spnLayoutNavigateCellX(nullptr),
spnLayoutNavigateCellY(nullptr),
btnLayoutAddByStretching(nullptr)
{

}

void gui::uiblocks::UILayoutBlock::init(QWidget* w)
{
    assert(w);
    this->lstLayoutGridList = w->findChild<QListWidget*>("lstLayoutGridList");
    assert(this->lstLayoutGridList);
    this->txtLayoutGridName = w->findChild<QLineEdit*>("txtLayoutGridName");
    assert(this->txtLayoutGridName);
    this->tabLayoutTab = w->findChild<QTabWidget*>("tabLayoutTab");
    assert(this->tabLayoutTab);
    this->rwLayoutArea = w->findChild<gui::rectwidget::RectWidget*>("rwLayoutArea");
    assert(this->rwLayoutArea);
    this->dsbLayoutPaddingTop = w->findChild<QDoubleSpinBox*>("dsbLayoutPaddingTop");
    assert(this->dsbLayoutPaddingTop);
    this->cbLayoutPaddingTopPropagate = w->findChild<QCheckBox*>("cbLayoutPaddingTopPropagate");
    assert(this->cbLayoutPaddingTopPropagate);
    this->dsbLayoutPaddingBottom = w->findChild<QDoubleSpinBox*>("dsbLayoutPaddingBottom");
    assert(this->dsbLayoutPaddingBottom);
    this->cbLayoutPaddingBottomPropagate = w->findChild<QCheckBox*>("cbLayoutPaddingBottomPropagate");
    assert(this->cbLayoutPaddingBottomPropagate);
    this->dsbLayoutPaddingLeft = w->findChild<QDoubleSpinBox*>("dsbLayoutPaddingLeft");
    assert(this->dsbLayoutPaddingLeft);
    this->cbLayoutPaddingLeftPropagate = w->findChild<QCheckBox*>("cbLayoutPaddingLeftPropagate");
    assert(this->cbLayoutPaddingLeftPropagate);
    this->dsbLayoutPaddingRight = w->findChild<QDoubleSpinBox*>("dsbLayoutPaddingRight");
    assert(this->dsbLayoutPaddingRight);
    this->cbLayoutPaddingRightPropagate = w->findChild<QCheckBox*>("cbLayoutPaddingRightPropagate");
    assert(this->cbLayoutPaddingRightPropagate);
    this->cbLayoutFixedWidth = w->findChild<QCheckBox*>("cbLayoutFixedWidth");
    assert(this->cbLayoutFixedWidth);
    this->cbLayoutFixedHeight = w->findChild<QCheckBox*>("cbLayoutFixedHeight");
    assert(this->cbLayoutFixedHeight);
    this->btnLayoutMerge = w->findChild<QPushButton*>("btnLayoutMerge");
    assert(this->btnLayoutMerge);
    this->btnLayoutSplit = w->findChild<QPushButton*>("btnLayoutSplit");
    assert(this->btnLayoutSplit);
    this->btnLayoutAdd = w->findChild<QPushButton*>("btnLayoutAdd");
    assert(this->btnLayoutAdd);
    this->btnLayoutRemove = w->findChild<QPushButton*>("btnLayoutRemove");
    assert(this->btnLayoutRemove);
    this->spnLayoutGridRows = w->findChild<QSpinBox*>("spnLayoutGridRows");
    assert(this->spnLayoutGridRows);
    this->spnLayoutGridCols = w->findChild<QSpinBox*>("spnLayoutGridCols");
    assert(this->spnLayoutGridCols);
    this->tblLayoutCells = w->findChild<QScrollArea*>("tblLayoutCells");
    assert(this->tblLayoutCells);
    this->cbLayoutShow = w->findChild<QCheckBox*>("cbLayoutShow");
    assert(this->cbLayoutShow);
    this->spnLayoutNavigateCellX = w->findChild<QSpinBox*>("spnLayoutNavigateCellX");
    assert(this->spnLayoutNavigateCellX);
    this->spnLayoutNavigateCellY = w->findChild<QSpinBox*>("spnLayoutNavigateCellY");
    assert(this->spnLayoutNavigateCellY);
    this->btnLayoutAddByStretching = w->findChild<QPushButton*>("btnLayoutAddByStretching");
    assert(this->btnLayoutAddByStretching);
}

gui::uiblocks::UILayoutBlock::~UILayoutBlock()
{

}
