/* \file uianimationblock.h

    Contains definition of UI group for Animation group of widgets
 */
#pragma once
namespace gui
{

namespace colorview
{

class ColorView;

}

namespace textedit
{

class TextEdit;

}

namespace rectwidget
{

class RectWidget;

}

}


class QWidget;
class QListWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class QTabWidget;
class QDoubleSpinBox;
class QCheckBox;
class QSpinBox;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Animation group of widgets
 */
class UIAnimationBlock
{
public:
    /*! Constructs new UI group
     */
    UIAnimationBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIAnimationBlock();

    QListWidget* lstAnimations;
    QLineEdit* txtAnimationName;
    QComboBox* cmbAddedAnimationType;
    QPushButton* btnAnimationsAdd;
    QPushButton* btnAnimationsRemove;
    QPushButton* btnAnimationsStart;
    QPushButton* btnAnimationsCancel;
    QTabWidget* tabAnimationTypes;
    QComboBox* cmbWayAnimationWay;
    QPushButton* btnCompositeAnimationRemoveFromList;
    QPushButton* btnCompositeAnimationAddToList;
    QPushButton* btnCompositeAnimationMoveBack;
    QPushButton* btnCompositeAnimationMoveFront;
    QDoubleSpinBox* dsbAnimationTime;
    QCheckBox* cbAnimationLooped;
    QSpinBox* sbBlinkingFrequency;
    gui::colorview::ColorView* cwColorEndingColor;
    gui::colorview::ColorView* cwColorStartingColor;
    QDoubleSpinBox* dabResizeStartingSizeX;
    QDoubleSpinBox* dabResizeStartingSizeY;
    QDoubleSpinBox* dabResizeEndingSizeX;
    QDoubleSpinBox* dabResizeEndingSizeY;
    QDoubleSpinBox* dsbRotateStartingAngle;
    QDoubleSpinBox* dsbRotateEndingAngle;
    QDoubleSpinBox* dabSimpleMovementStartingPointX;
    QDoubleSpinBox* dabSimpleMovementStartingPointY;
    QPushButton* btnSimpleMovementPickStartingPoint;
    QDoubleSpinBox* dabSimpleMovementEndingPointX;
    QDoubleSpinBox* dabSimpleMovementEndingPointY;
    QPushButton* btnSimpleMovementPickEndingPoint;
    gui::textedit::TextEdit* txtFontListList;
    QSpinBox* sbFontSizeStartingSize;
    QSpinBox* sbFontSizeEndingSize;
    gui::textedit::TextEdit* txtOptionListList;
    gui::textedit::TextEdit* txtTextureCoordinatesList;
    gui::rectwidget::RectWidget* rctTCCStartingRect;
    gui::rectwidget::RectWidget* rctTCCEndingRect;
    QDoubleSpinBox* dsbCameraRotationPivotX;
    QDoubleSpinBox* dsbCameraRotationPivotY;
    QDoubleSpinBox* dsbCameraRotationStartingAngle;
    QDoubleSpinBox* dsbCameraRotationEndingAngle;
    QDoubleSpinBox* dsbCameraShakingOffsetX;
    QDoubleSpinBox* dsbCameraShakingOffsetY;
    QSpinBox* sbCameraShakingFrequency;
    QListWidget* lstCompositeList;
    QListWidget* lstCompositeCandidates;
    QTabWidget* tabAnimationObjectTypes;
    QComboBox* cmbAnimationEasingType;
    QDoubleSpinBox* dsbAnimationEasingOvershootAmplitude;
    QDoubleSpinBox* dsbAnimationEasingPeriod;
};

}

}
