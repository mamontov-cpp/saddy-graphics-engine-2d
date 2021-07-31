/*!\file eventfilter.h
 * 
 *
 * Describes an event filter, used to react
 * to global hot keys in application, like
 * pressing Ctrl+Z, Ctrl+R, Ctrl+1,2,3,4,5,6 and
 * Ctrl+F1 and Ctrl+F2 in future
 */
#pragma once
#include <QObject>
#include <QKeyEvent>

namespace core
{

class Editor;

}

namespace gui
{

/*!
 * \class EventFilter
 * An event filter used to react
 * to global hot keys in application, like
 * pressing Ctrl+Z, Ctrl+R, Ctrl+1,2,3,4,5,6 and
 * Ctrl+F1 and Ctrl+F2 in future
 */
class EventFilter : public QObject
{
    Q_OBJECT
public:
    /*! Creates new event filter
        \param[in] parent a parent object
     */
    explicit EventFilter(QObject *parent = nullptr);
    /*!
     * Sets editor for event filter
     * \param e editor
     */
    void setEditor(core::Editor* e);
    /*!
     * Handled events for filter
     * \param[in] o an object, which emitted 
     * \param[in] e an event
     * \return whether event is handled
     */
    virtual bool eventFilter(QObject *o, QEvent *e) override;
protected:
    /*!
     * A linked editor
     */
    core::Editor* m_editor;
};

}
