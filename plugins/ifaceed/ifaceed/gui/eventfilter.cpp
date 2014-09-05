#include "eventfilter.h"

#include "../mainpanel.h"
#include "../core/editor.h"

gui::EventFilter::EventFilter(QObject *parent) :
    QObject(parent)
{
}

void gui::EventFilter::setPanel(MainPanel* panel)
{
    m_panel = panel;
}

 bool gui::EventFilter::eventFilter(QObject *o, QEvent *e)
 {
     bool handled = false;
     if(e->type() == QEvent::KeyPress && m_panel)
     {
        QKeyEvent* ev = static_cast<QKeyEvent*>(e);
        if (ev->modifiers() & Qt::ControlModifier)
        {
            if (ev->key() == Qt::Key_Z)
            {
                handled = true;
                m_panel->editor()->undo();
            }
            if (ev->key() == Qt::Key_R)
            {
                handled = true;
                m_panel->editor()->redo();
            }
            if (ev->key() >= Qt::Key_1 && ev->key() <= Qt::Key_7)
            {
                int index = ev->key() - Qt::Key_1;
                m_panel->myUI()->tabWidget->setCurrentIndex(index);
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
