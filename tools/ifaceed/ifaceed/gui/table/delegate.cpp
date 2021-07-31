#include "gui/table/delegate.h"

#include "core/editor.h"

#include "../mainpanelproxy.h"

#include "../../history/database/removeproperty.h"


#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>



gui::table::Delegate::Delegate() 
: m_custom_object(false), 
m_widget(nullptr), 
m_object(nullptr),
m_editor(nullptr),
m_my_widget(nullptr),
m_row(-1)
{
    
}

gui::table::Delegate::~Delegate()
{
    
}

void gui::table::Delegate::makeLinkedTo(QTableWidget* widget,core::Editor* editor)
{
    m_widget = widget;
    m_editor = editor;
}

void gui::table::Delegate::linkToDatabase()
{
    m_custom_object = false;
}

void  gui::table::Delegate::linkToCustomObject(sad::db::custom::Object* o)
{
    m_custom_object = true;
    m_object = o;
}

bool gui::table::Delegate::isLinkedToDatabase() const
{
    return !m_custom_object;
}

bool gui::table::Delegate::isLinkedToCustomObject() const
{
    return m_custom_object;
}

void  gui::table::Delegate::setPropertyName(const QString & name)
{
    m_property_name = name;
}

const QString& gui::table::Delegate::propertyName() const
{
    return m_property_name;
}

void gui::table::Delegate::add()
{
    this->insert(m_widget->rowCount());
}

void gui::table::Delegate::insert(int row)
{
    m_widget->insertRow(row);
    unsigned int lastrow = row;
    m_row = row;

    QTableWidgetItem* item = new QTableWidgetItem(this->propertyName());
    item->setFlags(item->flags()
                   & ~Qt::ItemIsEditable
                   & ~Qt::ItemIsSelectable
                   & ~Qt::ItemIsEnabled
    );
    m_widget->setItem(lastrow, 0, item);

    this->makeEditor();

    if (this->isLinkedToDatabase())
    {		
        QPushButton * button = new QPushButton();
        button->setText("X");
        QFont font = button->font();
        font.setPointSize(16);
        button->setFont(font);
        button->setStyleSheet("QPushButton {color: red}");
        m_widget->setCellWidget(lastrow, 2, button);
        QObject::connect(button, SIGNAL(clicked()), this, SLOT(removeWithCommand()));
    }
    else
    {
        QVariant self;
        self.setValue(this);
        m_widget->item(lastrow, 0)->setData(Qt::UserRole, self);
    }
}

void gui::table::Delegate::disconnectSlots()
{
    this->disconnect();
}

void gui::table::Delegate::remove()
{
    // Find a row in table
    unsigned int row = this->findPropertyInTable();
    if (row != -1)
    {
        this->disconnect();
        if (this->isLinkedToDatabase())
        {
            QObject::disconnect(this, SLOT(removeWithCommand()));
        }
        m_widget->removeRow(row);
        m_my_widget = nullptr;
    }
}

void gui::table::Delegate::removeWithCommand()
{
    m_editor->panelProxy()->takeDelegateByPropertyName(this->propertyName());
    history::database::RemoveProperty* p = new history::database::RemoveProperty(this);
    p->commit(m_editor);
    if (m_editor->currentBatchCommand())
    {
        m_editor->currentBatchCommand()->add(p);
    }
    else
    {
        m_editor->history()->add(p);		
    }
}

int gui::table::Delegate::findPropertyInTable() const
{
    unsigned int findrow = -1;
    for(unsigned int i = 0; i < m_widget->rowCount(); i++)
    {
        if (m_widget->item(i, 0)->text() == this->propertyName())
        {
            findrow = i;
        }
    }
    return findrow;
}


void gui::table::Delegate::insertToTable() const
{
    m_widget->setCellWidget(m_row, 1, m_my_widget);
    m_widget->resizeRowToContents(m_row);
}
