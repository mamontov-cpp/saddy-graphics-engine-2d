/* \file uiconsoleblock.h

    Contains definition of UI group for Console group of widgets
 */
#pragma once
namespace gui
{

namespace codeedit
{

class CodeEdit;

}

}


class QWidget;
class QPushButton;
class QTextEdit;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Console group of widgets
 */
class UIConsoleBlock
{
public:
    /*! Constructs new UI group
     */
    UIConsoleBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIConsoleBlock();

    gui::codeedit::CodeEdit* txtConsoleCode;
    QPushButton* btnConsoleRun;
    QTextEdit* txtConsoleResults;
    QPushButton* btnConsoleHelp;
    QPushButton* btnConsoleSave;
    QPushButton* btnConsoleLoad;
};

}

}
