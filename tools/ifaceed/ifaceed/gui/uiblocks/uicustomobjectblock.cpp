#include <new>
#include <cassert>
#include "uicustomobjectblock.h"
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QRadioButton>
#include "../resourcetreewidget/resourcetreewidget.h"

gui::uiblocks::UICustomObjectBlock::UICustomObjectBlock() : btnCustomObjectAdd(nullptr),
tabCustomObjectsProperties(nullptr),
rtwCustomObjectSchemas(nullptr),
twCustomObjectProperties(nullptr),
rbCustomObjectPlaceAndRotate(nullptr),
rbCustomObjectTwoClicksPlaces(nullptr)
{

}

void gui::uiblocks::UICustomObjectBlock::init(QWidget* w)
{
    assert(w);
    this->btnCustomObjectAdd = w->findChild<QPushButton*>("btnCustomObjectAdd");
    assert(this->btnCustomObjectAdd);
    this->tabCustomObjectsProperties = w->findChild<QTabWidget*>("tabCustomObjectsProperties");
    assert(this->tabCustomObjectsProperties);
    this->rtwCustomObjectSchemas = w->findChild<gui::resourcetreewidget::ResourceTreeWidget*>("rtwCustomObjectSchemas");
    assert(this->rtwCustomObjectSchemas);
    this->twCustomObjectProperties = w->findChild<QTableWidget*>("twCustomObjectProperties");
    assert(this->twCustomObjectProperties);
    this->rbCustomObjectPlaceAndRotate = w->findChild<QRadioButton*>("rbCustomObjectPlaceAndRotate");
    assert(this->rbCustomObjectPlaceAndRotate);
    this->rbCustomObjectTwoClicksPlaces = w->findChild<QRadioButton*>("rbCustomObjectTwoClicksPlaces");
    assert(this->rbCustomObjectTwoClicksPlaces);
}

gui::uiblocks::UICustomObjectBlock::~UICustomObjectBlock()
{

}
