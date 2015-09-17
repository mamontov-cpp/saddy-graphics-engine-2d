#include <scriptinghelp.h>

ScriptingHelp::ScriptingHelp(QWidget* parent) : QDialog(parent)
{
    m_ui.setupUi(this);

    connect(m_ui.txtRequest, SIGNAL(returnPressed()), this,SLOT(applySearch()));
    connect(m_ui.btnCancelSearch, SIGNAL(clicked()), this,SLOT(cancelSearch()));
    m_ui.btnCancelSearch->setDefault(false);
    m_ui.btnCancelSearch->setAutoDefault(false);
}

ScriptingHelp::~ScriptingHelp()
{
}

void ScriptingHelp::applySearch()
{
    QString newrequest = this->m_ui.txtRequest->text();
    if (newrequest != m_last_request)
    {
        cancelSearch();
        m_last_request = newrequest;
        if (newrequest.size())
        {
            m_extra_selections.clear();
            m_ui.txtHelp->moveCursor(QTextCursor::Start);
            while ( m_ui.txtHelp->find(newrequest) )
            {
                QTextEdit::ExtraSelection extra;
                extra.cursor = m_ui.txtHelp->textCursor();
                extra.format.setBackground(QBrush(Qt::yellow));
                m_extra_selections.append(extra);
            }

            m_ui.txtHelp->setExtraSelections(m_extra_selections);
            m_position = 0;
            if (m_extra_selections.size())
            {
                m_ui.txtHelp->setTextCursor(m_extra_selections[m_position].cursor);
                m_ui.txtHelp->ensureCursorVisible();
            }
            m_ui.txtHelp->update();
        }
    }
    else
    {
        if (m_extra_selections.size())
        {
            ++m_position;
            if (m_position == m_extra_selections.size())
            {
                m_position = 0;
            }
            m_ui.txtHelp->setTextCursor(m_extra_selections[m_position].cursor);
            m_ui.txtHelp->ensureCursorVisible();
        }
    }

    
}

void ScriptingHelp::cancelSearch()
{
    QList<QTextEdit::ExtraSelection> s;
    m_ui.txtHelp->setExtraSelections(s);
    m_extra_selections.clear();
}

void ScriptingHelp::setText(const QString& text)
{
    m_ui.txtHelp->setText(text);
}
