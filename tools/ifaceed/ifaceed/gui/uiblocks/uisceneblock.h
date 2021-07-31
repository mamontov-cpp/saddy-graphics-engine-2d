/* \file uisceneblock.h

    Contains definition of UI group for Scene group of widgets
 */
#pragma once
class QWidget;
class QListWidget;
class QLineEdit;
class QPushButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Scene group of widgets
 */
class UISceneBlock
{
public:
    /*! Constructs new UI group
     */
    UISceneBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UISceneBlock();

    QListWidget* lstScenes;
    QLineEdit* txtSceneName;
    QPushButton* btnSceneAdd;
    QPushButton* btnScenesMoveFront;
    QPushButton* btnScenesMoveBack;
    QPushButton* btnSceneDelete;
    QListWidget* lstSceneObjects;
    QPushButton* btnSceneClear;
};

}

}
