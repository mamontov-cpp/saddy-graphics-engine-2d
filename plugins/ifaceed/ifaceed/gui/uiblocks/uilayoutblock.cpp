#include <new>
#include <cassert>
#include "uilayoutblock.h"
#include <QListWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QPushButton>
#include "../gui/rectwidget/rectwidget.h"

gui::uiblocks::UILayoutBlock::UILayoutBlock() : lstLayoutGridList(NULL),
txtLayoutGridName(NULL),
tabLayoutTab(NULL),
rwLayoutArea(NULL),
spnLayoutRows(NULL),
spnLayoutCols(NULL),
dsbLayoutPaddingTop(NULL),
cbLayoutPaddingTopPropagate(NULL),
dsbLayoutPaddingBottom(NULL),
cbLayoutPaddingBottomPropagate(NULL),
dsbLayoutPaddingLeft(NULL),
cbLayoutPaddingLeftPropagate(NULL),
dsbLayoutPaddingRight(NULL),
cbLayoutPaddingRightPropagate(NULL),
cbLayoutFixedWidth(NULL),
cbLayoutFixedHeight(NULL),
tblLayoutCells(NULL),
btnLayoutMerge(NULL),
btnLayoutSplit(NULL),
btnLayoutAdd(NULL),
btnLayoutRemove(NULL)
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
    this->spnLayoutRows = w->findChild<QSpinBox*>("spnLayoutRows");
    assert(this->spnLayoutRows);
    this->spnLayoutCols = w->findChild<QSpinBox*>("spnLayoutCols");
    assert(this->spnLayoutCols);
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
    this->tblLayoutCells = w->findChild<QTableWidget*>("tblLayoutCells");
    assert(this->tblLayoutCells);
    this->btnLayoutMerge = w->findChild<QPushButton*>("btnLayoutMerge");
    assert(this->btnLayoutMerge);
    this->btnLayoutSplit = w->findChild<QPushButton*>("btnLayoutSplit");
    assert(this->btnLayoutSplit);
    this->btnLayoutAdd = w->findChild<QPushButton*>("btnLayoutAdd");
    assert(this->btnLayoutAdd);
    this->btnLayoutRemove = w->findChild<QPushButton*>("btnLayoutRemove");
    assert(this->btnLayoutRemove);
}

gui::uiblocks::UILayoutBlock::~UILayoutBlock()
{

}
