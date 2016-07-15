#include <new>
#include <cassert>
#include "uisceneblock.h"
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>

gui::uiblocks::UISceneBlock::UISceneBlock() : lstScenes(NULL),
txtSceneName(NULL),
btnSceneAdd(NULL),
btnScenesMoveFront(NULL),
btnScenesMoveBack(NULL),
btnSceneDelete(NULL),
lstSceneObjects(NULL),
btnSceneClear(NULL)
{

}

void gui::uiblocks::UISceneBlock::init(QWidget* w)
{
    assert(w);
    this->lstScenes = w->findChild<QListWidget*>("lstScenes");
    assert(this->lstScenes);
    this->txtSceneName = w->findChild<QLineEdit*>("txtSceneName");
    assert(this->txtSceneName);
    this->btnSceneAdd = w->findChild<QPushButton*>("btnSceneAdd");
    assert(this->btnSceneAdd);
    this->btnScenesMoveFront = w->findChild<QPushButton*>("btnScenesMoveFront");
    assert(this->btnScenesMoveFront);
    this->btnScenesMoveBack = w->findChild<QPushButton*>("btnScenesMoveBack");
    assert(this->btnScenesMoveBack);
    this->btnSceneDelete = w->findChild<QPushButton*>("btnSceneDelete");
    assert(this->btnSceneDelete);
    this->lstSceneObjects = w->findChild<QListWidget*>("lstSceneObjects");
    assert(this->lstSceneObjects);
    this->btnSceneClear = w->findChild<QPushButton*>("btnSceneClear");
    assert(this->btnSceneClear);
}

gui::uiblocks::UISceneBlock::~UISceneBlock()
{

}
