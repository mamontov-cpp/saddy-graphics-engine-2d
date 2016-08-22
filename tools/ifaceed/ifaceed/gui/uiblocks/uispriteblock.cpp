#include <new>
#include <cassert>
#include "uispriteblock.h"
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include "../resourcetreewidget/resourcetreewidget.h"

gui::uiblocks::UISpriteBlock::UISpriteBlock() : rtwSpriteSprite(NULL),
btnSpriteMakeBackground(NULL),
btnSpriteAdd(NULL),
rbPlaceAndRotate(NULL),
rbTwoClicksPlaces(NULL),
cbFlipX(NULL),
cbFlipY(NULL)
{

}

void gui::uiblocks::UISpriteBlock::init(QWidget* w)
{
    assert(w);
    this->rtwSpriteSprite = w->findChild<gui::resourcetreewidget::ResourceTreeWidget*>("rtwSpriteSprite");
    assert(this->rtwSpriteSprite);
    this->btnSpriteMakeBackground = w->findChild<QPushButton*>("btnSpriteMakeBackground");
    assert(this->btnSpriteMakeBackground);
    this->btnSpriteAdd = w->findChild<QPushButton*>("btnSpriteAdd");
    assert(this->btnSpriteAdd);
    this->rbPlaceAndRotate = w->findChild<QRadioButton*>("rbPlaceAndRotate");
    assert(this->rbPlaceAndRotate);
    this->rbTwoClicksPlaces = w->findChild<QRadioButton*>("rbTwoClicksPlaces");
    assert(this->rbTwoClicksPlaces);
    this->cbFlipX = w->findChild<QCheckBox*>("cbFlipX");
    assert(this->cbFlipX);
    this->cbFlipY = w->findChild<QCheckBox*>("cbFlipY");
    assert(this->cbFlipY);
}

gui::uiblocks::UISpriteBlock::~UISpriteBlock()
{

}
