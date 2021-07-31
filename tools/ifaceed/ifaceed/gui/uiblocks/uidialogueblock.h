/* \file uidialogueblock.h

    Contains definition of UI group for Dialogue group of widgets
 */
#pragma once
class QWidget;
class QListWidget;
class QLineEdit;
class QPushButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Dialogue group of widgets
 */
class UIDialogueBlock
{
public:
    /*! Constructs new UI group
     */
    UIDialogueBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIDialogueBlock();

    QListWidget* lstDialogues;
    QLineEdit* txtDialogueName;
    QPushButton* btnDialogueAdd;
    QPushButton* btnDialogueRemove;
};

}

}
