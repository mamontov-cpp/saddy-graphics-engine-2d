/* \file uiscenenodeblock.h

    Contains definition of UI group for SceneNode group of widgets
 */
#pragma once
namespace gui
{

namespace colorpicker
{

class ColorPicker;

}

namespace rectwidget
{

class RectWidget;

}

namespace anglewidget
{

class AngleWidget;

}

}


class QWidget;
class QPushButton;
class QCheckBox;
class QLineEdit;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for SceneNode group of widgets
 */
class UISceneNodeBlock
{
public:
    /*! Constructs new UI group
     */
    UISceneNodeBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UISceneNodeBlock();

    QPushButton* btnSceneNodeDelete;
    QPushButton* btnSceneNodeMoveFront;
    QPushButton* btnSceneNodeMoveBack;
    gui::colorpicker::ColorPicker* clpSceneNodeColor;
    gui::rectwidget::RectWidget* rwSceneNodeRect;
    gui::anglewidget::AngleWidget* awSceneNodeAngle;
    QCheckBox* cbSceneNodeVisible;
    QLineEdit* txtObjectName;
    QPushButton* btnSceneNodeSpanBetweenTwoPoints;
    QPushButton* btnSceneNodeCopySizeAndPosition;
};

}

}
