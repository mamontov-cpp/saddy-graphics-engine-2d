#include "eventfilter.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include <QTabWidget>

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
			if (ev->key() == Qt::Key_F1)
			{
				if (m_panel->editor()->isInWaysEditingState())
				{
					m_panel->editor()->tryEnterObjectEditingState();
				}
			}
			if (ev->key() == Qt::Key_F2)
			{
				if (m_panel->editor()->isInObjectEditingState())
				{
					m_panel->editor()->tryEnterWayEditingState();
				}
			}
			if (ev->key() == Qt::Key_F3)
			{
				m_panel->editor()->machine()->enterState("idle");
				m_panel->UI()->tabTypes->setCurrentIndex(2);
			}
			if (m_panel->editor()->isInObjectEditingState())
			{
				if (ev->key() >= Qt::Key_1 && ev->key() <= Qt::Key_7)
				{
					int index = ev->key() - Qt::Key_1;
					m_panel->UI()->tabObjects->setCurrentIndex(index);
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
