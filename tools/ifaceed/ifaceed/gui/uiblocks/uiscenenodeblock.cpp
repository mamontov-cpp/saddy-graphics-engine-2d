#include <new>
#include <cassert>
#include "uiscenenodeblock.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include "../gui/qt-palette-color-picker/colorpicker.h"
#include "../gui/rectwidget/rectwidget.h"
#include "../anglewidget/anglewidget.h"

gui::uiblocks::UISceneNodeBlock::UISceneNodeBlock() : btnSceneNodeDelete(nullptr),
btnSceneNodeMoveFront(nullptr),
btnSceneNodeMoveBack(nullptr),
clpSceneNodeColor(nullptr),
rwSceneNodeRect(nullptr),
awSceneNodeAngle(nullptr),
cbSceneNodeVisible(nullptr),
txtObjectName(nullptr),
btnSceneNodeSpanBetweenTwoPoints(nullptr),
btnSceneNodeCopySizeAndPosition(nullptr)
{

}

void gui::uiblocks::UISceneNodeBlock::init(QWidget* w)
{
    assert(w);
    this->btnSceneNodeDelete = w->findChild<QPushButton*>("btnSceneNodeDelete");
    assert(this->btnSceneNodeDelete);
    this->btnSceneNodeMoveFront = w->findChild<QPushButton*>("btnSceneNodeMoveFront");
    assert(this->btnSceneNodeMoveFront);
    this->btnSceneNodeMoveBack = w->findChild<QPushButton*>("btnSceneNodeMoveBack");
    assert(this->btnSceneNodeMoveBack);
    this->clpSceneNodeColor = w->findChild<gui::colorpicker::ColorPicker*>("clpSceneNodeColor");
    assert(this->clpSceneNodeColor);
    this->rwSceneNodeRect = w->findChild<gui::rectwidget::RectWidget*>("rwSceneNodeRect");
    assert(this->rwSceneNodeRect);
    this->awSceneNodeAngle = w->findChild<gui::anglewidget::AngleWidget*>("awSceneNodeAngle");
    assert(this->awSceneNodeAngle);
    this->cbSceneNodeVisible = w->findChild<QCheckBox*>("cbSceneNodeVisible");
    assert(this->cbSceneNodeVisible);
    this->txtObjectName = w->findChild<QLineEdit*>("txtObjectName");
    assert(this->txtObjectName);
    this->btnSceneNodeSpanBetweenTwoPoints = w->findChild<QPushButton*>("btnSceneNodeSpanBetweenTwoPoints");
    assert(this->btnSceneNodeSpanBetweenTwoPoints);
    this->btnSceneNodeCopySizeAndPosition = w->findChild<QPushButton*>("btnSceneNodeCopySizeAndPosition");
    assert(this->btnSceneNodeCopySizeAndPosition);
}

gui::uiblocks::UISceneNodeBlock::~UISceneNodeBlock()
{

}
