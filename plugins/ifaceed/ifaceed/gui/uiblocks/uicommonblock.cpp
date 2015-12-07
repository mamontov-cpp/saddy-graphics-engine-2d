#include <new>
#include <cassert>
#include "uicommonblock.h"
#include <QLineEdit>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>

gui::uiblocks::UICommonBlock::UICommonBlock() : txtMousePosX(NULL),
txtMousePosY(NULL),
tabTypes(NULL),
tabObjects(NULL),
twDatabaseProperties(NULL),
btnFixTextureCoordinates(NULL),
btnClearObjectSelection(NULL)
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
}

gui::uiblocks::UICommonBlock::~UICommonBlock()
{

}
