/* \file uianimationsgroupblock.h

    Contains definition of UI group for AnimationsGroup group of widgets
 */
#pragma once
class QWidget;
class QListWidget;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QRadioButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for AnimationsGroup group of widgets
 */
class UIAnimationsGroupBlock
{
public:
    /*! Constructs new UI group
     */
    UIAnimationsGroupBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIAnimationsGroupBlock();

    QListWidget* lstAnimationsGroupInGroup;
    QListWidget* lstAnimationsGroupAllAnimations;
    QPushButton* btnAnimationsGroupRemoveFromList;
    QPushButton* btnAnimationsGroupAddToList;
    QListWidget* lstAnimationsGroup;
    QLineEdit* txtAnimationsGroupName;
    QCheckBox* cbAnimationsGroupLooped;
    QPushButton* btnAnimationsGroupAdd;
    QPushButton* btnAnimationsGroupRemove;
    QPushButton* btnAnimationsGroupStart;
    QPushButton* btnAnimationsGroupCancel;
    QRadioButton* rbAnimationsGroupParallel;
    QRadioButton* rbAnimationsGroupSequential;
};

}

}
