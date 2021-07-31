/* \file uispriteblock.h

    Contains definition of UI group for Sprite group of widgets
 */
#pragma once
namespace gui
{

namespace resourcetreewidget
{

class ResourceTreeWidget;

}

}


class QWidget;
class QPushButton;
class QRadioButton;
class QCheckBox;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Sprite group of widgets
 */
class UISpriteBlock
{
public:
    /*! Constructs new UI group
     */
    UISpriteBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UISpriteBlock();

    gui::resourcetreewidget::ResourceTreeWidget* rtwSpriteSprite;
    QPushButton* btnSpriteMakeBackground;
    QPushButton* btnSpriteAdd;
    QRadioButton* rbPlaceAndRotate;
    QRadioButton* rbTwoClicksPlaces;
    QCheckBox* cbFlipX;
    QCheckBox* cbFlipY;
};

}

}
