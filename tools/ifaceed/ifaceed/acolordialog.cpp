#include "acolordialog.h"

AColorDialog::AColorDialog()
{
    m_ui.setupUi(this);
    setWindowTitle("Select a color...");
    connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_ui.cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void AColorDialog::setColorPalette(const QList<QList<QColor> > & palette)
{
    m_ui.widget->setPalette(palette);
}

QList<QList<QColor> > AColorDialog::colorPalette() const
{
    return m_ui.widget->palette();	
}

void AColorDialog::setSelectedColor(const QColor & color)
{
    m_ui.widget->setSelectedColor(color);
}

QColor AColorDialog::selectedColor() const
{
    return m_ui.widget->selectedColor();
}

AColorDialog::~AColorDialog()
{
    
}
