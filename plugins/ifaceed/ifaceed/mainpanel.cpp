#include "mainpanel.h"
#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>
#include "gui/fontdelegate.h"

/*
class FontDelegate: public QItemDelegate
{
	void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
	{
		this->QItemDelegate::paint(painter,option,index);
		painter->setPen(QColor(255,0,0));
		painter->fillRect(option.rect,QColor(255,0,0));
		
	}
};
*/

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	QFontDatabase db;
	QFont fnt;
	int id = db.addApplicationFont("AvQest.ttf");
	if (id!=-1) {
		QStringList lst = db.applicationFontFamilies ( id );
		fnt = db.font(lst[0],"",18);
		bool ok = (fnt!=QFont());
	}
	connect(ui.cmbFonts,SIGNAL(currentIndexChanged(int)),this,SLOT(ifCleanupSystemFonts(int)));
	ui.cmbFonts->setItemDelegate(new FontDelegate());
	ui.cmbFonts->addItem("abc", QVariant(fnt));
	ui.cmbFonts->addItem("dfg", QVariant(fnt));
	ui.cmbFonts->addItem("eps", QVariant(fnt));

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