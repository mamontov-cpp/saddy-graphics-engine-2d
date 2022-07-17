#include "int64tuplewidget.h"

#include <QHeaderView>

#include <climits>

gui::tuplewidget::Int64TupleWidget::Int64TupleWidget(QWidget* parent) : QTableWidget(parent)
{
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    this->setRowCount(2);
    this->setColumnCount(2);

    QTableWidgetItem * item = new  QTableWidgetItem();
    item->setFlags(item->flags() 
                 & ~Qt::ItemIsEditable 
                 & ~Qt::ItemIsSelectable
                 & ~Qt::ItemIsEnabled
    );
    this->setItem(0, 0, item);

    m_first = new QwwLongSpinBox();
    m_first->setRange(LLONG_MIN, LLONG_MAX);
    m_first->setValue(0);
    this->setCellWidget(0, 1, m_first);

    item = new  QTableWidgetItem();
    item->setFlags(item->flags() 
                 & ~Qt::ItemIsEditable 
                 & ~Qt::ItemIsSelectable
                 & ~Qt::ItemIsEnabled
    );
    this->setItem(1, 0, item);

    m_second = new QwwLongSpinBox();
    m_second->setRange(LLONG_MIN, LLONG_MAX);
    m_second->setValue(0);
    this->setCellWidget(1, 1, m_second);

    QObject::connect(m_first, SIGNAL(valueChanged(qlonglong)), this, SLOT(firstChanged(qlonglong)));
    QObject::connect(m_second, SIGNAL(valueChanged(qlonglong)), this, SLOT(secondChanged(qlonglong)));
}

gui::tuplewidget::Int64TupleWidget::~Int64TupleWidget()
{
    QObject::disconnect(this, SLOT(firstChanged(qlonglong)));
    QObject::disconnect(this, SLOT(secondChanged(qlonglong)));	
}

void gui::tuplewidget::Int64TupleWidget::setLabel(const QString& first, const QString& second)
{
    this->item(0, 0)->setText(first);
    this->item(1, 0)->setText(second);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
}

QPair<QString, QString> gui::tuplewidget::Int64TupleWidget::label() const
{
    return QPair<QString, QString>(this->item(0, 0)->text(), this->item(1, 0)->text());
}

void gui::tuplewidget::Int64TupleWidget::setRange(qlonglong minimum, qlonglong maximum)
{
    m_first->setRange(minimum, maximum);
    m_second->setRange(minimum, maximum);
}

QPair<qlonglong, qlonglong>  gui::tuplewidget::Int64TupleWidget::range() const
{
    return QPair<qlonglong, qlonglong>(m_first->minimum(), m_first->maximum());
}


void gui::tuplewidget::Int64TupleWidget::setValue(qlonglong first, qlonglong second)
{
    bool b[2] = { m_first->blockSignals(true), m_second->blockSignals(true) };
    m_first->setValue(first);
    m_second->setValue(second);
    m_first->blockSignals(b[0]);
    m_second->blockSignals(b[1]);
}

QPair<qlonglong, qlonglong> gui::tuplewidget::Int64TupleWidget::value() const
{
    return 	QPair<qlonglong, qlonglong>(m_first->value(), m_second->value());
}


void gui::tuplewidget::Int64TupleWidget::firstChanged(qlonglong v)
{
    emit valueChanged(v, m_second->value());
}

void gui::tuplewidget::Int64TupleWidget::secondChanged(qlonglong v)
{
    emit valueChanged(m_first->value(), v);
}


