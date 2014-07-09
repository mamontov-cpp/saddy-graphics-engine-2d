#include "gui/fontsizewidget/fontsizewidget.h"

#include <QInputDialog>

gui::fontsizewidget::FontSizeWidget::FontSizeWidget(QWidget * parent) : QWidget(parent)
{
	m_layout = new QVBoxLayout(this);
	
	m_size_label = new QLabel(this);
	m_size_label->setText("Size: ");
		
	m_sizes = new QComboBox(this);
	for(int i = 1 ; i < 73; i++) 
	{
		m_sizes->addItem(QString::number(i), QVariant(i));
	}
	// Pick 10 as default value
	m_sizes->setCurrentIndex(9);
	
	m_add = new QPushButton("Pick other", this);

	m_size_label->setMinimumSize(m_size_label->sizeHint());
	m_sizes->setMinimumSize(m_sizes->sizeHint());
	m_add->setMinimumSize(m_add->sizeHint());
	
	m_layout->addWidget(m_size_label,1);
	m_layout->addWidget(m_sizes,1);
	m_layout->addWidget(m_add,1);

	this->setLayout(m_layout);

	

	connect(m_sizes, SIGNAL(currentIndexChanged(int)), this, SLOT(sizeChanged(int)));
	connect(m_add, SIGNAL(clicked()), this, SLOT(addSizeClicked()));
}

void gui::fontsizewidget::FontSizeWidget::setValue(unsigned int value)
{
	int foundindex = m_sizes->findData(QVariant((int)value), Qt::UserRole);	
	if (foundindex != -1)
	{
		bool b = m_sizes->blockSignals(true);
		m_sizes->setCurrentIndex(foundindex);
		m_sizes->blockSignals(b);
	}
	else
	{
		m_sizes->addItem(QString::number(value), QVariant(value));
		bool b = m_sizes->blockSignals(true);
		m_sizes->setCurrentIndex(m_sizes->count() - 1);
		m_sizes->blockSignals(b);
	}
	emit valueChanged(value);
}

unsigned int gui::fontsizewidget::FontSizeWidget::value() const
{
	int result = 0;
	int index = m_sizes->currentIndex();
	if (index != -1) 
	{
		result = m_sizes->itemData(index, Qt::UserRole).toInt();
	}
	return (unsigned int)result;
} 

void gui::fontsizewidget::FontSizeWidget::sizeChanged(int index)
{
	if (index != -1) 
	{
		int result = m_sizes->itemData(index, Qt::UserRole).toInt();
		emit valueChanged((unsigned int)result);
	}
}

void gui::fontsizewidget::FontSizeWidget::addSizeClicked()
{
	QInputDialog d(this);
    d.setInputMode(QInputDialog::IntInput);
    d.setWindowTitle("Input some font size");
    d.setIntRange(73, 1000);
    d.exec();
    if (d.result() == QDialog::Accepted)
    {
        int size = d.intValue();
		bool b = m_sizes->blockSignals(true);
        m_sizes->addItem(QString::number(size), QVariant(size));
        m_sizes->setCurrentIndex(m_sizes->count()-1);
		m_sizes->blockSignals(b);
    }
}
