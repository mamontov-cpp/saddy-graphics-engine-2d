#include "mainpanel.h"
#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>

class FontDelegate: public QItemDelegate
{
	void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
	{
		this->QItemDelegate::paint(painter,option,index);
		painter->setPen(QColor(255,0,0));
		painter->fillRect(option.rect,QColor(255,0,0));
		
	}
};

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	bool result = QFontDatabase::removeAllApplicationFonts();
	connect(ui.cmbFonts,SIGNAL(currentIndexChanged(int)),this,SLOT(ifCleanupSystemFonts(int)));
	ui.cmbFonts->setItemDelegate(new FontDelegate());
	ui.cmbFonts->addItem("abc");
	ui.cmbFonts->addItem("dfg");
	ui.cmbFonts->addItem("eps");

}

MainPanel::~MainPanel()
{

}
void MainPanel::ifCleanupSystemFonts(int)
{
}


void MainPanel::closeEvent(QCloseEvent* ev)
{
 this->QMainWindow::closeEvent(ev);
}