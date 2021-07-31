/* \file uianimationinstanceblock.h

    Contains definition of UI group for AnimationInstance group of widgets
 */
#pragma once
class QWidget;
class QListWidget;
class QLineEdit;
class QComboBox;
class QRadioButton;
class QDoubleSpinBox;
class QPushButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for AnimationInstance group of widgets
 */
class UIAnimationInstanceBlock
{
public:
    /*! Constructs new UI group
     */
    UIAnimationInstanceBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIAnimationInstanceBlock();

    QListWidget* lstAnimationInstances;
    QLineEdit* txtAnimationInstanceName;
    QComboBox* cmbAnimationInstanceAnimationFromTree;
    QRadioButton* rbAnimationInstanceFromTree;
    QComboBox* cmbAnimationInstanceAnimationFromDatabase;
    QRadioButton* rbAnimationInstanceFromDatabase;
    QComboBox* cmbAnimationInstanceObject;
    QDoubleSpinBox* dsbAnimationInstanceStartTime;
    QComboBox* cmbWayAnimationInstanceWay;
    QPushButton* btnAnimationsInstanceStart;
    QPushButton* btnAnimationsInstanceCancel;
    QComboBox* cmbAddedAnimationInstanceType;
    QPushButton* btnAnimationsInstanceAdd;
    QPushButton* btnAnimationsInstanceRemove;
};

}

}
