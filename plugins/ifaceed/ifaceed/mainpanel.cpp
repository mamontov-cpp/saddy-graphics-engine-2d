#include "mainpanel.h"
#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>
#include <QColorDialog>
#include <QInputDialog>
#include <marshal/actioncontext.h>
#include "gui/fontdelegate.h"
#include "gui/colordelegate.h"
#include "core/ifaceeditor.h"
#include "core/fonttemplatesdatabase.h"
#include "core/fontdatabase.h"
#include "core/spritedatabase.h"
#include "core/mockspritetablewidget.h"
#include "editorcore/editorbehaviourshareddata.h"
#include "objects/screenlabel.h"

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	connect(ui.btnPickFontColor,SIGNAL(clicked()),this,SLOT(addNewFontColor()));
	connect(ui.btnPickFontSize,SIGNAL(clicked()), this, SLOT(addNewFontSize()));
	connect(ui.btnAddLabel, SIGNAL(clicked()), this, SLOT(addFontObject()));

	ui.cmbFonts->setItemDelegate(new FontDelegate());
	ui.cmbFontColor->setItemDelegate(new ColorDelegate());
	m_sprite_table = new MockSpriteTableWidget(ui.cmbSpriteConfig,ui.cmbSpriteGroup,ui.cmbSpriteIndex);

	QColor colors[] = { Qt::red, 
						Qt::darkRed,
						Qt::blue,
						Qt::darkBlue,
						Qt::green,
						Qt::darkGreen,
						Qt::white, 
						Qt::magenta, 
						Qt::yellow, 
						Qt::black 
	                  };
	for (int i=0;i<10;i++)
	{
		QString text = QString::number(colors[i].red()) + QString(",")
			         + QString::number(colors[i].green()) + QString(",")
					 + QString::number(colors[i].blue());
		text = QString("(") + text + QString(")");
		ui.cmbFontColor->addItem(text, QVariant(colors[i]));
	}
	// Populate font size
	for (int i=5;i<201;i++)
	{
		ui.cmbFontSize->addItem(QString::number(i),QVariant(i));
	}

	// Add SpriteViewer
	QRectF contentRect = QRectF(ui.spriteViewerPad->contentsRect());

	m_spriteTableWidget = new QSpriteTableWidget(ui.cmbSpriteConfig, contentRect);

	QGridLayout* grPadLayout = new QGridLayout;
	grPadLayout->addWidget(m_spriteTableWidget);
	ui.spriteViewerPad->setLayout(grPadLayout);
	// TODO: Uncomment, when Ryaskov adds his patch.
	// m_spriteTableWidget->addToForm(this);
	
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
		// TODO: Ryaskov
		// When bug will be fixed, uncomment this line and comment upper line
		// m_spriteTableWidget->add(*it);
	}
	delete it;
}

void MainPanel::addNewFontColor()
{
	QColor initial(0,0,0);
	if (ui.cmbFontColor->currentIndex()!=-1)
	{
		initial = ui.cmbFontColor->itemData(ui.cmbFontColor->currentIndex()).value<QColor>();
	}

	QColor to_add = QColorDialog::getColor(initial,this,"Pick a new color for label");
	if (to_add.isValid())
	{
		QString text = QString::number(to_add.red()) + QString(",")
			         + QString::number(to_add.green()) + QString(",")
					 + QString::number(to_add.blue());
		text = QString("(") + text + QString(")");
		ui.cmbFontColor->addItem(text, QVariant(to_add));
		ui.cmbFontColor->setCurrentIndex(ui.cmbFontColor->count()-1);
	}
}


void MainPanel::addNewFontSize()
{
	QInputDialog d(this);
	d.setInputMode(QInputDialog::IntInput);
	d.setWindowTitle("Input some font size");
	d.setIntRange(201,1000);
	d.exec();
	if (d.result() == QDialog::Accepted)
	{
		int size = d.intValue();
		ui.cmbFontSize->addItem(QString::number(size), QVariant(size));
		ui.cmbFontSize->setCurrentIndex(ui.cmbFontSize->count()-1);
	}
}


void MainPanel::setMouseMovePosView(float x, float y)
{
	m_tmp_mov_x = x;
	m_tmp_mov_y = y;

	QTimer::singleShot(0, this, SLOT(setMouseMovePosViewImpl()));
}

void MainPanel::setMouseMovePosViewImpl()
{
	ui.txtMousePosX->setText(QString::number(m_tmp_mov_x));
	ui.txtMousePosY->setText(QString::number(m_tmp_mov_y));
}

void MainPanel::highlightStateImpl()
{
	ui.txtEditorState->setText(m_tmp_state);
}

void MainPanel::highlightState(const hst::string & hints)
{
	m_tmp_state = hints.data();
	QTimer::singleShot(0, this, SLOT(highlightStateImpl()));
}


void MainPanel::addFontObject()
{
	if (ui.txtLabelText->toPlainText().length() == 0)
	{
		QMessageBox::critical(NULL, "IFace Editor", "You must specify label text to add it");
	}
	else
	{
		ScreenLabel * label = new ScreenLabel();
		label->setActive(true);
		label->setVisible(true);
		
		// Set props
		ActionContext c;
		hst::string fontName=ui.cmbFonts->currentText().toStdString().c_str();
		label->getProperty("font")->set(sad::Variant(fontName),&c);
		QColor qcolor = ui.cmbFontColor->itemData(ui.cmbFontColor->currentIndex()).value<QColor>();
		hst::color hcolor(qcolor.red(), qcolor.green(), qcolor.blue());
		label->getProperty("color")->set(sad::Variant(hcolor), &c);
		label->getProperty("pos")->set(sad::Variant(hPointF(0,0)), &c);
		float angle = ui.dblAngle->value();
		label->getProperty("angle")->set(sad::Variant(angle), &c);
		unsigned int size = ui.cmbFontSize->itemData(ui.cmbFontSize->currentIndex()).value<int>();
		label->getProperty("size")->set(sad::Variant(size), &c);
		hst::string text=ui.txtLabelText->toPlainText().toStdString().c_str();
		label->getProperty("text")->set(sad::Variant(text), &c);


		label->tryReload(this->m_editor->database());
		label->setScene(static_cast<InterlockedScene*>(this->m_editor->scene()));
		this->m_editor->behaviourSharedData()->setSelectedObject(label);
		this->m_editor->scene()->markForAddition(label);

	}
}

