/* \file uiphraseblock.h

    Contains definition of UI group for Phrase group of widgets
 */
#pragma once
class QWidget;
class QListWidget;
class QDoubleSpinBox;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Phrase group of widgets
 */
class UIPhraseBlock
{
public:
    /*! Constructs new UI group
     */
    UIPhraseBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIPhraseBlock();

    QListWidget* lstPhrases;
    QDoubleSpinBox* dsbPhraseDuration;
    QLineEdit* txtPhraseViewHint;
    QLineEdit* txtPhraseActorName;
    QLineEdit* txtPhraseActorPortrait;
    QPlainTextEdit* txtPhrasePhrase;
    QPushButton* btnPhraseAdd;
    QPushButton* btnPhraseRemove;
    QPushButton* btnPhraseMoveBack;
    QPushButton* btnPhraseMoveFront;
};

}

}
