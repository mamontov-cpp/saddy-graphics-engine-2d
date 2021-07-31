#include <new>
#include <cassert>
#include "uilabelblock.h"
#include <QPushButton>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include "../gui/fontsizewidget/fontsizewidget.h"
#include "../resourcetreewidget/resourcetreewidget.h"

gui::uiblocks::UILabelBlock::UILabelBlock() : btnLabelAdd(nullptr),
tabLabelOptions(nullptr),
fswLabelFontSize(nullptr),
txtLabelText(nullptr),
rtwLabelFont(nullptr),
cmbLabelBreakText(nullptr),
cmbLabelTextEllipsis(nullptr),
cmbLabelOverflowStrategyForLines(nullptr),
cmbLabelOverflowStrategy(nullptr),
cmbLabelTextEllipsisForLines(nullptr),
spbMaximalLineWidth(nullptr),
dsbLineSpacingRatio(nullptr),
spbMaximalLinesCount(nullptr),
cbLabelHasFormatting(nullptr)
{

}

void gui::uiblocks::UILabelBlock::init(QWidget* w)
{
    assert(w);
    this->btnLabelAdd = w->findChild<QPushButton*>("btnLabelAdd");
    assert(this->btnLabelAdd);
    this->tabLabelOptions = w->findChild<QTabWidget*>("tabLabelOptions");
    assert(this->tabLabelOptions);
    this->fswLabelFontSize = w->findChild<gui::fontsizewidget::FontSizeWidget*>("fswLabelFontSize");
    assert(this->fswLabelFontSize);
    this->txtLabelText = w->findChild<QPlainTextEdit*>("txtLabelText");
    assert(this->txtLabelText);
    this->rtwLabelFont = w->findChild<gui::resourcetreewidget::ResourceTreeWidget*>("rtwLabelFont");
    assert(this->rtwLabelFont);
    this->cmbLabelBreakText = w->findChild<QComboBox*>("cmbLabelBreakText");
    assert(this->cmbLabelBreakText);
    this->cmbLabelTextEllipsis = w->findChild<QComboBox*>("cmbLabelTextEllipsis");
    assert(this->cmbLabelTextEllipsis);
    this->cmbLabelOverflowStrategyForLines = w->findChild<QComboBox*>("cmbLabelOverflowStrategyForLines");
    assert(this->cmbLabelOverflowStrategyForLines);
    this->cmbLabelOverflowStrategy = w->findChild<QComboBox*>("cmbLabelOverflowStrategy");
    assert(this->cmbLabelOverflowStrategy);
    this->cmbLabelTextEllipsisForLines = w->findChild<QComboBox*>("cmbLabelTextEllipsisForLines");
    assert(this->cmbLabelTextEllipsisForLines);
    this->spbMaximalLineWidth = w->findChild<QSpinBox*>("spbMaximalLineWidth");
    assert(this->spbMaximalLineWidth);
    this->dsbLineSpacingRatio = w->findChild<QDoubleSpinBox*>("dsbLineSpacingRatio");
    assert(this->dsbLineSpacingRatio);
    this->spbMaximalLinesCount = w->findChild<QSpinBox*>("spbMaximalLinesCount");
    assert(this->spbMaximalLinesCount);
    this->cbLabelHasFormatting = w->findChild<QCheckBox*>("cbLabelHasFormatting");
    assert(this->cbLabelHasFormatting);
}

gui::uiblocks::UILabelBlock::~UILabelBlock()
{

}
