#include <new>
#include <cassert>
#include "uiconsoleblock.h"
#include <QPushButton>
#include <QTextEdit>
#include "../gui/codeedit/codeedit.h"

gui::uiblocks::UIConsoleBlock::UIConsoleBlock() : txtConsoleCode(nullptr),
btnConsoleRun(nullptr),
txtConsoleResults(nullptr),
btnConsoleHelp(nullptr),
btnConsoleSave(nullptr),
btnConsoleLoad(nullptr)
{

}

void gui::uiblocks::UIConsoleBlock::init(QWidget* w)
{
    assert(w);
    this->txtConsoleCode = w->findChild<gui::codeedit::CodeEdit*>("txtConsoleCode");
    assert(this->txtConsoleCode);
    this->btnConsoleRun = w->findChild<QPushButton*>("btnConsoleRun");
    assert(this->btnConsoleRun);
    this->txtConsoleResults = w->findChild<QTextEdit*>("txtConsoleResults");
    assert(this->txtConsoleResults);
    this->btnConsoleHelp = w->findChild<QPushButton*>("btnConsoleHelp");
    assert(this->btnConsoleHelp);
    this->btnConsoleSave = w->findChild<QPushButton*>("btnConsoleSave");
    assert(this->btnConsoleSave);
    this->btnConsoleLoad = w->findChild<QPushButton*>("btnConsoleLoad");
    assert(this->btnConsoleLoad);
}

gui::uiblocks::UIConsoleBlock::~UIConsoleBlock()
{

}
