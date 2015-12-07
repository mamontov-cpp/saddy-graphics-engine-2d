#include <new>
#include <cassert>
#include "uiscenenodeblock.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include "../gui/colorpicker/colorpicker.h"
#include "../gui/rectwidget/rectwidget.h"
#include "../anglewidget/anglewidget.h"

gui::uiblocks::UISceneNodeBlock::UISceneNodeBlock() : btnSceneNodeDelete(NULL),
btnSceneNodeMoveFront(NULL),
btnSceneNodeMoveBack(NULL),
clpSceneNodeColor(NULL),
rwSceneNodeRect(NULL),
awSceneNodeAngle(NULL),
cbSceneNodeVisible(NULL),
txtObjectName(NULL)
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
}

gui::uiblocks::UISceneNodeBlock::~UISceneNodeBlock()
{

}
