#include "gui/rectwidget/rectwidget.h"

gui::rectwidget::RectWidget::RectWidget(QWidget * parent) : QWidget(parent)
{
	m_layout = new QGridLayout(this);

	const int labelnamescount = 4;
	const int maxlabelnamecount = 10;
	char labelnames[labelnamescount][maxlabelnamecount] = {
		"X: ", 
		"Y: ", 
		"Width: ", 
		"Height: " 
	};
	QDoubleSpinBox** boxes[labelnamescount] = {
		&m_x,
		&m_y,
		&m_width,
		&m_height
	};
	for(int i = 0; i < labelnamescount; i++)
	{
		m_labels[i] = new QLabel(this);
		m_labels[i]->setText(labelnames[i]);
		m_labels[i]->setMinimumSize(m_labels[i]->sizeHint());

		*(boxes[i]) = new QDoubleSpinBox(this);
		QDoubleSpinBox * box = *(boxes[i]);
		box->setMinimum(0);
		box->setMaximum(1000);
		box->setMinimumSize(box->sizeHint());
		box->setValue(0);
	}
	
	m_layout->addWidget(m_labels[0], 0, 0);
	m_layout->addWidget(m_x, 0, 1);
	m_layout->addWidget(m_labels[1], 1, 0);
	m_layout->addWidget(m_y, 1, 1);
	m_layout->addWidget(m_labels[2], 0, 2);
	m_layout->addWidget(m_width, 0, 3);
	m_layout->addWidget(m_labels[3], 1, 2);
	m_layout->addWidget(m_height, 1, 3);

	this->setLayout(m_layout);

	connect(m_x, SIGNAL(valueChanged(double)), this, SLOT(xChanged(double)));
	connect(m_y, SIGNAL(valueChanged(double)), this, SLOT(yChanged(double)));
	connect(m_width, SIGNAL(valueChanged(double)), this, SLOT(widthChanged(double)));
	connect(m_height, SIGNAL(valueChanged(double)), this, SLOT(heightChanged(double)));
}


void gui::rectwidget::RectWidget::setValue(const QRectF & value)
{
	const int boxescount = 4;
	QDoubleSpinBox* boxes[boxescount] = {
		m_x,
		m_y,
		m_width,
		m_height
	};
	bool flags[boxescount];
	for(int i = 0; i < boxescount; i++)
	{
		flags[i] = boxes[i]->blockSignals(true);
	}

	m_x->setValue(value.left());
	m_y->setValue(value.top());
	m_width->setValue(value.width());
	m_height->setValue(value.height());

	for(int i = 0; i < boxescount; i++)
	{
		boxes[i]->blockSignals(flags[i]);
	}
}

QRectF gui::rectwidget::RectWidget::value() const
{
	return QRectF(m_x->value(), m_y->value(), m_width->value(), m_height->value());
}

void gui::rectwidget::RectWidget::xChanged(double d)
{
	emit valueChanged(QRectF(
		d,
		m_y->value(),
		m_width->value(),
		m_height->value()
	));
}

void gui::rectwidget::RectWidget::yChanged(double d)
{
	emit valueChanged(QRectF(
		m_x->value(),
		d,
		m_width->value(),
		m_height->value()
	));
}

void gui::rectwidget::RectWidget::widthChanged(double d)
{
	emit valueChanged(QRectF(
		m_x->value(),
		m_y->value(),
		d,
		m_height->value()
	));
}

void gui::rectwidget::RectWidget::heightChanged(double d)
{
	emit valueChanged(QRectF(
		m_x->value(),
		m_y->value(),
		m_width->value(),
		d
	));
}

