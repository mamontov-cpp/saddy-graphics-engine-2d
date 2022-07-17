#include "doubletuplewidget.h"

#include <QHeaderView>

#include <climits>

gui::tuplewidget::DoubleTupleWidget::DoubleTupleWidget(QWidget* parent) : QTableWidget(parent)
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

    m_first = new QDoubleSpinBox();
    m_first->setRange(-100000, 100000);
    m_first->setValue(0);
    this->setCellWidget(0, 1, m_first);

    item = new  QTableWidgetItem();
    item->setFlags(item->flags() 
                 & ~Qt::ItemIsEditable 
                 & ~Qt::ItemIsSelectable
                 & ~Qt::ItemIsEnabled
    );
    this->setItem(1, 0, item);

    m_second = new QDoubleSpinBox();
    m_second->setRange(-100000, 100000);
    m_second->setValue(0);
    this->setCellWidget(1, 1, m_second);

    QObject::connect(m_first, SIGNAL(valueChanged(double)), this, SLOT(firstChanged(double)));
    QObject::connect(m_second, SIGNAL(valueChanged(double)), this, SLOT(secondChanged(double)));
}

gui::tuplewidget::DoubleTupleWidget::~DoubleTupleWidget()
{
    QObject::disconnect(this, SLOT(firstChanged(double)));
    QObject::disconnect(this, SLOT(secondChanged(double)));	
}

void gui::tuplewidget::DoubleTupleWidget::setLabel(const QString& first, const QString& second)
{
    this->item(0, 0)->setText(first);
    this->item(1, 0)->setText(second);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
}

QPair<QString, QString> gui::tuplewidget::DoubleTupleWidget::label() const
{
    return QPair<QString, QString>(this->item(0, 0)->text(), this->item(1, 0)->text());
}

void gui::tuplewidget::DoubleTupleWidget::setRange(double minimum, double maximum)
{
    m_first->setRange(minimum, maximum);
    m_second->setRange(minimum, maximum);
}

QPair<double, double>  gui::tuplewidget::DoubleTupleWidget::range() const
{
    return QPair<double, double>(m_first->minimum(), m_first->maximum());
}


void gui::tuplewidget::DoubleTupleWidget::setValue(double first, double second)
{
    bool b[2] = { m_first->blockSignals(true), m_second->blockSignals(true) };
    m_first->setValue(first);
    m_second->setValue(second);
    m_first->blockSignals(b[0]);
    m_second->blockSignals(b[1]);
}

QPair<double, double> gui::tuplewidget::DoubleTupleWidget::value() const
{
    return 	QPair<double, double>(m_first->value(), m_second->value());
}


void gui::tuplewidget::DoubleTupleWidget::firstChanged(double v)
{
    emit valueChanged(v, m_second->value());
}

void gui::tuplewidget::DoubleTupleWidget::secondChanged(double v)
{
    emit valueChanged(m_first->value(), v);
}


