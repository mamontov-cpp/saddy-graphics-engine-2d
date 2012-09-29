#include "mainpanel.h"
#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>
#include "gui/fontdelegate.h"
#include "core/ifaceeditor.h"
#include "core/fonttemplatesdatabase.h"
#include "core/fontdatabase.h"
#include "core/spritedatabase.h"
#include "core/mockspritetablewidget.h"


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
	ui.cmbFonts->setItemDelegate(new FontDelegate());
	m_sprite_table = new MockSpriteTableWidget(ui.cmbSpriteConfig,ui.cmbSpriteGroup,ui.cmbSpriteIndex);

}

MainPanel::~MainPanel()
{
	delete m_sprite_table;
}



void MainPanel::closeEvent(QCloseEvent* ev)
{
 this->QMainWindow::closeEvent(ev);
}

void MainPanel::synchronizeDatabase()
{
	ui.cmbFonts->clear();
	FontTemplateDatabase * db = m_editor->database();
	IFaceEditorFontList & list =db->fonts();
	
	for(IFaceEditorFontListCursor it = list.begin(); !(it.end()); it++)
	{
		ui.cmbFonts->addItem(it.name(), QVariant(it.fonts()->qtFont()));
	}
	SpriteDatabase & sprites = db->sprites();
	AbstractSpriteDatabaseIterator * it = sprites.begin();
	for(it; !(it->isEnd());it->next())
	{
		m_sprite_table->add(*it);
	}
	delete it;
}