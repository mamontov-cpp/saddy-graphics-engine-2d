/*! \file dialoguesphraseref.h
    

    A wrapper for reference to a phrase from dialogue
 */
#pragma once

#include <QObject>

#include <dialogue/dialogue.h>
#include <dialogue/phrase.h>

namespace gui
{
    
namespace actions
{
    
class DialogueActions;

}

}

namespace scripting
{

class Scripting;

namespace dialogues
{

/*! A wrapper for reference to a phrase from dialogue
 */
class PhraseRef: public QObject
{
 Q_OBJECT
 Q_PROPERTY(QString actorName READ actorName WRITE setActorName)
 Q_PROPERTY(QString actorPortrait READ actorPortrait WRITE setActorPortrait)
 Q_PROPERTY(QString text READ text WRITE setText)
 Q_PROPERTY(double duration READ duration WRITE setDuration)
 Q_PROPERTY(QString viewHint READ viewHint WRITE setViewHint)
 Q_PROPERTY(unsigned int position READ position)
public:
    /*! Constructs new wrapper
     */
    PhraseRef();
    /*! Constructs new wrapper
        \param[in] scripting a scripting part
        \param[in] d a dialogue, which is referenced
        \param[in] pos a position
     */
    PhraseRef(scripting::Scripting* scripting, sad::dialogue::Dialogue* d, unsigned int pos);
    /*! Could be inherited
     */
    virtual ~PhraseRef();
    /*! Tests, whether phrase ref is valid
        \return whether is valid
     */
    bool valid() const;
    /*! Converts a phrase to a phrase
     */
    const sad::dialogue::Phrase& toPhrase() const;
    /*! Returns actions, linked to main panel
        \return actions
     */
    gui::actions::DialogueActions* actions() const;
    /*! Sets actor name for phrase
        \param[in] name a name for actor
     */
    void setActorName(QString name);
    /*! Sets actor portrait
        \param[in] portrait a portrait for actor
     */
    void setActorPortrait(QString portrait);
    /*! Sets phrase 
        \param[in] phrase a phrase
     */
    void setText(QString phrase);
    /*! Sets duration 
        \param[in] duration a duration for phrase
     */ 
    void setDuration(double duration);
    /*! Sets view hint for a phrase
        \param[in] viewhint a hint for viewing phrase
     */
    void setViewHint(QString viewhint);
    /*! Returns actor name
        \return actor name
     */
    QString actorName() const;
    /*! Returns actor portrait, which could be used as link to sad::Sprite2D::Options link
        \return actor portrait
     */
    QString actorPortrait() const;
    /*! Returns phrase text
        \return phrase text
     */
    QString text() const;
    /*! Returns duration
        \return duration
     */
    double duration() const;
    /*! Returns view hint for a phrase
        \return view hint for a phrase
     */
    QString viewHint() const;
    /*! Returns a position to point ref
        \return position
     */
    unsigned int position() const;
public slots:
    /*! Contains to string
        \return object to string
    */
    QString toString() const;
    /*! Moves point back in list
     */
    void moveBack();
    /*! Moves point front in list
     */
    void moveFront();
protected:
    /*! A scripting part
     */
    scripting::Scripting* m_scripting;
    /*! An inner point dialogue
     */
    sad::dialogue::Dialogue* m_d;
    /*! A position of reference in list
     */
    unsigned int m_pos;
};

}

}
