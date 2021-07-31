#include <new>
#include <cassert>
#include "uicommonblock.h"
#include <QLineEdit>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QCheckBox>

gui::uiblocks::UICommonBlock::UICommonBlock() : txtMousePosX(nullptr),
txtMousePosY(nullptr),
tabTypes(nullptr),
tabObjects(nullptr),
twDatabaseProperties(nullptr),
btnFixTextureCoordinates(nullptr),
btnClearObjectSelection(nullptr),
cbFastMode(nullptr)
{

}

void gui::uiblocks::UICommonBlock::init(QWidget* w)
{
    assert(w);
    this->txtMousePosX = w->findChild<QLineEdit*>("txtMousePosX");
    assert(this->txtMousePosX);
    this->txtMousePosY = w->findChild<QLineEdit*>("txtMousePosY");
    assert(this->txtMousePosY);
    this->tabTypes = w->findChild<QTabWidget*>("tabTypes");
    assert(this->tabTypes);
    this->tabObjects = w->findChild<QTabWidget*>("tabObjects");
    assert(this->tabObjects);
    this->twDatabaseProperties = w->findChild<QTableWidget*>("twDatabaseProperties");
    assert(this->twDatabaseProperties);
    this->btnFixTextureCoordinates = w->findChild<QPushButton*>("btnFixTextureCoordinates");
    assert(this->btnFixTextureCoordinates);
    this->btnClearObjectSelection = w->findChild<QPushButton*>("btnClearObjectSelection");
    assert(this->btnClearObjectSelection);
    this->cbFastMode = w->findChild<QCheckBox*>("cbFastMode");
    assert(this->cbFastMode);
}

gui::uiblocks::UICommonBlock::~UICommonBlock()
{

}
