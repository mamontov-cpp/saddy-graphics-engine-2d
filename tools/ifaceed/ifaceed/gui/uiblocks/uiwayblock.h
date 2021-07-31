/* \file uiwayblock.h

    Contains definition of UI group for Way group of widgets
 */
#pragma once
class QWidget;
class QListWidget;
class QLineEdit;
class QDoubleSpinBox;
class QCheckBox;
class QPushButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Way group of widgets
 */
class UIWayBlock
{
public:
    /*! Constructs new UI group
     */
    UIWayBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIWayBlock();

    QListWidget* lstWays;
    QLineEdit* txtWayName;
    QDoubleSpinBox* dsbWayTotalTime;
    QCheckBox* cbWayClosed;
    QPushButton* btnWayAdd;
    QPushButton* btnWayRemove;
    QListWidget* lstWayPoints;
    QDoubleSpinBox* dsbWayPointX;
    QDoubleSpinBox* dsbWayPointY;
    QPushButton* btnWayPointAdd;
    QPushButton* btnWayPointRemove;
    QPushButton* btnWayPointMoveBack;
    QPushButton* btnWayPointMoveFront;
};

}

}
