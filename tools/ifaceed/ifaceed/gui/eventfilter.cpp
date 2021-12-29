#include "eventfilter.h"

#include "../core/editor.h"

#include "mainpanelproxy.h"

gui::EventFilter::EventFilter(QObject *parent) :
    QObject(parent), m_editor(nullptr)
{
}

void gui::EventFilter::setEditor(core::Editor* e)
{
    m_editor = e;
}

bool gui::EventFilter::eventFilter(QObject *o, QEvent *e)
{
    if (!m_editor)
    {
        return QObject::eventFilter(o, e);
    }
    bool handled = false;
    gui::MainPanelProxy* proxy = m_editor->panelProxy();
    if(e->type() == QEvent::KeyPress && m_editor)
    {
        QKeyEvent* ev = static_cast<QKeyEvent*>(e);
        if (ev->modifiers() & Qt::ControlModifier)
        {
            if (ev->key() == Qt::Key_D)
            {
                handled = true;
                proxy->clearObjectSelection();
            }
            if (ev->key() == Qt::Key_Z)
            {
                handled = true;
                m_editor->undo();
            }
            if (ev->key() == Qt::Key_R)
            {
                handled = true;
                m_editor->redo();
            }
            if (m_editor->shared()->isAnyKindOfAnimationIsRunning() == false)
            {
                if (ev->key() == Qt::Key_F1)
                {
                    if (m_editor->isInWaysEditingState())
                    {
                        m_editor->tryEnterObjectEditingState();
                    }
                }
                if (ev->key() == Qt::Key_F2)
                {
                    if (m_editor->isInObjectEditingState())
                    {
                        m_editor->tryEnterWayEditingState();
                    }
                }
                if (ev->key() == Qt::Key_F3)
                {
                    m_editor->machine()->enterState("idle");
                    proxy->setCurrentTabWidgetIndex(2);
                }
                if (ev->key() == Qt::Key_F4)
                {
                    proxy->setCurrentTabWidgetIndex(3);
                }
                if (ev->key() == Qt::Key_F5)
                {
                    proxy->setCurrentTabWidgetIndex(4);
                }
                if (ev->key() == Qt::Key_F6)
                {
                    proxy->setCurrentTabWidgetIndex(5);
                }
                if (ev->key() == Qt::Key_F7)
                {
                    proxy->setCurrentTabWidgetIndex(6);
                }
                if (m_editor->isInObjectEditingState())
                {
                    if (ev->key() >= Qt::Key_1 && ev->key() <= Qt::Key_8)
                    {
                        const int index = ev->key() - Qt::Key_1;
                        proxy->setCurrentTabObjectsIndex(index);
                    }
                }
            }
        }
    }
    bool result = true;
    if (!handled)
    {
        result = QObject::eventFilter(o, e);
    }
    return result;
}
