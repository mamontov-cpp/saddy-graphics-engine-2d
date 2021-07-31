/* \file uilayoutblock.h

    Contains definition of UI group for Layout group of widgets
 */
#pragma once
namespace gui
{

namespace rectwidget
{

class RectWidget;

}

}


class QWidget;
class QListWidget;
class QLineEdit;
class QTabWidget;
class QDoubleSpinBox;
class QCheckBox;
class QPushButton;
class QSpinBox;
class QScrollArea;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Layout group of widgets
 */
class UILayoutBlock
{
public:
    /*! Constructs new UI group
     */
    UILayoutBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UILayoutBlock();

    QListWidget* lstLayoutGridList;
    QLineEdit* txtLayoutGridName;
    QTabWidget* tabLayoutTab;
    gui::rectwidget::RectWidget* rwLayoutArea;
    QDoubleSpinBox* dsbLayoutPaddingTop;
    QCheckBox* cbLayoutPaddingTopPropagate;
    QDoubleSpinBox* dsbLayoutPaddingBottom;
    QCheckBox* cbLayoutPaddingBottomPropagate;
    QDoubleSpinBox* dsbLayoutPaddingLeft;
    QCheckBox* cbLayoutPaddingLeftPropagate;
    QDoubleSpinBox* dsbLayoutPaddingRight;
    QCheckBox* cbLayoutPaddingRightPropagate;
    QCheckBox* cbLayoutFixedWidth;
    QCheckBox* cbLayoutFixedHeight;
    QPushButton* btnLayoutMerge;
    QPushButton* btnLayoutSplit;
    QPushButton* btnLayoutAdd;
    QPushButton* btnLayoutRemove;
    QSpinBox* spnLayoutGridRows;
    QSpinBox* spnLayoutGridCols;
    QScrollArea* tblLayoutCells;
    QCheckBox* cbLayoutShow;
    QSpinBox* spnLayoutNavigateCellX;
    QSpinBox* spnLayoutNavigateCellY;
    QPushButton* btnLayoutAddByStretching;
};

}

}
