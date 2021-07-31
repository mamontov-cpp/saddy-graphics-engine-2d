/* \file uilabelblock.h

    Contains definition of UI group for Label group of widgets
 */
#pragma once
namespace gui
{

namespace fontsizewidget
{

class FontSizeWidget;

}

namespace resourcetreewidget
{

class ResourceTreeWidget;

}

}


class QWidget;
class QPushButton;
class QTabWidget;
class QPlainTextEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Label group of widgets
 */
class UILabelBlock
{
public:
    /*! Constructs new UI group
     */
    UILabelBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UILabelBlock();

    QPushButton* btnLabelAdd;
    QTabWidget* tabLabelOptions;
    gui::fontsizewidget::FontSizeWidget* fswLabelFontSize;
    QPlainTextEdit* txtLabelText;
    gui::resourcetreewidget::ResourceTreeWidget* rtwLabelFont;
    QComboBox* cmbLabelBreakText;
    QComboBox* cmbLabelTextEllipsis;
    QComboBox* cmbLabelOverflowStrategyForLines;
    QComboBox* cmbLabelOverflowStrategy;
    QComboBox* cmbLabelTextEllipsisForLines;
    QSpinBox* spbMaximalLineWidth;
    QDoubleSpinBox* dsbLineSpacingRatio;
    QSpinBox* spbMaximalLinesCount;
    QCheckBox* cbLabelHasFormatting;
};

}

}
